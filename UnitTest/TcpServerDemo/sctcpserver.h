#ifndef SCTCPSERVER_H
#define SCTCPSERVER_H

#include "session.h"
#include <QObject>
#include <QTcpServer>

class SCTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit SCTcpServer(QObject *parent = nullptr);
    ~SCTcpServer();

public slots:
    void dispose();
    void onNewConnection();
    void onSocketDisconnected(int sessionId);

private:
    static qint64 sessionCount;
    QMap<qint64, Session *> sessions;
};

#endif    // SCTCPSERVER_H
