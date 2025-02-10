#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    quint16 port = 1234;
    if (argc == 2)
    {
        port = std::atoi(argv[1]);
    }

    server.startServer(port);

    return a.exec();
}
