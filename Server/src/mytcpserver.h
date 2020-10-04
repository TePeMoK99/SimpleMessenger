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
    ~MyTcpServer()
    {
        qDebug() << "removed";
    }

    bool start(const QHostAddress &address, const int &port);
    bool start(const QString &address_str, const int &port);

    // Methods for sending messages:
    void sendMsgToUser(const QString &group_name, const QString &message,
                       const QString &reciever, const QString &sender) const;
    void sendMsgToAll(const QString &group_name, const QString &message, const QString &sender) const;
    void sendMsgUserJoin(const QString &group_name, const QString &new_user) const;
    void sendMsgUserLeft(const QString &group_name, const QString &user) const;

    void sendUsersList(const QString &group_name, const QString &reciever) const;

    void createGroup(const QString &group_name, const QString &password);
    void addClient(const QString &client_name, const quintptr &handle);
    void moveClient(const QString &client_name, QString &from, const QString &to);
    void removeClient(const QString &group_name, const QString &client_name);

    bool nicknameUsed(const QString &nickname) const;
    bool groupExist(const QString &group_name) const;
    bool checkGroupPassword(const QString &group_name, const QString &password) const;


    static QByteArray makeByteArray(const int &msg_type, const QStringList &params);

private slots:
    void incomingConnection(const qintptr handle) override;

private:
    QTcpSocket *socket;
    QHash<QString, GroupListItem> groups;
    QHash<quintptr, MyTcpClient*> handles_list;
};
