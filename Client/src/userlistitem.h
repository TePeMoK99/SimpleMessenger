#pragma once

#include <QString>

struct UserListItem
{
    UserListItem() : nickname {""}, online {false} { }
    UserListItem(const QString &nickname, const bool &online) :
        nickname {nickname}, online {online}
    { }

    bool operator==(const UserListItem &item1)
    {
        return item1.nickname == nickname;
    }

    bool operator==(const QString &nick)
    {
        return nickname == nick;
    }

    friend bool operator==(const QString &nick, const UserListItem &item)
    {
        return nick == item.nickname;
    }

    QString nickname;
    bool online;
};
