#include "mytcpserver.h"
#include <QDataStream>
#include "types.h"

MyTcpServer::MyTcpServer()
{

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

void MyTcpServer::sendMessageToAll(const QString &message, const QString &sender) const
{
//    QByteArray data;
//    QDataStream data_stream {&data, QIODevice::WriteOnly};

//    data_stream << quint16(0) << quint8(MessageTypes::PUBLIC_MESSAGE) << sender << message;
//    data_stream.device()->seek(0);
//    data_stream << quint16(data.size() - sizeof(quint16));
    const int list_size {clients_list.length()};
    const QByteArray data {makeByteArray(MessageTypes::PUBLIC_MESSAGE, {sender, message})};

    for (int i {0}; i < list_size; i++)
    {
        clients_list[i]->tcp_socket->write(data);
        qDebug() << "Message [" << message << "] from " << sender << "sent to all";
    }
}

void MyTcpServer::sendMessageToUser(const QString &message, const QString &reciever, const QString &sender) const
{
//    QByteArray data;
//    QDataStream data_stream {&data, QIODevice::WriteOnly};

//    data_stream << quint16(0) << quint8(MessageTypes::PRIVATE_MESSAGE) << sender << message;
//    data_stream.device()->seek(0);
//    data_stream << quint16(data.size() - sizeof(quint16));

    const int list_size {clients_list.length()};
    const QByteArray data {makeByteArray(MessageTypes::PRIVATE_MESSAGE, {sender, message})};

    for (int i {0}; i < list_size; i++)
    {
        if (clients_list[i]->nickname == reciever)
        {
            clients_list[i]->tcp_socket->write(data);
            qDebug() << "Message [" << message << "] from " << sender << "sent to " << reciever;
        }
    }
}

void MyTcpServer::sendMessageUserJoin(const QString &new_user) const
{
//    QByteArray data;
//    QDataStream data_stream {&data, QIODevice::WriteOnly};

//    data_stream << quint16(0) << quint8(MessageTypes::USER_JOIN) <<  message;
//    data_stream.device() ->seek(0);
//    data_stream << quint16(data.size() - sizeof(quint16));
    const int list_size {clients_list.length()};
    const QByteArray data {makeByteArray(MessageTypes::USER_JOIN, {new_user})};

    for (int i {0}; i < list_size; i++)
    {
        clients_list[i]->tcp_socket->write(data);
    }
}

void MyTcpServer::sendUsersList() const
{
    QString users_str {""};
    for (auto i : clients_list)
    {
        users_str += i->nickname + ",";
    }
    users_str.remove(users_str.length() - 1, 1);

    const int list_size {clients_list.length()};
//    QByteArray data;
//    QDataStream data_stream {&data, QIODevice::WriteOnly};

//    data_stream << quint16(0) << quint8(MessageTypes::USERS_LIST) <<  users_str;
//    data_stream.device()->seek(0);
//    data_stream << quint16(data.size() - sizeof(quint16));

    const QByteArray data {makeByteArray(MessageTypes::USERS_LIST, {users_str})};

    for (int i {0}; i < list_size; i++)
    {
        clients_list[i]->tcp_socket->write(data);
    }
}

bool MyTcpServer::isNicknameUsed(const QString &nickname) const
{
    for (auto i : clients_list)
    {
        if (i->nickname == nickname)
            return true;
    }

    return false;
}

void MyTcpServer::removeSocketFromList(const QString &client_name)
{
    const int size {clients_list.size()};
    int index {};

    for (int i = 0; i < size; i++)
    {
        if (clients_list[i]->nickname == client_name)
        {
            index = i;
            break;
        }
    }

    clients_list.removeAt(index);
}

QByteArray MyTcpServer::makeByteArray(const int &msg_type, const QStringList &params)
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << quint16(0) << quint8(msg_type);

    for (auto i : params)
    {
        data_stream << i;
    }

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
