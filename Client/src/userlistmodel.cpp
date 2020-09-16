#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent)
    : QAbstractListModel(parent)
{
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
