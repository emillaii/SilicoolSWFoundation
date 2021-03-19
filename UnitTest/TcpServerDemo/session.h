#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QTcpSocket>
#include <QThreadPool>
#include <QTimerEvent>

class Session;

class Command : public QRunnable
{
public:
    Command(Session *session, const QString &msg);

    // QRunnable interface
public:
    void run();

private:
    Session *session;
    QString msg;
};

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(qint64 sessionId, QTcpSocket *socket, QObject *parent = nullptr);
    ~Session();
    void dispose();
    void clearReceiveBuffer();

signals:
    void sendMessage(QString msg);
    void socketDisconnected(qint64 sessionId);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);

private:
    void msgHandle(const QString &msg);

private slots:
    void onReadyRead();
    void onSendMessage(QString msg);
    void onDisconnected();

private:
    const QString heartbeat = "heartbeat\r\n";
    const QString heartbeatRsp = "heartbeatRsp";
    bool gotHeartbeatbeatRsp = true;
    int heartBeatTimerId = -1;

    const QByteArray endMark = "\r\n";
    qint64 sessionId;
    QTcpSocket *socket;
    QByteArray receiveBuffer;
    QThreadPool thdPool;
};

#endif    // SESSION_H
