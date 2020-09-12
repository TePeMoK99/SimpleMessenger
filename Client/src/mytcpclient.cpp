#include "mytcpclient.h"

MyTcpClient::MyTcpClient()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

void MyTcpClient::connectToServer()
{
    socket->connectToHost("127.0.0.1", 55555);
}

void MyTcpClient::socketReady()
{
    if (socket->waitForConnected(500))
    {
        socket->waitForReadyRead(500);
        data = socket->readAll();

        qDebug() << data;
    }
}

void MyTcpClient::socketDisconnected()
{
    socket->deleteLater();        
}
