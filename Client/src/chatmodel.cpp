#include "chatmodel.h"
#include "types.h"

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel {parent}, m_isAuth {false}, m_isJoined {false}
{
    client = TCPClient::instance();
    connect(client, &TCPClient::publicMsgRecieved, this, &ChatModel::onPublicMessageRecieved);
    connect(client, &TCPClient::privateMsgRecieved, this, &ChatModel::onPrivateMessageRecieved);
    connect(client, &TCPClient::userJoinRecieved, this, &ChatModel::onUserJoinRecieved);
    connect(client, &TCPClient::userLeftRecieved, this, &ChatModel::onUserLeftRecieved);
    connect(client, &TCPClient::authSuccess, this, &ChatModel::onAuthSuccess);
    connect(client, &TCPClient::registerSuccess, this, &ChatModel::onRegisterSuccess);
    connect(client, &TCPClient::authFail, this, &ChatModel::onAuthFail);
    connect(client, &TCPClient::registerFail, this, &ChatModel::onRegisterFail);
    connect(client, &TCPClient::joinGroupSuccess, this, &ChatModel::onJoinGroupSuccess);
    connect(client, &TCPClient::joinGroupFail, this, &ChatModel::onJoinGroupFail);
    connect(client, &TCPClient::leaveGroupSuccess, this, &ChatModel::onLeaveGroupSuccess);
}

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
    case IsMyRole:      return QVariant(m_messages_list[index.row()].isMine);
    case TimeRole:      return QVariant(m_messages_list[index.row()].time);
    case FontColorRole: return QVariant(m_messages_list[index.row()].fontColor);
    case BackColorRole: return QVariant(m_messages_list[index.row()].backColor);
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
    roles[FontColorRole] = "font_color";
    roles[BackColorRole] = "back_color";

    return roles;
}

void ChatModel::leaveGroup()
{
    client->leaveGroup();
}

void ChatModel::createGroup(const QString &group_name, const QString &password)
{
    client->createGroup(group_name, hashPassword(password));
}

void ChatModel::joinGroup(const QString &group_name, const QString &password)
{
    client->joinGroup(group_name, hashPassword(password));
}

void ChatModel::signUp(const QString &nickname, const QString &password)
{
    client->singUp(QHostAddress::LocalHost, 12345, nickname, hashPassword(password));
    //    client->singUp(QHostAddress::LocalHost, 12345, nickname, password);
}

void ChatModel::signIn(const QString &nickname, const QString &password)
{
    client->signIn(QHostAddress::LocalHost, 12345, nickname, hashPassword(password));
}

void ChatModel::signOut()
{
    client->signOut();
}

void ChatModel::sendPublicMsg(const QString &message)
{
    client->sendPublicMsg(message);
    //
    //
}

void ChatModel::sendPrivateMsg(const QString &reciever, const QString &message)
{
    client->sendPrivateMsg(reciever, message);
    //
    //
}

void ChatModel::onPublicMessageRecieved(QString sender, QString message)
{
    if (sender != m_nickname)
    {
        addMsgToList(MessageItem("From " + sender, message, Qt::black, "#90EE90",
                                 false, QTime::currentTime().toString("HH:mm")));
    }
    else
    {
        addMsgToList(MessageItem("You", message, Qt::black, "#80D4FF",
                                 true, QTime::currentTime().toString("HH:mm")));
    }
}

void ChatModel::onPrivateMessageRecieved(QString sender, QString reciever, QString message)
{
    if (sender != m_nickname)
    {
        addMsgToList(MessageItem("From " + sender + " to you", message, Qt::black, "#FFB319",
                                 false, QTime::currentTime().toString("HH:mm")));
    }
    else
    {
        addMsgToList(MessageItem("You to " + reciever, message, Qt::black, "#80D4FF",
                                 true, QTime::currentTime().toString("HH:mm")));
    }
}

void ChatModel::onUserJoinRecieved(QString sender)
{
    addMsgToList(MessageItem("", sender + " join chat", Qt::darkGreen, Qt::lightGray,
                             false, QTime::currentTime().toString("HH:mm")));
}

void ChatModel::onUserLeftRecieved(QString sender)
{
    addMsgToList(MessageItem("", sender + " left chat", Qt::darkRed, Qt::lightGray,
                             false, QTime::currentTime().toString("HH:mm")));
}

void ChatModel::onAuthSuccess(QString nickname)
{
    m_isAuth = true;
    emit isAuthChanged(m_isAuth);
    m_nickname = nickname;
    emit nicknameChanged(m_nickname);
}

void ChatModel::onAuthFail(QString error)
{
    m_isAuth = false;
    emit isAuthChanged(m_isAuth);
    m_nickname = "";
    emit nicknameChanged(m_nickname);
    // TODO: Show message [Auth fail + error]
}

void ChatModel::onRegisterSuccess(QString nickname)
{
    // TODO: Show message [Register success]
}

void ChatModel::onRegisterFail(QString error)
{
    // TODO: Show message [Register fail + error]
}

void ChatModel::onJoinGroupSuccess(QString group)
{
    m_isJoined = true;
    emit isJoinedChanged(m_isJoined);
    m_group = group;
    emit groupChanged(m_group);
}

void ChatModel::onJoinGroupFail(QString error)
{
    m_isJoined = false;
    emit isJoinedChanged(m_isJoined);
    m_group = "";
    emit groupChanged(m_group);
    qDebug() << "Join group error: " + error;
}

void ChatModel::onLeaveGroupSuccess()
{
    emit beginRemoveRows(QModelIndex(), 0, m_messages_list.size() - 1);
    m_messages_list.clear();
    emit endRemoveRows();

    m_isJoined = false;
    emit isJoinedChanged(m_isJoined);
    m_group = "";
    emit groupChanged(m_group);
}

void ChatModel::addMsgToList(const MessageItem &msg_item)
{
    emit beginInsertRows(QModelIndex(), 0, 0);
    m_messages_list.push_front(msg_item);
    emit endInsertRows();

    const int size {m_messages_list.size()};

    if (size > 100)
    {
        emit beginRemoveRows(QModelIndex(), size - 1, size - 1);
        m_messages_list.pop_back();
        emit endRemoveRows();
    }
}

QByteArray ChatModel::hashPassword(const QString &password) const
{
    return QCryptographicHash::hash(QByteArray().append(password), QCryptographicHash::Md5);
}
