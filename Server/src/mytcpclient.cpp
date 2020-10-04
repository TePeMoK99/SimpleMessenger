﻿#include "mytcpclient.h"
#include "types.h"
#include <windows.h>
#include <QDataStream>

MyTcpClient::MyTcpClient(const quintptr handle, MyTcpServer *server, QObject *parent) :
    QObject {parent}, handle {handle}, tcp_server {server}, block_size {0}, client_name {}
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
    tcp_server->removeClient(current_group, client_name);
    qDebug() << "Socket " << handle << " disconnected";
    deleteLater();
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
    case MessageTypes::AUTH_REQUEST:
    {
        QString name {};
        data_stream >> name;

        qDebug() << "S_Case AUTH_REQUEST " << name;

        if (!tcp_server->nicknameUsed(name))
        {
            client_name = name;
            current_group = "None";
            tcp_server->addClient(client_name, handle);
            tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::AUTH_SUCCESS, {client_name}));
        }
        else
        {
            tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::AUTH_FAIL, {"Auth error"}));
        }

        break;
    }
    case MessageTypes::JOIN_GROUP_REQUEST:
    {
        qDebug() << "S_Case JOIN_GROUP_REQUEST";
        QString new_group {};
        QString password {};
        data_stream >> new_group;
        data_stream >> password;

        if (!tcp_server->groupExist(new_group))
        {
            tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::JOIN_GROUP_FAIL, {"Group doesn't exist"}));
        }
        else if (!tcp_server->checkGroupPassword(new_group, password))
        {
            tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::JOIN_GROUP_FAIL, {"Wrong password"}));
        }
        else
        {
            tcp_server->moveClient(client_name, current_group, new_group);
//            current_group = new_group;
            tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::JOIN_GROUP_SUCCESS, {current_group}));
            tcp_server->sendMsgUserJoin(current_group, client_name);
        }

        break;
    }
    case MessageTypes::CREATE_GROUP_REQUEST:
    {
        qDebug() << "S_Case CREATE_GROUP_REQUEST";
        QString group {};
        QString password {};
        data_stream >> group;
        data_stream >> password;

        if (tcp_server->groupExist(group))
        {
            tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::CREATE_GROUP_FAIL, {"Group name already used"}));
            break;
        }

        tcp_server->createGroup(group, password);
        tcp_server->moveClient(client_name, current_group, group);
//        current_group = group;
        tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::CREATE_GROUP_SUCCESS, {group}));

        break;
    }
    case MessageTypes::LEAVE_GROUP:
    {
        qDebug() << "S_Case LEFT_GROUP";
        tcp_server->sendMsgUserLeft(current_group, client_name);
        tcp_socket->write(MyTcpServer::makeByteArray(MessageTypes::JOIN_GROUP_FAIL, {"Left group"}));
        tcp_server->moveClient(client_name, current_group, "None");
//        current_group = "None";

        break;
    }
    case MessageTypes::PUBLIC_MESSAGE:
    {
        qDebug() << "S_Case PUBLIC_MESSAGE";
        QString message {};
        data_stream >> message;

        tcp_server->sendMsgToAll(current_group, message, client_name);

        break;
    }
    case MessageTypes::PRIVATE_MESSAGE:
    {
        qDebug() << "S_Case PRIVATE_MESSAGE";
        QString message {};
        QString reciever {};
        data_stream >> reciever;
        data_stream >> message;

        tcp_server->sendMsgToUser(current_group, message, reciever, client_name);

        break;
    }
    case MessageTypes::USER_JOIN:
    {
        qDebug() << "S_Case USER_JOIN";
        QString name {};
        data_stream >> name;
        tcp_server->sendMsgUserJoin(current_group, name);

        break;
    }
    case MessageTypes::USER_LEFT:
    {
        qDebug() << "S_Case USER_LEFT";
        QString sender {};
        data_stream >> sender;
        tcp_server->sendMsgUserLeft(current_group, sender);

        break;
    }
    case MessageTypes::USERS_LIST_REQUEST:
    {
        QString reciever {};
        data_stream >> reciever;
        qDebug() << "S_Case USERS_LIST_REQUEST";
        tcp_server->sendUsersList(current_group, reciever);

        break;
    }
    }
}
