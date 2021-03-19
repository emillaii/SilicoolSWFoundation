#include "SCtcpserver.h"

qint64 SCTcpServer::sessionCount = 0;

SCTcpServer::SCTcpServer(QObject *parent) : QTcpServer(parent)
{
    connect(this, &SCTcpServer::newConnection, this, &SCTcpServer::onNewConnection);
}

SCTcpServer::~SCTcpServer()
{
    dispose();
}

void SCTcpServer::dispose()
{
    foreach (auto s, sessions.values())
    {
        s->dispose();
    }
    sessions.clear();
    close();
}

void SCTcpServer::onNewConnection()
{
    auto socket = nextPendingConnection();
    qDebug() << "onNewConnection"
             << "peerAddress" << socket->peerAddress() << "peerPort" << socket->peerPort();

    qint64 sessionId = ++sessionCount;
    auto session = new Session(sessionId, socket);
    connect(session, &Session::socketDisconnected, this, &SCTcpServer::onSocketDisconnected);
    sessions[sessionId] = session;
}

void SCTcpServer::onSocketDisconnected(int sessionId)
{
    if (sessions.contains(sessionId))
    {
        auto session = sessions.take(sessionId);
        session->deleteLater();
    }
}
