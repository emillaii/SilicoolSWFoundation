#include "tcpserver.h"

TcpServer::TcpServer()
{

}

bool TcpServer::ServerInit(QHostAddress HostAddress, int port)
{
    if(server == nullptr)
    {
        server = new QTcpServer();

        if(!server->listen(HostAddress, port))
        {
            qDebug()<<server->errorString();
            server->close();
            return false;
        }
        else
        {
            connect(server, &QTcpServer::newConnection, this, &TcpServer::server_New_Connect);
            return true;
        }

    }
    else
    {
        qDebug()<<"服务器已经打开";
        return false;
    }


}

void TcpServer::server_New_Connect()
{
    //
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::socket_Read_Data);
    qDebug()<<"已经连接";
}
void TcpServer::socket_Read_Data()
{

//    QString buffer;
//        if (App::HexReceiveTcpClient) {
//            buffer = QUIHelper::byteArrayToHexStr(data);
//        } else if (App::AsciiTcpClient) {
//            buffer = QUIHelper::byteArrayToAsciiStr(data);
//        } else {
//            buffer = QString(data);
//        }

    QByteArray buffer;
    //
    buffer = socket->readAll();
    QString str = QString::fromLocal8Bit(buffer);

    qDebug()<<str;
}

void TcpServer::CloseServer()
{
    server->close();
}

void TcpServer::SendData(QString Msg)
{
    if(socket == nullptr)
        return;
    //QString strData =QString::fromLocal8Bit("Time: ") + QTime::currentTime().toString() + "\n" + str.toLocal8Bit() + "\n";

        QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(utf8);
        QTextCodec* gbk = QTextCodec::codecForName("gbk");
        QString str=Msg;

        //utf8 -> gbk

        //1. utf8 -> unicode

        QString strUnicode= utf8->toUnicode(str.toLocal8Bit().data());

        //2. unicode -> gbk, 得到QByteArray

        QByteArray gb_bytes= gbk->fromUnicode(strUnicode);

        //gbk -> utf8

//        //1. gbk to unicode
//        strUnicode=gbk->toUnicode(str.toLocal8Bit().data());
//        //2. unicode -> utf-8
//        QByteArray utf8_bytes=utf8->fromUnicode(strUnicode);

       socket->write(gb_bytes);
}
