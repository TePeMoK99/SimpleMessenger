#include "chatmodel.h"
#include "types.h"
#include <QTime>

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel {parent}, m_isAuth {false}
{ }

int ChatModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_messages_list.size();
}

QVariant ChatModel::data(const QModelIndex &index, int role) const
{    
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case SenderRole:    return QVariant(m_messages_list[index.row()].sender);
    case MessageRole:   return QVariant(m_messages_list[index.row()].message);
    case IsMyRole:      return QVariant(m_messages_list[index.row()].isMy);
    case TimeRole:      return QVariant(m_messages_list[index.row()].time);
    case ColorRole:     return QVariant(m_messages_list[index.row()].color);
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
    tcp_client = new MyTcpClient();
    m_nickname = nickname;

    connect(tcp_client, &MyTcpClient::privateMessageRecieved, this, &ChatModel::recievePrivateMessage);
    connect(tcp_client, &MyTcpClient::publicMessageRecieved,  this, &ChatModel::recievePublicMessage);
    connect(tcp_client, &MyTcpClient::userJoinRecieved,       this, &ChatModel::recieveUserJoin);
    connect(tcp_client, &MyTcpClient::userLeftRecieved,       this, &ChatModel::recieveUserLeft);
    connect(tcp_client, &MyTcpClient::userAuthSuccess,        this, &ChatModel::authSuccess);
    connect(tcp_client, &MyTcpClient::userAuthFail,           this, &ChatModel::authFail);

    // TODO: отправить сообщение о том, что пользователь присоединился

    tcp_client->joinChat("127.0.0.1", 11111, m_nickname);
}

void ChatModel::leftChat()
{
    tcp_client->leftChat();
    delete tcp_client;
}

void ChatModel::sendPublicMessage(const QString &message) const
{
    tcp_client->sendPublicMessage(message);
}

void ChatModel::sendPrivateMessage(const QString &reciever, const QString &message) const
{
    tcp_client->sendPrivateMessage(reciever, message);
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
    if (m_nickname == sender)
        addMessageToList(MessageItem(sender, message, Qt::black, true, QTime::currentTime().toString("HH:mm")));
    else
        addMessageToList(MessageItem(sender, message, Qt::black, false, QTime::currentTime().toString("HH:mm")));
}

void ChatModel::recievePrivateMessage(const QString &message, const QString &sender)
{
    if (m_nickname == sender)
        addMessageToList(MessageItem(sender, message, Qt::black, true, QTime::currentTime().toString("HH:mm")));
    else
        addMessageToList(MessageItem(sender + " to you", message, Qt::black, false, QTime::currentTime().toString("HH:mm")));
}

void ChatModel::recieveUserJoin(const QString &sender)
{
    addMessageToList(MessageItem("", sender + " join chat", Qt::darkGreen, false, QTime::currentTime().toString("HH:mm")));
}

void ChatModel::recieveUserLeft(const QString &sender)
{
    addMessageToList(MessageItem("", sender + " left chat", Qt::darkRed, false, QTime::currentTime().toString("HH:mm")));
}

void ChatModel::authSuccess()
{
    setIsAuth(true);
}

void ChatModel::authFail()
{
    setIsAuth(false);
}

void ChatModel::addMessageToList(const MessageItem &msg_item)
{
    emit beginInsertRows(QModelIndex(), 0, 0);

    m_messages_list.push_front(msg_item);

    emit endInsertRows();
}
