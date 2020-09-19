#include "mytcpclient.h"
#include "types.h"
#include <windows.h>
#include <QDataStream>

MyTcpClient::MyTcpClient(const quintptr handle, MyTcpServer *server, QObject *parent) :
    QObject {parent}, handle {handle}, tcp_server {server}, block_size {0}, client_name {}, isAuth {false}
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
    tcp_server->removeSocketFromList(client_name);
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
            client_name = name;
            isAuth = true;
            tcp_server->sendAuthSuccess(client_name);
            tcp_server->sendMessageUserJoin(client_name);
        }
        else
        {
            tcp_server->sendAuthFail(client_name);
        }

        break;
    }
    case MessageTypes::PUBLIC_MESSAGE:
    {
        qDebug() << "S_Case PUBLIC_MESSAGE";
        QString message {};
        data_stream >> message;

        tcp_server->sendMessageToAll(message, client_name);

        break;
    }
    case MessageTypes::PRIVATE_MESSAGE:
    {
        qDebug() << "S_Case PRIVATE_MESSAGE";
        QString message {};
        QString reciever {};
        data_stream >> reciever;
        data_stream >> message;

        tcp_server->sendMessageToUser(message, reciever, client_name);

        break;
    }
    case MessageTypes::USER_JOIN:
    {
        qDebug() << "S_Case USER_JOIN";
        QString name {};
        data_stream >> name;

        tcp_server->sendMessageUserJoin(name);

        break;
    }
    case MessageTypes::USER_LEFT:
    {
        qDebug() << "S_Case USER_LEFT";
        QString name {};
        data_stream >> name;
        // TODO: передать всем остальным, что пользователь вышел
        tcp_server->sendMessageUserLeft(name);

        break;
    }
    case MessageTypes::USERS_LIST_REQUEST:
    {
        QString reciever {};
        data_stream >> reciever;
        qDebug() << "S_Case USERS_LIST_REQUEST";
        tcp_server->sendUsersList(reciever);

        break;
    }
    }
}
