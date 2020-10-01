#pragma once

#include <QTcpSocket>
#include <QDebug>
#include <vector>

class MyTcpClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MyTcpClient)
    friend class UserListModel;
    friend class ChatModel;

public:
    static MyTcpClient* instance();

    void sendPublicMessage(const QString &message_text);
    void sendPrivateMessage(const QString &reciever, const QString &message_text);

    void joinChat(const QString &host, const int &port, const QString &nickname);
    void leftChat();
    void joinGroup(const QString &group, const QString &password);
    void leftGroup();
    void createGroup(const QString &group, const QString &password);

    static QByteArray makeByteArray(const int &msg_type, const QStringList &params);

private slots:
    void socketConnected();
    void socketReadyRead();
    void socketDisconnected();

signals:
    void publicMessageRecieved(const QString &sender, const QString &message);
    void privateMessageRecieved(const QString &sender, const QString &message);

    void userJoinRecieved(const QString &sender);
    void userLeftRecieved(const QString &sender);
    void usersListRecieved(const QStringList &users_list);

    void authSuccess(const QString &nickname);
    void authFail();

    void joinGroupSuccess(const QString &group);
    void joinGroupFail(const QString &error);

private:
    MyTcpClient();

    QTcpSocket *tcp_socket;
    QByteArray data {};
    QString client_name;
    QString group_name;
    quint16 block_size;
};
