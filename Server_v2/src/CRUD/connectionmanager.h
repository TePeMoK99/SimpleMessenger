#pragma once

#include <QObject>
#include <QString>
#include <QSqlDatabase>

#include <memory>

#include "crudtypes.h"

namespace CRUD
{
class CRUDCloser
{
public:
    void operator()(QSqlDatabase *db);
};

class ConnectionManagerPrivate
{
public:
    std::unique_ptr<QSqlDatabase, CRUDCloser> m_database;
    QString m_databasePath;
    STATE m_state = STATE::OK;
    bool m_isValid = true;

    bool setup();
    bool setupWorkspace();
    bool setupTables();
    void setIsValid(const bool &valid);
};

class ConnectionManager
{
public:
    Q_DISABLE_COPY(ConnectionManager)
    ~ConnectionManager();
    static ConnectionManager &instance();

    QString dbPath() const;
    STATE state() const;
    bool isValid() const;

private:
    ConnectionManager();
    std::unique_ptr<ConnectionManagerPrivate> m_managerPrivate;
};
}
