#include <QCoreApplication>
#include "src/mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTCPServer server;
    server.startServer();

    return a.exec();
}
