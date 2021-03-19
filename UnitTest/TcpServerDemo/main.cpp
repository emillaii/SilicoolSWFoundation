#include "keyevent.h"
#include "sctcpserver.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SCTcpServer server;
    server.listen(QHostAddress::LocalHost, 5000);
    qDebug() << "Main thread id" << QThread::currentThreadId();
    qDebug() << "Listen local host, port: 5000";

    KeyEvent ke;
    ke.start();
    QObject::connect(&ke, &KeyEvent::disposeServer, &server, &SCTcpServer::dispose);
    QObject::connect(&ke, &KeyEvent::closeProgram, &a, &QCoreApplication::quit);

    return a.exec();
}
