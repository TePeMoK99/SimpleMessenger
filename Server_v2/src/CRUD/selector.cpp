#include "selector.h"

#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

namespace CRUD
{

std::pair<RESULT, bool> Selector::rowExist(const QString &data, const QString &field_name, const QString &table_name)
{
    QString request {"SELECT "
                     "COUNT(*) FROM " + table_name +
                " WHERE " + makeANDexpression({field_name}, {data})
                    };
    QSqlQuery result_query;
    RESULT result;
    std::tie(result, result_query) = m_executor.execute(request);
    result_query.next();

    return {result, result_query.value(0).toBool()};
}

std::pair<RESULT, bool> Selector::rowContains(const QString &table_name, const QStringList &fields, const QVariantList &row_data)
{
    QString request {"SELECT "
                     "COUNT(*) FROM " + table_name
                + " WHERE " + makeANDexpression(fields, row_data)
                    };

    QSqlQuery result_query;
    RESULT result;
    std::tie(result, result_query) = m_executor.execute(request);
    result_query.next();

    return {result, result_query.value(0).toBool()};
}

std::pair<RESULT, std::vector<std::pair<QString, QString> > > Selector::requestGroupsList()
{
    QString request {"SELECT "
                     "name, password "
                     "FROM ChatGroup"
                    };
    QSqlQuery result_query;
    RESULT result;
    std::tie(result, result_query) = m_executor.execute(request);

    std::vector<std::pair<QString, QString>> result_vector;
    while(result_query.next())
    {
        result_vector.push_back({result_query.value(0).toString(), result_query.value(1).toString()});
    }

    return {result, result_vector};
}
}
