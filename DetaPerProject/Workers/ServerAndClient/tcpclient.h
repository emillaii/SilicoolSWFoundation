#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QObject>
#include <QTextCodec>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
     TcpClient();

public:
    bool ClientInit(QString IpAddress, int port);

signals:
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientDisConnect(int clientID,QString IP,int Port);

private slots:
    void ReadData();
    void DisConnected();
    void Connected();

public slots:

    bool SendData(QString Msg);
    void socket_Read_Data();
    void ConnectToServer(QString IpAddress, int port);
    void CloseClient();
private:
    QTcpSocket *socket = nullptr;
    int clientID;
};
#endif // CLIENT_H
