#include "mytcpserver.h"
#include <QDataStream>
#include "types.h"

MyTcpServer::MyTcpServer()
{
    groups.insert("None", GroupListItem());
    groups.insert("Main", GroupListItem("Main", "0000"));
}

bool MyTcpServer::start(const QHostAddress &address, const int &port)
{
    if (!listen(address, port))
    {
        qDebug() << "Server doesn't started at " << address.toString() << ":" << port;
        return false;
    }
    qDebug() << "Server successfuly started at " << address.toString() << ":" << port;

    return true;
}

bool MyTcpServer::start(const QString &address_str, const int &port)
{
    QHostAddress address {address_str};
    if (!listen(address, port))
    {
        qDebug() << "Server doesn't started at " << address.toString() << ":" << port;
        return false;
    }
    qDebug() << "Server successfuly started at " << address.toString() << ":" << port;

    return true;
}

void MyTcpServer::sendMsgToAll(const QString &group_name, const QString &message, const QString &sender) const
{
    const QByteArray data {makeByteArray(MessageTypes::PUBLIC_MESSAGE, {sender, message})};

    for (auto i : groups[group_name].clients)
    {
        if (i->client_name != sender)
            i->tcp_socket->write(data);
    }
    qDebug() << "Group [" << group_name << "] Message [" << message << "] from " << sender << "sent to all";
}

void MyTcpServer::sendMsgToUser(const QString &group_name, const QString &message,
                                    const QString &reciever, const QString &sender) const
{
    const QByteArray data {makeByteArray(MessageTypes::PRIVATE_MESSAGE, {sender, message})};

    groups[group_name].clients[reciever]->tcp_socket->write(data);


    qDebug() << "Group [" << group_name << "] Message [" << message << "] from " << sender << "sent to " << reciever;
}

void MyTcpServer::sendMsgUserJoin(const QString &group_name, const QString &new_user) const
{
    const QByteArray data {makeByteArray(MessageTypes::USER_JOIN, {new_user})};

    for (auto i : groups[group_name].clients)
    {
        if (i->client_name != new_user)
            i->tcp_socket->write(data);
    }
}

void MyTcpServer::sendMsgUserLeft(const QString &group_name, const QString &user) const
{
    const QByteArray data {makeByteArray(MessageTypes::USER_LEFT, {user})};

    for (auto i : groups[group_name].clients)
    {
        if (i->client_name != user)
            i->tcp_socket->write(data);
    }
}

void MyTcpServer::sendUsersList(const QString &group_name, const QString &reciever) const
{
    QString users_str {""}; // name1,name2,...
    for (auto i : groups[group_name].clients)
    {
        users_str += i->client_name + ",";
    }

    users_str.remove(users_str.length() - 1, 1);
    const QByteArray data {makeByteArray(MessageTypes::USERS_LIST, {users_str})};
    qDebug() << groups[group_name].clients.keys();
    groups[group_name].clients[reciever]->tcp_socket->write(data);
}

void MyTcpServer::createGroup(const QString &group_name, const QString &password)
{
    groups.insert(group_name, GroupListItem(group_name, password));
    qDebug() << "Group" << group_name << "with password" << password << "created";
}

bool MyTcpServer::nicknameUsed(const QString &nickname) const
{
    for (auto i : groups)
    {
        if (i.clients.contains(nickname))
            return true;
    }

    return false;
}

bool MyTcpServer::groupExist(const QString &group_name) const
{
    return groups.contains(group_name);
}

bool MyTcpServer::checkGroupPassword(const QString &group_name, const QString &password) const
{
    return groups[group_name].password == password;
}

void MyTcpServer::removeClient(const QString &group_name, const QString &client_name)
{
    groups[group_name].clients.remove(client_name);
}

void MyTcpServer::addClient(const QString &client_name, const quintptr &handle)
{
    groups["None"].clients.insert(client_name, std::move(handles_list[handle]));
    handles_list.remove(handle);
}

void MyTcpServer::moveClient(const QString &client_name, QString &from, const QString &to)
{
    groups[to].clients.insert(client_name, std::move(groups[from].clients[client_name]));
    groups[from].clients.remove(client_name);
    qDebug() << client_name << "moved from" << from << "to" << to;
    from = to;
}

QByteArray MyTcpServer::makeByteArray(const int &msg_type, const QStringList &params)
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << quint16(0) << quint8(msg_type);

    for (auto i : params)
        data_stream << i;

    data_stream.device()->seek(0);
    data_stream << static_cast<quint16>(data.size() - sizeof(quint16));

    return data;
}

void MyTcpServer::incomingConnection(const qintptr handle)
{
    MyTcpClient *new_client = new MyTcpClient(handle, this);
    qDebug() << "Client " << handle << " connected";
    handles_list.insert(handle, new_client);
}
