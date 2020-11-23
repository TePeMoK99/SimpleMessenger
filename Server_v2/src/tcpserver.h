#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QHash>
#include <QCryptographicHash>
#include <QDebug>

#include "tcpclient.h"
#include "group.h"
#include "CRUD/processor.h"

namespace Server
{
class TCPServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);

    virtual void incomingConnection(qintptr handle) override;
    bool start();
    void stop();

private slots:
    void onRegisterRequest(quintptr handle, QString name, QString password);
    void onAuthRequest(quintptr handle, QString name, QString password);
    void onClientDisconnected(QString name);

    void onCreateGroupRequest(QString client_name, QString group_name, QString group_password);
    void onJoinGroupRequest(QString client_name, QString group_name, QString group_password);
    void onLeaveGroupRequest(QString client_name, QString group_name);

    void onPublicMessage(QString sender, QString group_name, QString msg);
    void onPrivateMessage(QString sender, QString receiver, QString group_name, QString msg);

    void onUsersListRequest(QString client_name, QString group_name);

private:
    static QByteArray makeByteArray(const quint8 &msg_type, const QStringList &params);
    static QByteArray makeByteArray(const quint8 &msg_type, const QString &param = {});

    QHash<quintptr, TCPClient*> not_auth_clients;
    QHash<QString, Group> groups;
    CRUD::Processor *crud_processor;

};
}
