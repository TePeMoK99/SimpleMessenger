#pragma once

#include <QStringList>

struct GroupListItem
{
    GroupListItem(const QString &name, const QString &password) :
        group_name {name}, password {password}
    { }

    QString group_name;
    QString password;
    QStringList client_list;
};
