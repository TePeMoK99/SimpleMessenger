#pragma once

#include <QSqlQuery>

#include "connectionmanager.h"

namespace CRUD
{
class Executor
{
public:
    Executor();
    Q_DISABLE_COPY(Executor)

    std::pair<RESULT, QSqlQuery> execute(const QString& queryText, const QVariantList &args = {});


private:
    ConnectionManager& m_connectionManager;
};
QString stringListToString(const QStringList &params);
QString makeANDexpression(const QStringList &fields, const QVariantList &data);
}
