#include "manipulator.h"
#include "crudmapper.h"

#include <QVariant>
#include <QDebug>
#include <QSqlQuery>
#include <sstream>

namespace CRUD
{

std::pair<RESULT, int> Manipulator::insertRow(const QString &table_name, const QStringList &fields, const QVariantList &row_data)
{
    const QString& query {generateInsertQuery(table_name, fields)};
    const std::pair<RESULT, QSqlQuery> &result {m_executor.execute(query, row_data)};

    return { result.first, result.second.lastInsertId().toInt()};
}

QString Manipulator::generateInsertQuery(const QString &table_name, const QStringList &fields) const
{
    QString query {"INSERT INTO " + table_name +" ("};
    for (auto i : fields)
    {
        query += (i + ",");
    }
    query.remove(query.length() - 1, 1);
    query += ") VALUES (";
    query += generateBindString(fields.size()) + ")";

    return query;
}

QString Manipulator::generateBindString(const size_t &param_count) const
{
    std::ostringstream bindings;
    std::fill_n(std::ostream_iterator<std::string>(bindings), param_count, "?,");
    std::string bindString {bindings.str()};
    bindString.pop_back(); // - ","

    return QString::fromStdString(bindString);
}

}
