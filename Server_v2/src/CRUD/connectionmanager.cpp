#include "connectionmanager.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

namespace CRUD
{
ConnectionManager::~ConnectionManager()
{

}

ConnectionManager &ConnectionManager::instance()
{
    static ConnectionManager instance {};
    return instance;
}

QString ConnectionManager::dbPath() const
{
    return m_managerPrivate->m_databasePath;
}

STATE ConnectionManager::state() const
{
    return m_managerPrivate->m_state;
}

bool ConnectionManager::isValid() const
{
    return m_managerPrivate->m_isValid;
}

ConnectionManager::ConnectionManager()
    : m_managerPrivate(new ConnectionManagerPrivate())
{
    const bool setup_result {m_managerPrivate->setup()};
    m_managerPrivate->setIsValid(setup_result);
}

bool ConnectionManagerPrivate::setup()
{
    const QString driver {"QSQLITE"};

    if(!QSqlDatabase::isDriverAvailable(driver))
    {
        m_state = STATE::ERROR_NO_DRIVER;
        qWarning() << driver << " is not available";

        return false;
    }

    if(!setupWorkspace())
    {
        m_state = STATE::ERROR_WORKSPACE;
        qWarning() << "Workspace setup failde";
        return false;
    }

    QSqlDatabase* database {new QSqlDatabase(QSqlDatabase::addDatabase(driver))};
    m_database.reset(database);
    m_database->setDatabaseName(m_databasePath);

    if(!m_database->open())
    {
        m_state = STATE::ERROR_OPENING;
        qWarning() << "Opening database" << m_database->databaseName() << "failed"
                   << " reason:" << m_database->lastError().text();
        return false;
    }

    qDebug() << "Openning database success";

    return true;
}

bool ConnectionManagerPrivate::setupWorkspace()
{
    const QString database_name {"db.db"};
    const QString location {"C:\\Users\\pyankov\\Documents\\Qt_Projects\\SimpleMessenger\\Server_v2"};
    const QString fullpath {location + "\\" + database_name};

    m_databasePath = fullpath;

    QDir database_directory(location);

    if(!database_directory.exists())
    {
        const bool result {database_directory.mkpath(location)};
        qWarning() << "DB directory not exist, creating result: " << result;
    }

    qDebug() << "Database path: " << fullpath;

    return database_directory.exists();
}

bool ConnectionManagerPrivate::setupTables()
{
    bool result {true};

    std::vector<QSqlQuery> creation_queries
    {
        QSqlQuery
        {
            ""
        }
    };

    for(auto& query : creation_queries)
    {
        if(!query.exec())
        {
            result = false;
            m_state = STATE::ERROR_TABLES;
            qWarning() << "Table creation failed. Reason: " << query.lastError();
        }
        else
        {
            qWarning() << "Table successfully created! Query: \n" << query.lastQuery();
        }
    }

    return result;
}

void ConnectionManagerPrivate::setIsValid(const bool &valid)
{
    if(m_isValid != valid)
    {
        m_isValid = valid;
    }
}

void CRUDCloser::operator()(QSqlDatabase *db)
{
    db->close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    delete db;
}

}
