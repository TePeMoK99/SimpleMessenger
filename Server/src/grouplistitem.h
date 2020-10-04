#pragma once

#include <QStringList>
#include "mytcpclient.h"

class MyTcpClient;

struct GroupListItem
{
    GroupListItem() : name {"None"}, password {""}, clients {}
    { }

    GroupListItem(const QString &name, const QString &password) :
        name {name}, password {password}
    { }

    QString name;
    QString password;
    QHash<QString, MyTcpClient*> clients;
};
