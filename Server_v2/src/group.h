#pragma once
#include <QObject>
#include <QHash>

namespace Server
{
class TCPClient;
struct Group
{
    Group() : name {"None"}, clients {}
    { }

    Group(const QString &name) :
        name {name}
    { }

    QString name;
    QHash<QString, TCPClient*> clients;
};
}
