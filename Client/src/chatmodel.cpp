#include "chatmodel.h"

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel(parent)
{ }

int ChatModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_message_list.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{    
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case SenderRole:    return QVariant(m_message_list[index.row()].sender);
        case MessageRole:   return QVariant(m_message_list[index.row()].message);
        case IsMyRole:      return QVariant(m_message_list[index.row()].isMy);
        case TimeRole:      return QVariant(m_message_list[index.row()].time);
    }

    return QVariant();
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> roles {};
    roles[SenderRole] = "sender_";
    roles[MessageRole] = "message_";
    roles[TimeRole] = "time_";
    roles[IsMyRole] = "isMy_";

    return roles;
}

void ChatModel::sendMessage(const QString &message_text)
{
    udp_client->sendMessage(message_text, MyUdpClient::USUAL_MESSAGE);

    emit beginInsertRows(QModelIndex(), 0, 0);

    m_message_list.push_front(MessageItem(m_nickname, message_text, true));

    emit endInsertRows();
}

void ChatModel::recieveMessage(const MessageItem &message)
{
//    if (message.sender == m_nickname)
//        return;

    emit beginInsertRows(QModelIndex(), 0, 0);

    m_message_list.push_front(message);

    emit endInsertRows();
}

void ChatModel::enterChat(const QString &nickname, const int &port)
{
    m_nickname = nickname;
    udp_client = new MyUdpClient(m_nickname, port);
    connect(udp_client, &MyUdpClient::messageRecieved, this, &ChatModel::recieveMessage);
}
