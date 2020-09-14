#include "chatmodel.h"
#include "types.h"
#include <QTime>

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
    case ColorRole:     return QVariant(m_message_list[index.row()].color);
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
    roles[ColorRole] = "color_";

    return roles;
}

void ChatModel::joinChat(const QString &nickname)
{
    tcp_client = new MyTcpClient(nickname);
    m_nickname = nickname;

    connect(tcp_client, &MyTcpClient::publicMessageRecieved, this, &ChatModel::recievePublicMessage);
    connect(tcp_client, &MyTcpClient::privateMessageRecieved, this, &ChatModel::recievePrivateMessage);
    connect(tcp_client, &MyTcpClient::userJoinRecieved, this, &ChatModel::recieveUserJoin);
    connect(tcp_client, &MyTcpClient::userLeftRecieved, this, &ChatModel::recieveUserLeft);

    // TODO: отправить сообщение о том, что пользователь присоединился

    tcp_client->joinChat("127.0.0.1", 11111);
}

void ChatModel::sendPublicMessage(const QString &message)
{
    tcp_client->sendPublicMessage(message);
}

void ChatModel::setIsAuth(const bool &isAuth)
{
    if (m_isAuth == isAuth)
        return;

    m_isAuth = isAuth;

    emit isAuthChanged(m_isAuth);
}

void ChatModel::recievePublicMessage(const QString &sender, const QString &message)
{
    emit beginInsertRows(QModelIndex(), 0, 0);

    if (m_nickname == sender)
        m_message_list.push_front(MessageItem(sender, message, Qt::black, true, QTime::currentTime().toString("HH:mm")));
    else
        m_message_list.push_front(MessageItem(sender, message, Qt::black, false, QTime::currentTime().toString("HH:mm")));

    emit endInsertRows();

}

void ChatModel::recievePrivateMessage(const QString &message, const QString &sender)
{

}

void ChatModel::recieveUserJoin(const QString &sender)
{

}

void ChatModel::recieveUserLeft(const QString &sender)
{

}
