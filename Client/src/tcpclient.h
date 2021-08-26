#pragma once

#include <QSslSocket>
#include <QTcpSocket>
#include <QDebug>
#include <vector>

class TCPClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TCPClient)
    friend class UserListModel;
    friend class ChatModel;

public:
    static TCPClient* instance();

    void sendPublicMsg(const QString &message_text);
    void sendPrivateMsg(const QString &reciever, const QString &message_text);

    void singUp(const QHostAddress &host, const int &port, const QString &nickname, const QString &password);
    void signIn(const QHostAddress &host, const int &port, const QString &nickname, const QString &password);
    void signOut();
    void joinGroup(const QString &group, const QString &password);
    void leaveGroup();
    void createGroup(const QString &group, const QString &password);

    static QByteArray makeByteArray(const quint8 &msg_type, const QStringList &params);
    static QByteArray makeByteArray(const quint8 &msg_type, const QString &param = {});

private slots:
    void onReayRead();
    void onDisconnected();

signals:
    void publicMsgRecieved (QString sender, QString message);
    void privateMsgRecieved(QString sender, QString reciever, QString message);

    void userJoinRecieved(QString sender);
    void userLeftRecieved(QString sender);
    void usersListRecieved(QStringList users_list);

    void authSuccess(QString nickname);
    void authFail(QString error);
    void registerSuccess(QString nickname);
    void registerFail(QString nickname);

    void joinGroupSuccess(QString group);
    void joinGroupFail(QString error);

    void leaveGroupSuccess();

private:
    TCPClient();

    QSslSocket  *socket;
    QByteArray  data;
    QString     name;
    QString     current_group;
    quint16     block_size;
};
