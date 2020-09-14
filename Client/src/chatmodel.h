#pragma once

#include <QAbstractListModel>

#include "messageitem.h"
#include "mytcpclient.h"

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
    void joinChat(const QString &nickname);
    void sendPublicMessage(const QString &message);
    void setIsAuth(const bool &isAuth);

signals:
    void isAuthChanged(const bool &isAuth);

private slots:
    void recievePublicMessage(const QString &sender, const QString &message);
    void recievePrivateMessage(const QString &sender, const QString &message);
    void recieveUserJoin(const QString &sender);
    void recieveUserLeft(const QString &sender);

private:
    QList <MessageItem> m_message_list;

    QString m_nickname;
    bool m_isAuth;

    MyTcpClient *tcp_client;
};
