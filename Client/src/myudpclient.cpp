#include "myudpclient.h"
#include <QDataStream>
#include <QMap>

MyUdpClient::MyUdpClient() : nickname {"Default"}, port {00000}
{
    udp_socket = new QUdpSocket(this);
    udp_socket->bind(QHostAddress::Any, port);

    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

    qDebug() << "UDPClient created!";
}

MyUdpClient::MyUdpClient(const QString &nickname, const int &port) :
    nickname {nickname}, port {port}
{
    udp_socket = new QUdpSocket(this);
    udp_socket->bind(QHostAddress::Any, port);

    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

    qDebug() << "UDPClient created!";
}

void MyUdpClient::readMessage()
{
    qDebug() << "Message recieved";

    QByteArray data;
    data.resize(udp_socket->pendingDatagramSize());

    QHostAddress *address = new QHostAddress();
    udp_socket->readDatagram(data.data(), data.size(), address);

    qDebug() << address->toString();

    QDataStream data_stream(&data, QIODevice::ReadOnly);

    qint64 size {-1};

    if(data_stream.device()->size() > static_cast<qint64>(sizeof(qint64)))
    {
        data_stream >> size;
    }
    else
        return;

    if (data_stream.device()->size() - static_cast<qint64>(sizeof(qint64)) < size)
        return;

    qint8 type {0};
    data_stream >> type;

    switch (type)
    {
    case USUAL_MESSAGE:
    {
        QString message;
        data_stream >> message;
        QStringList splitted_message {message.split("|")};

        emit messageRecieved(MessageItem(address->toString(), splitted_message[1]));

        break;
    }
    case PERSON_ONLINE:
    {        
//        QString message;
//        data_stream >> message;
//        users_list[address.toString()] = message;

        break;
    }
    case WHO_IS_ONLINE:
    {
        sendMessage(nickname, PERSON_ONLINE);

        break;
    }
    }
}

bool MyUdpClient::sendMessage(const QString &message, const qint8 &type_of_message)
{
    qDebug() << "message sended";

    if (udp_socket->writeDatagram(prepareByteArray(message, type_of_message),
                                  QHostAddress::LocalHost, port) != -1)
        return true;

    return false;
}

QByteArray MyUdpClient::prepareByteArray(const QString &message, const qint8 &type_of_message) const
{
    QByteArray data;
    QDataStream data_stream(&data, QIODevice::WriteOnly);

    data_stream << qint64(0);
    data_stream << type_of_message;
    data_stream << nickname + "|" + message;

    data_stream.device()->seek(qint64(0));
    data_stream << qint64(data.size() - sizeof(qint64));

    return data;
}
