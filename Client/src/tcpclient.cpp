#include "tcpclient.h"

#include <QDataStream>
#include <windows.h>

#include "types.h"

TCPClient::TCPClient() : name {"Default"}, block_size {0}
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,    this, &TCPClient::onReayRead);
    connect(socket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
}

TCPClient* TCPClient::instance()
{
    static TCPClient inst;
    return &inst;
}

void TCPClient::sendPublicMsg(const QString &message_text)
{
    socket->write(makeByteArray(MessageType::PUBLIC_MESSAGE, {message_text}));
}

void TCPClient::sendPrivateMsg(const QString &reciever, const QString &message_text)
{
    socket->write(makeByteArray(MessageType::PRIVATE_MESSAGE, {reciever, message_text}));
}

void TCPClient::singUp(const QHostAddress &host, const int &port, const QString &nickname, const QString &password)
{
    socket->connectToHost(host, port);
    QByteArray data {makeByteArray(MessageType::REGISTER_REQUEST, {nickname, password})};
    qDebug() << data;
    socket->write(data);
    qDebug() << "Registration request sent";
}

void TCPClient::signIn(const QHostAddress &host, const int &port, const QString &nickname, const QString &password)
{
    socket->connectToHost(host, port);
    socket->write(makeByteArray(MessageType::AUTH_REQUEST, {nickname, password}));
    qDebug() << "Auth request sent";
}

void TCPClient::signOut()
{
    socket->disconnectFromHost();
}

QByteArray TCPClient::makeByteArray(const quint8 &msg_type, const QStringList &params)
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << quint16(0) << msg_type;

    for (const auto &i : params)
    {
        data_stream << i;
    }

    data_stream.device()->seek(0);
    data_stream << static_cast<quint16>(data.size() - sizeof(quint16));

    return data;
}

QByteArray TCPClient::makeByteArray(const quint8 &msg_type, const QString &param)
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << quint16(0) << msg_type;
    data_stream << param;

    data_stream.device()->seek(0);
    data_stream << static_cast<quint16>(data.size() - sizeof(quint16));

    return data;
}

void TCPClient::joinGroup(const QString &group, const QString &password)
{
    socket->write(makeByteArray(MessageType::JOIN_GROUP_REQUEST, {group, password}));
}

void TCPClient::leaveGroup()
{
    socket->write(makeByteArray(MessageType::LEAVE_GROUP_REQUEST));
}

void TCPClient::createGroup(const QString &group, const QString &password)
{
    socket->write(makeByteArray(MessageType::CREATE_GROUP_REQUEST, {group, password}));
}

void TCPClient::onReayRead()
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
    case MessageType::AUTH_SUCCESS:
    {
        qDebug() << "AUTH_SUCCESS";
        data_stream >> name;
        emit authSuccess(name);

        break;
    }
    case MessageType::AUTH_FAIL:
    {
        qDebug() << "AUTH_FAIL";
        QString error {};
        data_stream >> error;
        emit authFail(error);

        break;
    }
    case MessageType::REGISTER_SUCCESS:
    {
        qDebug() << "REGISTER_SUCCESS";
        data_stream >> name;
        emit registerSuccess(name);

        break;
    }
    case MessageType::REGISTER_FAIL:
    {
        qDebug() << "REGISTER_FAIL";
        QString error {};
        data_stream >> error;
        emit registerFail(error);

        break;
    }
    case MessageType::JOIN_GROUP_SUCCESS:
    {
        qDebug() << "JOIN_GROIP_SUCCESS";
        data_stream >> current_group;
        emit joinGroupSuccess(current_group);
        socket->write(makeByteArray(MessageType::USERS_LIST_REQUEST));

        break;
    }
    case MessageType::JOIN_GROUP_FAIL:
    {
        qDebug() << "JOIN_GROUP_FAIL";
        QString error {};
        data_stream >> error;
        emit joinGroupFail(error);

        break;
    }
    case MessageType::USERS_LIST:
    {
        qDebug() << "USERS_LIST";
        QString users_str {};
        data_stream >> users_str;
        QStringList users_list = users_str.split("|");
        emit usersListRecieved(users_list);

        break;
    }
    case MessageType::PUBLIC_MESSAGE:
    {
        qDebug() << "PUBLIC_MESSAGE";
        QString sender {};
        QString message {};
        data_stream >> sender;
        data_stream >> message;
        emit publicMsgRecieved(sender, message);

        break;
    }
    case MessageType::PRIVATE_MESSAGE:
    {
        qDebug() << "PRIVATE_MESSAGE";
        QString sender {};
        QString reciever {};
        QString message {};
        data_stream >> sender;
        data_stream >> reciever;
        data_stream >> message;
        emit privateMsgRecieved(sender, reciever, message);

        break;
    }
    case MessageType::USER_JOIN:
    {
        qDebug() << "USER_JOIN";
        QString new_user {};
        data_stream >> new_user;
        emit userJoinRecieved(new_user);

        break;
    }
    case MessageType::USER_LEFT:
    {
        qDebug() << "USER_LEFT";
        QString leaved_user {};
        data_stream >> leaved_user;
        emit userLeftRecieved(leaved_user);

        break;
    }
    case MessageType::LEAVE_GROUP_SUCCESS:
    {
        qDebug() << "LEAVE_GROUP_SUCCESS";
        emit leaveGroupSuccess();

        break;
    }
    }
    if (socket->bytesAvailable() > 0)
    {
        emit socket->readyRead();
    }
}

void TCPClient::onDisconnected()
{
    socket->write(makeByteArray(MessageType::USER_LEFT, {name}));
    name = current_group = "";
    socket->disconnectFromHost();
}
