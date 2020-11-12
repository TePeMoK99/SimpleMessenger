#include "tcpclient.h"

namespace Server
{
TCPClient::TCPClient(quintptr handle) : block_size {0}, handle {handle}
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    connect(socket, &QTcpSocket::readyRead, this, &TCPClient::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
}

void TCPClient::onReadyRead()
{
    QByteArray data {socket->readAll()};
    QDataStream data_stream {&data, QIODevice::ReadOnly};

    if (block_size == 0)
    {
        if (quint64(data.size()) < sizeof(quint16))
        {
            return;
        }
        data_stream >> block_size;
    }

    if (quint64(data.size()) < block_size)
    {
        return;
    }

    block_size = 0;   

    quint8 type {};
    data_stream >> type;
    switch (type)
    {
    case MessageType::REGISTER_REQUEST:
    {
        QString n_name {};
        QString password {};
        data_stream >> n_name;
        data_stream >> password;
        qDebug() << "REGISTER_REQUEST from " + n_name;
        emit registerRequest(handle, n_name, password);

        break;
    }
    case MessageType::AUTH_REQUEST:
    {
        QString n_name {};
        QString password {};
        data_stream >> n_name;
        data_stream >> password;
        qDebug() << "AUTH_REQUEST from " + n_name;
        emit authRequest(handle, n_name, password);

        break;
    }
    case MessageType::JOIN_GROUP_REQUEST:
    {
        QString group_name {};
        QString group_password {};
        data_stream >> group_name;
        data_stream >> group_password;
        qDebug() << "JOIN_GROUP_REQUEST from " + name + " to group " + group_name;
        emit joinGroupRequest(name, group_name, group_password);

        break;
    }
    case MessageType::CREATE_GROUP_REQUEST:
    {
        QString group_name {};
        QString group_password {};
        data_stream >> group_name;
        data_stream >> group_password;
        qDebug() << "CREATE_GROUP_REQUEST: [" + group_name + " : " + group_password + "]";
        emit createGroupRequest(name, group_name, group_password);

        break;
    }
    case MessageType::LEAVE_GROUP_REQUEST:
    {
        emit leftGroupRequest(name, current_group);

        break;
    }
    case MessageType::USERS_LIST_REQUEST:
    {
        qDebug() << "USERS_LIST_REQUEST from " + name + " in group: " + current_group;
        emit usersListRequest(name, current_group);

        break;
    }
    case MessageType::PUBLIC_MESSAGE:
    {
        QString msg {};
        data_stream >> msg;
        qDebug() << "PUBLIC_MESSAGE: [" + msg + "] from " + name;
        emit publicMessage(name, current_group, msg);
        break;
    }
    case MessageType::PRIVATE_MESSAGE:
    {
        QString reciever {};
        QString msg {};
        qDebug() << "PUBLIC_MESSAGE: [" + msg + "] from " + name + " to " + reciever;
        emit privateMessage(name, reciever, current_group, msg);

        break;
    }
    }
    emit socket->readyRead();
}

void TCPClient::onDisconnected()
{
    emit leftGroupRequest(name, current_group);
    deleteLater();
}
}
