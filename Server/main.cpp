#include <QCoreApplication>

#include "src/mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server {};
    server.start(QHostAddress::Any, 11111);

    return a.exec();
}
