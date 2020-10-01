#pragma once

#include "mytcpclient.h"

#include <QAbstractListModel>
#include "messageitem.h"

// plt.hist(..., ...);

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

    bool isAuth() const      { return m_isAuth; }
    bool isJoined() const    { return m_isJoined; }
    QString nickname() const { return m_nickname; }
    QString group() const    { return m_group; }

public slots:
    void sendPrivateMessage(const QString &reciever, const QString &message);
    void sendPublicMessage(const QString &message);

    void joinChat(const QString &nickname);
    void leftChat();
    void joinGroup(const QString &group_name, const QString &password);
    void leftGroup();
    void createGroup(const QString &group_name, const QString &password);

signals:
    void isAuthChanged(const bool &isAuth);
    void isJoinedChanged(const bool &isJoined);
    void nicknameChanged(const QString &nickname);
    void groupChanged(const QString &group);

private slots:
    void recievePublicMessage(const QString &sender, const QString &message);
    void recievePrivateMessage(const QString &sender, const QString &message);

    void recieveUserJoin(const QString &user_join);
    void recieveUserLeft(const QString &user_join);

    void authSuccess(const QString &nickname);
    void authFail();

    void joinGroupSuccess(const QString &group);
    void joinGroupFail(const QString &error);


private:
    void addMessageToList(const MessageItem &msg_item);

    QList <MessageItem> m_messages_list;
    MyTcpClient *tcp_client;
    QString m_nickname;
    QString m_group;
    bool m_isAuth;
    bool m_isJoined;
};
