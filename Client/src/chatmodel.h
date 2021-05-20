#pragma once

#include "tcpclient.h"

#include <QAbstractListModel>
#include <QTime>
#include <QCryptographicHash>
#include <QByteArray>
#include <QHostAddress>

#include "messageitem.h"


class ChatModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool    isAuth   READ isAuth   NOTIFY isAuthChanged)
    Q_PROPERTY(bool    isJoined READ isJoined NOTIFY isJoinedChanged)
    Q_PROPERTY(QString group    READ group    NOTIFY groupChanged)
    Q_PROPERTY(QString nickname READ nickname NOTIFY nicknameChanged)

public:
    explicit ChatModel(QObject *parent = nullptr);

    enum Roles
    {
        SenderRole = Qt::UserRole + 1,
        MessageRole,
        TimeRole,
        IsMyRole,
        FontColorRole,
        BackColorRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool isAuth() const      { return m_isAuth;   }
    bool isJoined() const    { return m_isJoined; }
    QString nickname() const { return m_nickname; }
    QString group() const    { return m_group;    }

public slots:
    void sendPrivateMsg(const QString &reciever, const QString &message);
    void sendPublicMsg(const QString &message);

    void signUp(const QString &nickname, const QString &password);
    void signIn(const QString &nickname, const QString &password);
    void signOut();
    void joinGroup(const QString &group_name, const QString &password);
    void leaveGroup();
    void createGroup(const QString &group_name, const QString &password);

signals:
    void isAuthChanged(bool isAuth);
    void isJoinedChanged(bool isJoined);
    void nicknameChanged(QString nickname);
    void groupChanged(QString group);
    void gs();

private slots:
    void onPublicMessageRecieved (QString sender, QString message);
    void onPrivateMessageRecieved(QString sender, QString reciever, QString message);

    void onUserJoinRecieved(QString user);
    void onUserLeftRecieved(QString user);

    void onAuthSuccess(QString nickname);
    void onAuthFail(QString error);
    void onRegisterSuccess(QString nickname);
    void onRegisterFail(QString error);

    void onJoinGroupSuccess(QString group);
    void onJoinGroupFail(QString error);
    void onLeaveGroupSuccess();


private:
    void addMsgToList(const MessageItem &msg_item);
    QByteArray hashPassword(const QString &password) const;

    QList <MessageItem> m_messages_list;
    TCPClient *client;
    QString m_nickname;
    QString m_group;
    bool m_isAuth;
    bool m_isJoined;
};
