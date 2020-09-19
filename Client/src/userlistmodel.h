#pragma once

#include <QAbstractListModel>
#include <QList>

#include "userlistitem.h"
#include "mytcpclient.h"

class UserListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit UserListModel(QObject *parent = nullptr);


    enum Roles
    {
        NicknameRole = Qt::UserRole + 1,
        OnlineRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private slots:
    void addUser(const QString &nickname);
    void removeUser(const QString &nickname);

private:
    QList<UserListItem> m_users_list;
    MyTcpClient *tcp_client;
};
