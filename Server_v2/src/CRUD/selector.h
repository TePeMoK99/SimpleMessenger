#pragma once

#include <QVariant>

#include "crudtypes.h"
#include "executor.h"

namespace CRUD
{
class Selector
{
public:
    std::pair<RESULT, bool> rowExist(const QString &data, const QString &field_name, const QString &table_name);
    std::pair<RESULT, bool> rowContains(const QString &table_name, const QStringList &fields, const QVariantList &row_data);
    std::pair<RESULT, QStringList> requestGroupsList();


private:
    Executor m_executor;
};

}
