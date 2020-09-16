#pragma once

#include <QString>

struct UserListItem
{
    UserListItem() : nickname {""}, online {false} { }
    UserListItem(const QString &nickname, const bool &online) :
        nickname {nickname}, online {online}
    { }

    QString nickname;
    bool online;
};
