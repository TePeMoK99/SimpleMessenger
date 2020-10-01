#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent)
    : QAbstractListModel(parent), m_users_online {0}
{
    tcp_client = MyTcpClient::instance();
    connect(tcp_client, &MyTcpClient::userJoinRecieved,  this, &UserListModel::addUser);
    connect(tcp_client, &MyTcpClient::userLeftRecieved,  this, &UserListModel::removeUser);
    connect(tcp_client, &MyTcpClient::usersListRecieved, this, &UserListModel::recieveUsersList);
}

int UserListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_users_list.size();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case NicknameRole: return QVariant(m_users_list[index.row()].nickname);
    case OnlineRole:   return QVariant(m_users_list[index.row()].online);
    case ColorRole:    return QVariant(m_users_list[index.row()].color);
    }

    return QVariant();
}

QHash<int, QByteArray> UserListModel::roleNames() const
{
    QHash<int, QByteArray> roles {};
    roles[NicknameRole] = "nickname_";
    roles[OnlineRole] = "online_";
    roles[ColorRole] = "color_";

    return roles;
}

void UserListModel::setUsersOnline(const int &users_online)
{
    if (m_users_online == users_online)
        return;

    m_users_online = users_online;
    emit usersOnlineChanged(m_users_online);
}

void UserListModel::addUser(const QString &nickname)
{
    if (m_users_list.contains(UserListItem(nickname, Qt::black, true)))
        return;

    emit beginInsertRows(QModelIndex(), 0, 0);

    if (nickname == tcp_client->client_name)
        m_users_list.push_front(UserListItem(nickname, Qt::green, true));
    else
        m_users_list.push_front(UserListItem(nickname, Qt::black, true));

    emit endInsertRows();

    setUsersOnline(m_users_online + 1);
}

void UserListModel::removeUser(const QString &nickname)
{
    const int size {m_users_list.size()};

    for (int i {0}; i < size; i++)
    {
        if (m_users_list[i].nickname == nickname)
        {
            emit beginRemoveRows(QModelIndex(), i, i);
            m_users_list.removeAt(i);
            emit endRemoveRows();

            setUsersOnline(m_users_online - 1);
            break;
        }
    }
}

void UserListModel::recieveUsersList(const QStringList &users_list)
{
    emit beginRemoveRows(QModelIndex(), 0, m_users_list.size() - 1);
    m_users_list.clear();
    emit endRemoveRows();
    setUsersOnline(0);

    for (auto i : users_list)
    {
        addUser(i);
    }
}
