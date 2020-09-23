#pragma once

#include <QAbstractListModel>
#include <QList>

#include "userlistitem.h"
#include "mytcpclient.h"

class UserListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int usersOnline READ usersOnline WRITE setUsersOnline NOTIFY usersOnlineChanged)

public:
    explicit UserListModel(QObject *parent = nullptr);

    enum Roles
    {
        NicknameRole = Qt::UserRole + 1,
        OnlineRole,
        ColorRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int usersOnline() const { return m_users_online; }

public slots:
    void setUsersOnline(const int &users_online);

signals:
    void usersOnlineChanged(const int &users_online);

private slots:
    void addUser(const QString &nickname);
    void removeUser(const QString &nickname);
    void recieveUsersList(const QStringList &users_list);

private:
    QList<UserListItem> m_users_list;
    MyTcpClient *tcp_client;
    int m_users_online;
};
