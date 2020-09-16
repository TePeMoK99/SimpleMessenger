#pragma once

#include "mytcpclient.h"

#include <QAbstractListModel>

#include "messageitem.h"

// plt.hist(..., ...);

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool isAuth READ isAuth WRITE setIsAuth NOTIFY isAuthChanged)

public:
    explicit ChatModel(QObject *parent = nullptr);

    enum Roles
    {
        SenderRole = Qt::UserRole + 1,
        MessageRole,
        TimeRole,
        IsMyRole,
        ColorRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool isAuth() const     { return m_isAuth; }

public slots:
    void sendPrivateMessage(const QString &reciever, const QString &message) const;
    void sendPublicMessage(const QString &message) const;

    void joinChat(const QString &nickname);
    void leftChat();

    void setIsAuth(const bool &isAuth);

signals:
    void isAuthChanged(const bool &isAuth);

private slots:
    void recievePublicMessage(const QString &sender, const QString &message);
    void recievePrivateMessage(const QString &sender, const QString &message);

    void recieveUserJoin(const QString &user_join);
    void recieveUserLeft(const QString &user_join);

    void authSuccess();
    void authFail();

private:
    void addMessageToList(const MessageItem &msg_item);

    QList <MessageItem> m_messages_list;

    QString m_nickname;
    bool m_isAuth;

    MyTcpClient *tcp_client;
};
