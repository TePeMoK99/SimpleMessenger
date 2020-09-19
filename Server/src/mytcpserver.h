#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "mytcpclient.h"

class MyTcpClient;

class MyTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    MyTcpServer();

    bool start(const QHostAddress &address, const int &port);
    bool start(const QString &address_str, const int &port);
    void sendMessageToUser(const QString &message, const QString &reciever, const QString &sender) const;
    void sendMessageToAll(const QString &message, const QString &sender) const;
    void sendMessageUserJoin(const QString &new_user) const;
    void sendMessageUserLeft(const QString &user) const;
    void sendAuthSuccess(const QString &reciever) const;
    void sendAuthFail(const QString &reciever) const;
    void sendUsersList(const QString &reciever) const;
    bool isNicknameUsed(const QString &nickname) const;
    void removeSocketFromList(const QString &client_name);

    static QByteArray makeByteArray(const int &msg_type, const QStringList &params);

private slots:
    void incomingConnection(const qintptr handle) override;

private:
    QTcpSocket *socket;
    QList<MyTcpClient*> clients_list;
};
