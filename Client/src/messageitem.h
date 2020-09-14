#pragma once

#include <QString>
#include <QColor>

struct MessageItem
{
    MessageItem() :
        sender {""}, message {""}, color {Qt::black}, time {"00:00"}, isMy {true}
    { }

    MessageItem(const QString &sender, const QString &message, const QColor &color,
                const bool &isMy = false, const QString &time = "00:00") :
        sender {sender}, message {message}, color {color}, time {time}, isMy {isMy}
    { }

    QString sender;
    QString message;
    QColor color;
    QString time;
    bool isMy;
};
