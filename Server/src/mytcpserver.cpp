#include "mytcpserver.h"
#include <QDataStream>
#include "types.h"

MyTcpServer::MyTcpServer()
{
    groups_list.push_back(GroupListItem("Main", "0000"));
    groups_list.push_back(GroupListItem("Main1", "1234"));
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

void MyTcpServer::sendMessageToAll(const QString &group_name, const QString &message, const QString &sender) const
{
    const QByteArray data {makeByteArray(MessageTypes::PUBLIC_MESSAGE, {sender, message})};

    for (auto i : clients_list)
    {
        if (i->client_name != sender && i->group_name == group_name)
            i->tcp_socket->write(data);
    }
    qDebug() << "Message [" << message << "] from " << sender << "sent to all";
}

void MyTcpServer::sendMessageToUser(const QString &group_name, const QString &message, const QString &reciever, const QString &sender) const
{
    const QByteArray data {makeByteArray(MessageTypes::PRIVATE_MESSAGE, {sender, message})};

    for (auto i : clients_list)
    {
        if (i->client_name == reciever && i->group_name == group_name)
        {
            i->tcp_socket->write(data);
            break;
        }
    }
    qDebug() << "Message [" << message << "] from " << sender << "sent to " << reciever;
}

void MyTcpServer::sendMessageUserJoin(const QString &group_name, const QString &new_user) const
{
    const QByteArray data {makeByteArray(MessageTypes::USER_JOIN, {new_user})};

    for (auto i : clients_list)
    {
        if (i->client_name != new_user && i->group_name == group_name)
            i->tcp_socket->write(data);
    }
}

void MyTcpServer::sendMessageUserLeft(const QString &group_name, const QString &user) const
{
    const QByteArray data {makeByteArray(MessageTypes::USER_LEFT, {user})};

    for (auto i : clients_list)
    {
        if (i->client_name != user && i->group_name == group_name)
            i->tcp_socket->write(data);
    }
}

void MyTcpServer::sendUsersList(const QString &group_name, const QString &reciever) const
{
    QString users_str {""};
    for (auto i : clients_list)
    {
        if (i->group_name == group_name)
            users_str += i->client_name + ",";
    }

    users_str.remove(users_str.length() - 1, 1);

    const QByteArray data {makeByteArray(MessageTypes::USERS_LIST, {users_str})};

    for (auto i : clients_list)       
    {
        if (i->client_name == reciever && i->group_name == group_name)
        {
            i->tcp_socket->write(data);
            break;
        }
    }
}

void MyTcpServer::createGroup(const QString &group_name, const QString &password)
{
    groups_list.append(GroupListItem(group_name, password));
    qDebug() << "Group" << group_name << "with password" << password << "created";
}

void MyTcpServer::sendAuthSuccess(const QString &reciever) const
{
    const QByteArray data {makeByteArray(MessageTypes::AUTH_SUCCESS, {reciever})};

    for (auto i : clients_list)
    {
        if (i->client_name == reciever)
        {
            i->tcp_socket->write(data);
            break;
        }
    }
}

void MyTcpServer::sendAuthFail(const QString &reciever) const
{
    const QByteArray data {makeByteArray(MessageTypes::AUTH_FAIL, {})};

    for (auto i : clients_list)
    {
        if (i->client_name == reciever)        
            i->tcp_socket->write(data);        
    }
}

bool MyTcpServer::nicknameUsed(const QString &nickname) const
{
    for (auto i : clients_list)
    {
        if (i->client_name == nickname)        
            return true;        
    }

    return false;
}

bool MyTcpServer::groupExist(const QString &group_name) const
{
    for (auto i : groups_list)
    {
        if (i.group_name == group_name)
            return true;
    }

    return false;
}

bool MyTcpServer::checkGroupPassword(const QString &group_name, const QString &password) const
{
    for (auto i : groups_list)
    {
        if (i.group_name == group_name)
        {
            if (i.password == password)
                return true;
            return false;
        }
    }

    return false;
}

void MyTcpServer::removeSocketFromList(const QString &client_name)
{
    const int size {clients_list.size()};
    int index {};

    for (int i = 0; i < size; i++)
    {
        if (clients_list[i]->client_name == client_name)
        {
            index = i;
            clients_list.removeAt(index);
            break;
        }
    }
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
    clients_list.append(new_client);
    qDebug() << clients_list;
}
