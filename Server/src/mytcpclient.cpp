#include "mytcpclient.h"
#include "types.h"
#include <QDataStream>

MyTcpClient::MyTcpClient(const quintptr handle, MyTcpServer *server, QObject *parent) :
    QObject {parent}, handle {handle}, tcp_server {server}, block_size {0}, nickname {}, isAuth {false}
{
    tcp_socket = new QTcpSocket(this);
    tcp_socket->setSocketDescriptor(handle);

    connect(tcp_socket, &QTcpSocket::connected, this, &MyTcpClient::socketConnected);
    connect(tcp_socket, &QTcpSocket::disconnected, this, &MyTcpClient::socketDisconnected);
    connect(tcp_socket, &QTcpSocket::readyRead, this, &MyTcpClient::socketReadyRead);
}

void MyTcpClient::socketConnected()
{
    qDebug() << "Socket " << handle << " connected";
}

void MyTcpClient::socketDisconnected()
{
    // TODO: передать серверу, что клиент отключился
    tcp_server->removeSocketFromList(nickname);
    isAuth = false;
    qDebug() << "Socket " << handle << " disconnected";
    deleteLater();
}

void MyTcpClient::socketReadyRead()
{
    QDataStream data_stream {tcp_socket};

    if (block_size == 0)
    {
        if (tcp_socket->bytesAvailable() < static_cast<qint64>(sizeof(quint16)))
        {
            return;
        }
        data_stream >> block_size;
    }

    if (tcp_socket->bytesAvailable() < block_size)
    {
        return;
    }
    else
        block_size = 0;

    quint8 type {};
    data_stream >> type;

    switch (type)
    {
    case MessageTypes::AUTH_REQUEST:
    {
        QString name {};
        data_stream >> name;

        qDebug() << "S_Case AUTH_REQUEST " << name;

        if (!tcp_server->isNicknameUsed(name))
        {
            nickname = name;
            isAuth = true;
        }

        tcp_server->sendMessageUserJoin(nickname);

        break;
    }
    case MessageTypes::USERS_LIST:
    {
        qDebug() << "S_Case USERS_LIST";
        tcp_server->sendUsersList();

        break;
    }
    case MessageTypes::PUBLIC_MESSAGE:
    {
        qDebug() << "S_Case PUBLIC_MESSAGE";
        QString message {};
        data_stream >> message;

        tcp_server->sendMessageToAll(message, nickname);

        break;
    }
    case MessageTypes::PRIVATE_MESSAGE:
    {
        qDebug() << "S_Case PRIVATE_MESSAGE";
        QString message {};
        QString reciever {};
        data_stream >> reciever;
        data_stream >> message;

        tcp_server->sendMessageToUser(message, reciever, nickname);

        break;
    }
    case MessageTypes::USER_JOIN:
    {
        qDebug() << "S_Case USER_JOIN";
        QString name {};
        data_stream >> name;

        tcp_server->sendMessageUserJoin(nickname);

        break;
    }
    case MessageTypes::USER_LEFT:
    {
        qDebug() << "S_Case USER_LEFT";
        QString name {};
        data_stream >> name;
        // TODO: передать всем остальным, что пользователь вышел

        break;
    }
    }
}
