#include "session.h"

Command::Command(Session *session, const QString &msg) : session(session), msg(msg) {}

void Command::run()
{
    qDebug() << "thread handle msg:" << msg << "thread id:" << QThread::currentThreadId();
    try
    {
        session->sendMessage(msg + "---------");
    }
    catch (...)    // 异常处理
    {
        session->sendMessage("Error...\r\n");
    }
}

Session::Session(qint64 sessionId, QTcpSocket *socket, QObject *parent) : QObject(parent), sessionId(sessionId), socket(socket)
{
    connect(socket, &QTcpSocket::readyRead, this, &Session::onReadyRead);
    connect(this, &Session::sendMessage, this, &Session::onSendMessage);
    connect(socket, &QTcpSocket::disconnected, this, &Session::onDisconnected);

    heartBeatTimerId = startTimer(5000);
}

Session::~Session()
{
    dispose();
    qDebug() << "Session" << sessionId << "Destruct... ";
}

void Session::dispose()
{
    if (heartBeatTimerId >= 0)
    {
        killTimer(heartBeatTimerId);
        heartBeatTimerId = -1;
    }
    thdPool.waitForDone();
    socket->close();
}

void Session::clearReceiveBuffer()
{
    receiveBuffer.clear();
}

void Session::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == heartBeatTimerId)
    {
        if (!gotHeartbeatbeatRsp)
        {
            qCritical() << "Got heartbeat response timeout! Now close connection...";
            dispose();
            return;
        }
        gotHeartbeatbeatRsp = false;
        sendMessage(heartbeat.toUtf8());
    }
}

void Session::msgHandle(const QString &msg)
{
    qDebug() << "Session" << sessionId << "handle msg:" << msg;

    thdPool.start(new Command(this, msg));
}

void Session::onReadyRead()
{
    auto bytes = socket->readAll();
    qDebug() << "Session" << sessionId << "Receive bytes:" << bytes;
    receiveBuffer += bytes;

    // 粘包，分包处理，提取消息，这里以提取\r\n作为结束符举例
    int index = receiveBuffer.indexOf(endMark);
    while (index >= 0)
    {
        QString msg = receiveBuffer.mid(0, index);
        if (msg == heartbeatRsp)
        {
            gotHeartbeatbeatRsp = true;
        }
        else
        {
            msgHandle(msg);
        }

        receiveBuffer.remove(0, index + endMark.length());
        index = receiveBuffer.indexOf(endMark);
    }
}

void Session::onSendMessage(QString msg)
{
    qDebug() << "Session" << sessionId << "send:" << msg;
    socket->write(msg.toUtf8());
    socket->flush();
}

void Session::onDisconnected()
{
    qDebug() << "Session" << sessionId << "disconnected...";
    emit socketDisconnected(sessionId);
}
