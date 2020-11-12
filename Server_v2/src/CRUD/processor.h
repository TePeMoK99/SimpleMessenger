#pragma once

#include <memory>
#include <QMutex>
#include <QDebug>

#include "selector.h"
#include "manipulator.h"

namespace CRUD
{
class ProcessorPrivate
{
public:
    Manipulator manipulator;
    Selector selector;
};

class Processor
{
public:
    Q_DISABLE_COPY(Processor)
    ~Processor();

    static Processor *instance();

    std::pair<RESULT, std::vector<QVariantList>> requestTableData(TABLES table);
    bool userExist(const QString &nickname);
    bool groupExist(const QString &group_name);
    bool registerUser(const QString &nickname, const QString &password);
    bool registerGroup(const QString &group_name, const QString &group_password);
    bool checkUserPassword(const QString &nickname, const QString &password);
    bool checkGroupPassword(const QString &group_name, const QString &group_password);
    std::vector<std::pair<QString, QString>> requestGroupsList();

private:
    Processor();
    std::unique_ptr<ProcessorPrivate> m_processor_private;
};
}
