﻿#pragma once

#include <QAbstractListModel>
#include <QList>

#include "userlistitem.h"
#include "tcpclient.h"

class UserListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int usersOnline READ usersOnline WRITE setUsersOnline NOTIFY usersOnlineChanged)

public:
    explicit UserListModel(QObject *parent = nullptr);

    enum Roles
    {
        NicknameRole = Qt::UserRole + 1,
        IsOnlineRole,
        ColorRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int usersOnline() const { return m_users_online; }

public slots:
    void setUsersOnline(int users_online);

signals:
    void usersOnlineChanged(int users_online);

    void test();

private slots:
    void onUserJoinRecieved(QString nickname);
    void onUserLeftRecieved(QString nickname);
    void onUsersListRecieved(QStringList users_list);

private:
    QList<UserListItem> m_users_list;
    TCPClient           *tcp_client;
    int                 m_users_online;
};
