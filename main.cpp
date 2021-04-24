#include "MainWindow.h"
#include <QApplication>
#include "Socket/tcp_server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    quint16 port = 8888;
    tcp_server server;
    server.init_server(port);
    return a.exec();
}
