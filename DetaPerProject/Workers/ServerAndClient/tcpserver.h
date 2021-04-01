#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QSctpServer>
#include <QTime>
#include <QTextCodec>

class TcpServer:public QTcpSocket
{
    Q_OBJECT
public:
    TcpServer();

public slots:
    bool ServerInit(QHostAddress HostAddress, int port);
    void server_New_Connect();
    void socket_Read_Data();
    void CloseServer();
    void SendData(QString Msg);

private:
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
};

#endif // SERVER_H
