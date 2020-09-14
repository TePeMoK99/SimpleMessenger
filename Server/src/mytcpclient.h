#pragma once

#include <QTcpSocket>

#include "mytcpserver.h"

class MyTcpServer;

class MyTcpClient : public QObject
{
    Q_OBJECT
    friend class MyTcpServer;

public:
    MyTcpClient(const quintptr handle, MyTcpServer *server, QObject *parent = nullptr);

private slots:
    void socketReadyRead();
    void socketConnected();
    void socketDisconnected();

private:
    quintptr handle;
    MyTcpServer *tcp_server;
    quint16 block_size;
    QString nickname;
    QTcpSocket *tcp_socket;
    bool isAuth;
};
