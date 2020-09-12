#pragma once

#include <QString>

struct MessageItem
{
    MessageItem() :
        sender {""}, message {""}, time {"00:00"}, isMy(true)
    { }

    MessageItem(const QString &sender, const QString &message,
                const bool &isMy = false, const QString &time = "00:00") :
        sender {sender}, message {message}, time {time}, isMy {isMy}
    { }

    QString sender;
    QString message;
    QString time;
    bool isMy;
};
