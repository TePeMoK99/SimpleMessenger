#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent) :
    QAbstractListModel  {parent},
    tcp_client          {TCPClient::instance()},
    m_users_online      {0}
{

    connect(tcp_client, &TCPClient::userJoinRecieved,  this, &UserListModel::onUserJoinRecieved);
    connect(tcp_client, &TCPClient::userLeftRecieved,  this, &UserListModel::onUserLeftRecieved);
    connect(tcp_client, &TCPClient::usersListRecieved, this, &UserListModel::onUsersListRecieved);
}

int UserListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_users_list.size();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    switch (role)
    {
    case NicknameRole:  return m_users_list[index.row()].nickname;
    case IsOnlineRole:  return m_users_list[index.row()].online;
    case ColorRole:     return m_users_list[index.row()].color;
    }

    return QVariant();
}

QHash<int, QByteArray> UserListModel::roleNames() const
{    
    return {
        { NicknameRole,  "nickname_" },
        { IsOnlineRole,  "online_"   },
        { ColorRole,     "color_"    }
    };
}

void UserListModel::setUsersOnline(int users_online)
{
    if (m_users_online == users_online)
    {
        return;
    }

    m_users_online = users_online;
    usersOnlineChanged(m_users_online);
}

void UserListModel::onUserJoinRecieved(QString nickname)
{
    if (m_users_list.contains(UserListItem(nickname, Qt::black, true)))
    {
        return;
    }

    beginInsertRows(QModelIndex(), 0, 0);

    if (nickname == tcp_client->name)
    {
        m_users_list.push_front(UserListItem(nickname, Qt::darkGreen, true));
    }
    else
    {
        m_users_list.push_front(UserListItem(nickname, Qt::black, true));
    }

    endInsertRows();

    setUsersOnline(m_users_online + 1);
}

void UserListModel::onUserLeftRecieved(QString nickname)
{
    const int size {m_users_list.size()};

    for (int i {0}; i < size; i++)
    {
        if (m_users_list[i].nickname == nickname)
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_users_list.removeAt(i);
            endRemoveRows();

            setUsersOnline(m_users_online - 1);

            break;
        }
    }
}

void UserListModel::onUsersListRecieved(QStringList users_list)
{
    beginRemoveRows(QModelIndex(), 0, m_users_list.size() - 1);
    m_users_list.clear();
    endRemoveRows();
    setUsersOnline(0);

    for (auto i : users_list)
    {
        onUserJoinRecieved(i);
    }
}
