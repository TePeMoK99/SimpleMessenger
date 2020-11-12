#pragma once
#include <QObject>
#include <QHash>

namespace Server
{
class TCPClient;
struct Group
{
    Group() : name {"None"}, password {""}, clients {}
    { }

    Group(const QString &name, const QString &password) :
        name {name}, password {password}
    { }

    QString name;
    QString password;
    QHash<QString, TCPClient*> clients;
};
}
