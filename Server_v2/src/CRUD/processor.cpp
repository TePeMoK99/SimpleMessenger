#include "processor.h"
#include "crudmapper.h"

namespace CRUD
{
Processor::Processor()
    : m_processor_private(new ProcessorPrivate())
{ }

Processor::~Processor()
{

}

Processor *Processor::instance()
{
    static Processor inst;
    return &inst;
}

bool Processor::userExist(const QString &nickname)
{
    return m_processor_private->selector.rowExist(nickname, "nickname", "ChatUser").second;
}

bool Processor::groupExist(const QString &group_name)
{
    return m_processor_private->selector.rowExist(group_name, "name", "ChatGroup").second;
}

bool Processor::registerUser(const QString &nickname, const QString &password)
{
    CRUD::RESULT result;
    int last_row;
    std::tie(result, last_row) = m_processor_private->manipulator.insertRow(
                "ChatUser", {"nickname", "password"}, {nickname, password}
                );

    return result == CRUD::RESULT::SUCCESS ? true : false;
}

bool Processor::registerGroup(const QString &group_name, const QString &group_password)
{
    CRUD::RESULT result;
    int last_row;
    std::tie(result, last_row) = m_processor_private->manipulator.insertRow(
                "ChatGroup", {"name", "password"}, {group_name, group_password}
                );
    return result == CRUD::RESULT::SUCCESS ? true : false;
}

bool Processor::checkUserPassword(const QString &nickname, const QString &password)
{
    return m_processor_private->selector.rowContains("ChatUser", {"nickname", "password"}, {nickname, password}).second;
}

bool Processor::checkGroupPassword(const QString &group_name, const QString &group_password)
{
    return m_processor_private->selector.rowContains("ChatGroup", {"nickname", "password"}, {group_name, group_password}).second;
}

std::vector<std::pair<QString, QString> > Processor::requestGroupsList()
{
    CRUD::RESULT result;
    std::vector<std::pair<QString, QString>> result_vector;
    std::tie(result, result_vector) = m_processor_private->selector.requestGroupsList();
    if (result == RESULT::SUCCESS)
    {
        return result_vector;
    }

    qCritical() << "Can't request group's list";

    return {};
}
}
