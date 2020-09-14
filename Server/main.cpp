#include <QCoreApplication>

#include "src/mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;
    server.start("127.0.0.1", 11111);

    return a.exec();
}
