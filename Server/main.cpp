#include <QCoreApplication>
#include "src/mytcpserver.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    MyTCPServer server;
//    server.startServer();
    QJsonArray array;
    QJsonObject json;




    qDebug() << json;

    return a.exec();
}
