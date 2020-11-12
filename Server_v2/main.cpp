#include <QCoreApplication>
#include "src/tcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server::TCPServer myserver;
    myserver.start();
    return a.exec();
}
