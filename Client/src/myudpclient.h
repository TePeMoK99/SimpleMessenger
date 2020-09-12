#pragma once

#include <QString>
#include <QUdpSocket>
#include <utility>

#include "messageitem.h"

class MyUdpClient : public QObject
{
    Q_OBJECT

public:
    MyUdpClient();
    MyUdpClient(const QString &nickname, const int &port);

    enum TypeOfMessage
    {
        USUAL_MESSAGE,
        PERSON_ONLINE,
        WHO_IS_ONLINE
    };

    QByteArray prepareByteArray(const QString &message, const qint8 &type_of_message) const;

signals:
    void messageRecieved(const MessageItem message);

public slots:
    bool sendMessage(const QString &message, const qint8 &type_of_message);

private slots:
    void readMessage();

private:
    QUdpSocket *udp_socket;
    QString nickname;
    QMap<QString, QString> users_list;
    int port;
};
