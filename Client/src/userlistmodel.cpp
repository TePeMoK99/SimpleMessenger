#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    tcp_client = MyTcpClient::instance();
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
    case NicknameRole:  return QVariant(m_users_list[index.row()].nickname);
    case OnlineRole:    return QVariant(m_users_list[index.row()].online);
    }

    return QVariant();
}

QHash<int, QByteArray> UserListModel::roleNames() const
{
    QHash<int, QByteArray> roles {};
    roles[NicknameRole] = "nickname_";
    roles[OnlineRole] = "online_";

    return roles;
}

void UserListModel::addUser(const QString &nickname)
{
    emit beginInsertRows(QModelIndex(), 0, 0);
    m_users_list.append(UserListItem(nickname, true));
    emit endInsertRows();
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
            break;
        }
    }
}
