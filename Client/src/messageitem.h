#pragma once

#include <QString>
#include <QColor>

struct MessageItem
{
    MessageItem() :
        sender {""}, message {""}, fontColor {Qt::black}, time {"00:00"}, isMy {true}
    { }

    MessageItem(const QString &sender, const QString &message, const QColor &fontColor,
                const QColor &backColor, const bool &isMy = false, const QString &time = "00:00") :
        sender {sender}, message {message}, fontColor {fontColor},
        backColor {backColor}, time {time}, isMy {isMy}
    { }

    QString sender;
    QString message;
    QColor fontColor;
    QColor backColor;
    QString time;
    bool isMy;
};
