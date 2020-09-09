#include "mytcpserver.h"

MyTCPServer::MyTCPServer()
{

}

bool MyTCPServer::startServer()
{
    if (this->listen(QHostAddress::Any, 55555))
    {
        qDebug() << "Server started!";
        return true;
    }

    qDebug() << "Server start failed.";
    return false;
}

void MyTCPServer::incomingConnection(qintptr handle)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);

    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

    qDebug() << handle << "client connected.";
    socket->write("You are connected to the server.");
}

void MyTCPServer::socketReady()
{

}

void MyTCPServer::socketDisconnected()
{
    qDebug() << "Disc";
    socket->deleteLater();
}
