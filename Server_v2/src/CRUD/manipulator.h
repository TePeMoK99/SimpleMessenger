#pragma once

#include "executor.h"
#include "crudtypes.h"

#include <QVariant>

namespace CRUD
{
class Manipulator
{
public:
    std::pair<RESULT, int> insertRow(const QString &table_name, const QStringList &fields, const QVariantList &row_data);

private:
    QString generateBindString(const size_t &param_count) const;
    QString generateInsertQuery(const QString &table_name, const QStringList &fields) const;

    Executor m_executor;
};
}
