#include "tcpserver.h"

namespace Server
{
TCPServer::TCPServer(QObject *parent) : QTcpServer(parent)
{
    crud_processor = CRUD::Processor::instance();
}

void TCPServer::incomingConnection(qintptr handle)
{
    TCPClient *new_client = new TCPClient(handle);
    connect(new_client, &TCPClient::authRequest, this, &TCPServer::onAuthRequest);
    connect(new_client, &TCPClient::registerRequest, this, &TCPServer::onRegisterRequest);
    connect(new_client, &TCPClient::clientDisconnected, this, &TCPServer::onClientDisconnected);

    connect(new_client, &TCPClient::createGroupRequest, this, &TCPServer::onCreateGroupRequest);
    connect(new_client, &TCPClient::joinGroupRequest, this, &TCPServer::onJoinGroupRequest);
    connect(new_client, &TCPClient::leaveGroupRequest, this, &TCPServer::onLeaveGroupRequest);

    connect(new_client, &TCPClient::publicMessage, this, &TCPServer::onPublicMessage);
    connect(new_client, &TCPClient::privateMessage, this, &TCPServer::onPrivateMessage);

    connect(new_client, &TCPClient::usersListRequest, this, &TCPServer::onUsersListRequest);

    not_auth_clients[handle] = new_client;
    qDebug() << handle << "connected";
}

bool TCPServer::start()
{
    if (!listen(QHostAddress::Any, 12345))
    {
        return false;
    }

    qDebug() << "Server successfuly started";
    qDebug() << "List of aviable groups: ";

    QStringList groups_vector {crud_processor->requestGroupsList()};

    for (const auto &i : groups_vector)
    {
        groups[i] = Group(i);
        qDebug() << i;
    }

    return true;
}

void TCPServer::stop()
{
    // TODO: stop server
}

/*
 * Operations with users:
*/
void TCPServer::onRegisterRequest(quintptr handle, QString name, QString password)
{
    if (crud_processor->userExist(name))
    {
        QByteArray data {makeByteArray(MessageType::REGISTER_FAIL, "This nickname already used")};
        not_auth_clients[handle]->socket->write(data);
        qDebug() << "Nickname \"" + name + "\" already used";

        return;
    }

    if (!crud_processor->registerUser(name, password))
    {
        QByteArray data {makeByteArray(MessageType::REGISTER_FAIL, "Register fail")};
        not_auth_clients[handle]->socket->write(data);

        return;
    }

    qDebug() << name << "registered";

    QByteArray data {makeByteArray(MessageType::REGISTER_SUCCESS, name)};
    not_auth_clients[handle]->socket->write(data);
}

void TCPServer::onAuthRequest(quintptr handle, QString name, QString password)
{
    if (!crud_processor->userExist(name))
    {
        QByteArray data {makeByteArray(MessageType::AUTH_FAIL, "This user not exist")};
        not_auth_clients[handle]->socket->write(data);
        qDebug() << name << "not exist";

        return;
    }

    auto isAlreadyJoined = [this](const QString &client_name)
    {
        for (const auto &i : qAsConst(groups))
        {
            for (const auto &j : i.clients)
            {
                if (j->name == client_name)
                {
                    return true;
                }
            }
        }
        return false;
    };

    if (isAlreadyJoined(name))
    {
        QByteArray data {makeByteArray(MessageType::AUTH_FAIL,
            "User with this nickname already on server")};
        not_auth_clients[handle]->socket->write(data);
        qDebug() << "User with \"" + name + "\" nickname already on server";

        return;
    }
    if (!crud_processor->checkUserPassword(name, password))
    {
        QByteArray data {makeByteArray(MessageType::AUTH_FAIL, "Wrong nickname or password")};
        not_auth_clients[handle]->socket->write(data);
        qDebug() << name << "wrong password";

        return;
    }

    qDebug() << name << "authorized";

    not_auth_clients[handle]->current_group = "None";
    not_auth_clients[handle]->name = name;

    groups["None"].clients[name] = std::move(not_auth_clients[handle]);
    not_auth_clients.remove(handle);

    QByteArray data {makeByteArray(MessageType::AUTH_SUCCESS, name)};
    groups["None"].clients[name]->socket->write(data);
}

void TCPServer::onClientDisconnected(QString name)
{
    groups["None"].clients.remove(name);
}

/*\  /                         \  /
 * \/ Operations with messages: \/
*/

void TCPServer::onPublicMessage(QString sender, QString group_name, QString msg)
{
    QByteArray data {makeByteArray(MessageType::PUBLIC_MESSAGE, {sender, msg})};
    for (const auto &i : qAsConst(groups[group_name].clients))
    {
        i->socket->write(data);
    }
}

void TCPServer::onPrivateMessage(QString sender, QString receiver, QString group_name, QString msg)
{
    if (!groups[group_name].clients.contains(receiver))
    {
        QByteArray data {makeByteArray(MessageType::PRIVATE_MESSAGE_FAIL, "This user not exist")};
        groups[group_name].clients[sender]->socket->write(data);

        return;
    }

    QByteArray data {makeByteArray(MessageType::PRIVATE_MESSAGE, {sender, receiver, msg})};
    groups[group_name].clients[receiver]->socket->write(data);
    groups[group_name].clients[sender]->socket->write(data);
}

/*\  /                       \  /
 * \/ Operations with groups: \/
*/

void TCPServer::onCreateGroupRequest(QString client_name, QString group_name, QString group_password)
{
    if (crud_processor->groupExist(group_name))
    {
        QByteArray data {makeByteArray(MessageType::CREATE_GROUP_FAIL, "This group name already used")};
        groups["None"].clients[client_name]->socket->write(data);

        return;
    }
    crud_processor->registerGroup(group_name, group_password);
    groups[group_name] = Group(group_name);
    qDebug() << group_name + " created";

    QByteArray data {makeByteArray(MessageType::CREATE_GROUP_SUCCESS, group_name)};
    groups["None"].clients[client_name]->socket->write(data);
}

void TCPServer::onJoinGroupRequest(QString client_name, QString group_name, QString group_password)
{
    if (!groups.contains(group_name))
    {
        QByteArray data {makeByteArray(MessageType::JOIN_GROUP_FAIL, "Group not exist")};
        groups["None"].clients[client_name]->socket->write(data);

        return;
    }
    if (!crud_processor->checkGroupPassword(group_name, group_password))
    {
        QByteArray data {makeByteArray(MessageType::JOIN_GROUP_FAIL, "Wrong password")};
        groups["None"].clients[client_name]->socket->write(data);

        return;
    }

    groups["None"].clients[client_name]->current_group = group_name;
    groups[group_name].clients[client_name] = std::move(groups["None"].clients[client_name]);
    groups["None"].clients.remove(client_name);

    qDebug() << client_name + " [ None -> " + group_name + " ]";
    QByteArray data {makeByteArray(MessageType::JOIN_GROUP_SUCCESS, group_name)};
    groups[group_name].clients[client_name]->socket->write(data);

    data = makeByteArray(MessageType::USER_JOIN, client_name);

    for (const auto &i : qAsConst(groups[group_name].clients))
    {
        i->socket->write(data);
    }   
}

void TCPServer::onLeaveGroupRequest(QString client_name, QString group_name)
{
    groups[group_name].clients[client_name]->current_group = "None";
    groups["None"].clients[client_name] = std::move(groups[group_name].clients[client_name]);
    groups[group_name].clients.remove(client_name);

    groups["None"].clients[client_name]->socket->write(makeByteArray(MessageType::LEAVE_GROUP_SUCCESS));

    qDebug() << client_name + " [ " + group_name + " -> " + "None ]";

    QByteArray data {makeByteArray(MessageType::USER_LEFT, client_name)};

    for (const auto &i : qAsConst(groups[group_name].clients))
    {
        i->socket->write(data);
    }
}

void TCPServer::onUsersListRequest(QString client_name, QString group_name)
{
    QString clients_list {};
    qDebug() << "Online users:";

    for (const auto &i : qAsConst(groups[group_name].clients))
    {
        clients_list += i->name + "|";
        qDebug() << i->name;
    }

    clients_list.remove(clients_list.length() - 1, 1);

    QByteArray data {makeByteArray(MessageType::USERS_LIST, clients_list)};
    groups[group_name].clients[client_name]->socket->write(data);
}

QByteArray TCPServer::makeByteArray(const quint8 &msg_type, const QStringList &params)
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << quint16(0) << quint8(msg_type);

    for (const auto &i : params)
        data_stream << i;

    data_stream.device()->seek(0);
    data_stream << static_cast<quint16>(data.size() - sizeof(quint16));

    return data;
}

QByteArray TCPServer::makeByteArray(const quint8 &msg_type, const QString &param)
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);
    data_stream << quint16(0) << quint8(msg_type) << param;

    data_stream.device()->seek(0);
    data_stream << static_cast<quint16>(data.size() - sizeof(quint16));

    return data;
}
}
