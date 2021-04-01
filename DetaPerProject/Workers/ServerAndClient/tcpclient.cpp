#include "tcpclient.h"
#include <qthread.h>

TcpClient::TcpClient()
{

}

bool TcpClient::ClientInit(QString IpAddress, int port)
{
    if(socket == nullptr)
    {
        socket = new QTcpSocket();
        socket->connectToHost(IpAddress, port);

        qDebug()<<"waitForConnected";
        QObject::connect(socket, &QTcpSocket::readyRead, this, &TcpClient::socket_Read_Data);
        QObject::connect(socket, &QTcpSocket::connected, this, &TcpClient::Connected);
        QObject::connect(socket, &QTcpSocket::disconnected, this, &TcpClient::DisConnected);
        return true;
    }
    else
    {
        return false;
    }
}

void TcpClient::socket_Read_Data()
{
    if(socket==nullptr)
    {
        return;
    }
    QByteArray buffer = socket->readAll();
    QString msg = QTextCodec::codecForName("GBK")->toUnicode(buffer);
    emit ClientReadData(this->clientID,this->peerAddress().toString(),this->peerPort(),buffer);
    qDebug()<<msg;

}

void TcpClient::ConnectToServer(QString IpAddress, int port)
{

}

bool TcpClient::SendData(QString Msg)
{
    qDebug()<<Msg;
    try
    {
        if(socket!=nullptr)
        {
            QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
            QTextCodec::setCodecForLocale(utf8);
            QTextCodec* gbk = QTextCodec::codecForName("gbk");
            QString str=Msg;

            //utf8 -> gbk

            //1. utf8 -> unicode

            QString strUnicode= utf8->toUnicode(str.toLocal8Bit().data());

            //2. unicode -> gbk, 得到QByteArray

            QByteArray gb_bytes = gbk->fromUnicode(strUnicode);
            socket->write(gb_bytes);
            return true;
        }
        else
        {
            qDebug()<<"socket is null";
            return false;
        }
    }
    catch(const std::exception& ex)
    {
        qDebug()<<"Send Error";
        return false;
    }
    catch (...)
    {
        qDebug()<<"Send Error...";
        return false;
    }
}

void TcpClient::CloseClient()
{
    //socket->disconnectFromHost();
}

void TcpClient::ReadData()
{
    //读取完整一条数据并发送信号
    QByteArray data=this->readAll();
    emit ClientReadData(this->clientID,this->peerAddress().toString(),this->peerPort(),data);
}

void TcpClient::DisConnected()
{
     qDebug()<<"客户端已和服务器断开连接";
    //断开连接时，发送断开信号
    //socket->disconnectFromHost();
    emit ClientDisConnect(this->clientID,this->peerAddress().toString(),this->peerPort());
}

void TcpClient::Connected()
{
    qDebug()<<"客户端已连接上服务器";
}
