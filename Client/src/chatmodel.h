#pragma once

#include <QAbstractListModel>

#include "messageitem.h"
#include "myudpclient.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ChatModel(QObject *parent = nullptr);

    enum Roles
    {
        SenderRole = Qt::UserRole + 1,
        MessageRole,
        TimeRole,
        IsMyRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void sendMessage(const QString &message_text);
    void enterChat(const QString &nickname, const int &port);


private slots:
    void recieveMessage(const MessageItem &message);

private:
    QList <MessageItem> m_message_list;

    MyUdpClient* udp_client;
    QString m_nickname;
};
