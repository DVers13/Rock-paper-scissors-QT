#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    quint16 port = 1234;
    if (argc == 2)
    {
        port = std::atoi(argv[1]);
    }
    w.connectToServer("127.0.0.1", port);
    w.show();
    return a.exec();
}
