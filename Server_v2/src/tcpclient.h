#pragma once

#include <QTcpSocket>
#include <QSslSocket>
#include <QObject>
#include <QSslKey>

#include "types.h"
#include "tcpserver.h"

namespace Server
{
class TCPClient : public QObject
{
    Q_OBJECT
    friend class TCPServer;

public:
    TCPClient(quintptr handle);

private slots:
    void onReadyRead();
    void onDisconnected();

signals:
    void authRequest(quintptr handle, QString name, QString password);
    void registerRequest(quintptr handle, QString name, QString password);
    void clientDisconnected(QString name);

    void createGroupRequest(QString client_name, QString group_name, QString group_password);
    void joinGroupRequest(QString client_name, QString group_name, QString group_password);
    void leaveGroupRequest(QString client_name, QString group_name);

    void publicMessage(QString sender, QString group_name, QString msg);
    void privateMessage(QString sender, QString receiver, QString group_name, QString msg);

    void usersListRequest(QString name, QString group);

private:
    QSslSocket  *socket;
    quint16     block_size;
    quintptr    handle;
    QString     name;
    QString     current_group;
};
}
