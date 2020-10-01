#include "mytcpclient.h"

#include <QDataStream>
#include <windows.h>

#include "types.h"

MyTcpClient::MyTcpClient() : client_name {"Default"}, block_size {0}
{
    tcp_socket = new QTcpSocket(this);
    connect(tcp_socket, &QTcpSocket::readyRead,    this, &MyTcpClient::socketReadyRead);
    connect(tcp_socket, &QTcpSocket::connected,    this, &MyTcpClient::socketConnected);
    connect(tcp_socket, &QTcpSocket::disconnected, this, &MyTcpClient::socketDisconnected);
}

MyTcpClient* MyTcpClient::instance()
{
    static MyTcpClient inst;
    return &inst;
}

void MyTcpClient::sendPublicMessage(const QString &message_text)
{
    tcp_socket->write(makeByteArray(MessageTypes::PUBLIC_MESSAGE, {message_text}));
}

void MyTcpClient::sendPrivateMessage(const QString &reciever, const QString &message_text)
{
    tcp_socket->write(makeByteArray(MessageTypes::PRIVATE_MESSAGE, {reciever, message_text}));
}

QByteArray MyTcpClient::makeByteArray(const int &msg_type, const QStringList &params)
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

void MyTcpClient::joinChat(const QString &host, const int &port, const QString &nickname)
{
    tcp_socket->connectToHost(host, port);
    tcp_socket->write(makeByteArray(MessageTypes::AUTH_REQUEST, {nickname}));
}

void MyTcpClient::leftChat()
{
    socketDisconnected();
    emit authFail();
}

void MyTcpClient::joinGroup(const QString &group, const QString &password)
{
    tcp_socket->write(makeByteArray(MessageTypes::JOIN_GROUP_REQUEST, {group, password}));
}

void MyTcpClient::leftGroup()
{
    tcp_socket->write(makeByteArray(MessageTypes::LEFT_GROUP, {}));
}

void MyTcpClient::createGroup(const QString &group, const QString &password)
{
    tcp_socket->write(makeByteArray(MessageTypes::CREATE_GROUP_REQUEST, {group, password}));
}

void MyTcpClient::socketReadyRead()
{
    QDataStream data_stream {tcp_socket};

    if (block_size == 0)
    {
        if (tcp_socket->bytesAvailable() < static_cast<qint64>(sizeof(quint16)))        
            return;

        data_stream >> block_size;
    }

    if (tcp_socket->bytesAvailable() < block_size)
        return;

    block_size = 0;

    quint8 type {};
    data_stream >> type;

    switch (type)
    {
    case MessageTypes::AUTH_SUCCESS:
    {
        qDebug() << "C_Case AUTH_SUCCESS";
        data_stream >> client_name;
        emit authSuccess(client_name);

        break;
    }
    case MessageTypes::AUTH_FAIL:
    {
        qDebug() << "C_Case AUTH_FAIL";
        emit authFail();

        break;
    }
    case MessageTypes::JOIN_GROUP_SUCCESS:
    {
        qDebug() << "C_Case JOIN_GROUP_SUCCESS";
        data_stream >> group_name;
        emit joinGroupSuccess(group_name);
        tcp_socket->write(makeByteArray(MessageTypes::USERS_LIST_REQUEST, {client_name}));

        break;
    }
    case MessageTypes::JOIN_GROUP_FAIL:
    {
        qDebug() << "C_Case JOIN_GROUP_FAIL";
        QString error {};
        data_stream >> error;
        emit joinGroupFail(error);

        break;
    }
    case MessageTypes::CREATE_GROUP_SUCCESS:
    {
        qDebug() << "C_Case CREATE_GROUP_SUCCESS";
        data_stream >> group_name;
        emit joinGroupSuccess(group_name);
        tcp_socket->write(makeByteArray(MessageTypes::USERS_LIST_REQUEST, {client_name}));

        break;
    }
    case MessageTypes::CREATE_GROUP_FAIL:
    {
        qDebug() << "C_Case CREATE_GROUP_FAIL";
        QString error {};
        data_stream >> error;
        emit joinGroupFail(error);

        break;
    }
    case MessageTypes::USER_JOIN:
    {
        QString name {};
        data_stream >> name;
        qDebug() << "C_Case USER_JOIN" << name;
        emit userJoinRecieved(name);

        break;
    }
    case MessageTypes::USER_LEFT:
    {
        QString name {};
        data_stream >> name;
        qDebug() << "C_Case USER_LEFT"  << name;
        emit userLeftRecieved(name);

        break;
    }
    case MessageTypes::USERS_LIST:
    {
        qDebug() << "C_Case USERS_LIST";
        QString users_list_str {};
        data_stream >> users_list_str;
        emit usersListRecieved(users_list_str.split(","));

        break;
    }
    case MessageTypes::PUBLIC_MESSAGE:
    {
        qDebug() << "C_Case PUBLIC_MESSAGE";
        QString message {};
        QString sender {};
        data_stream >> sender;
        data_stream >> message;
        emit publicMessageRecieved(sender, message);

        break;
    }
    case MessageTypes::PRIVATE_MESSAGE:
    {
        qDebug() << "C_Case PRIVATE_MESSAGE";
        QString message {};
        QString sender {};
        data_stream >> sender;
        data_stream >> message;
        emit privateMessageRecieved(sender, message);

        break;
    }
    }
}

void MyTcpClient::socketConnected()
{

}

void MyTcpClient::socketDisconnected()
{
    tcp_socket->write(makeByteArray(MessageTypes::USER_LEFT, {client_name}));
    client_name = "";
    tcp_socket->disconnectFromHost();
}
