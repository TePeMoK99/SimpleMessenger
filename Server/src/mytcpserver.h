#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "mytcpclient.h"
#include "grouplistitem.h"

class MyTcpClient;

class MyTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    MyTcpServer();

    bool start(const QHostAddress &address, const int &port);
    bool start(const QString &address_str, const int &port);

    void sendMessageToUser(const QString &group_name, const QString &message, const QString &reciever, const QString &sender) const;
    void sendMessageToAll(const QString &group_name, const QString &message, const QString &sender) const;

    void sendMessageUserJoin(const QString &group_name, const QString &new_user) const;
    void sendMessageUserLeft(const QString &group_name, const QString &user) const;

    void sendAuthSuccess(const QString &reciever) const;
    void sendAuthFail(const QString &reciever) const;
    void sendUsersList(const QString &group_name, const QString &reciever) const;

    void createGroup(const QString &group_name, const QString &password);
    bool nicknameUsed(const QString &nickname) const;
    bool groupExist(const QString &group_name) const;
    bool checkGroupPassword(const QString &group_name, const QString &password) const;
    void removeSocketFromList(const QString &client_name);

    static QByteArray makeByteArray(const int &msg_type, const QStringList &params);

private slots:
    void incomingConnection(const qintptr handle) override;

private:
    QTcpSocket *socket;
    QList<MyTcpClient*> clients_list;
    QList<GroupListItem> groups_list;
};
