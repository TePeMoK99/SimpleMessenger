#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MyTCPServer : public QTcpServer
{
    Q_OBJECT

public:
    MyTCPServer();

public slots:
    bool startServer(); // Method for starting server

private slots:
    void incomingConnection(qintptr handle) override;   // Call when new connection incoming
    void socketReady(); // Call when emit readyRead()
    void socketDisconnected();  // Call when emit disconnected()

private:
    QTcpSocket *socket;         // socket
    QByteArray data;            // ByteArray for data
};
