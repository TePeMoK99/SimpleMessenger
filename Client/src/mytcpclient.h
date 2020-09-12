#pragma once

#include <QTcpSocket>
#include <QDebug>

class MyTcpClient : public QObject
{
    Q_OBJECT

public:
    MyTcpClient();

public slots:
    void connectToServer();

private slots:
    void socketReady();         // Call when emit readyRead()
    void socketDisconnected();  // Call when emit disconnected()

private:
    QTcpSocket *socket;
    QByteArray data;
};
