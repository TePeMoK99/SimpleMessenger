#pragma once

#include <QTcpSocket>
#include <QDebug>

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

    static QByteArray makeByteArray(const int &msg_type, const QStringList &params);

public slots:
    void joinChat(const QString &host, const int &port, const QString &nickname);
    void leftChat();

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

    void userAuthSuccess();
    void userAuthFail();

private:
    MyTcpClient();

    QTcpSocket *tcp_socket;
    QByteArray data {};
    QString client_name;
    quint16 block_size;
};
