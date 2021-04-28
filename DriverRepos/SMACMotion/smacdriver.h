#ifndef SMACDRIVER_H
#define SMACDRIVER_H

#include "errorHandling/silicolerrorhandler.h"
#include "highpricisiontimer.h"
#include "serialPortWrapper/scserialport.h"
#include "singletonthread.h"
#include <QObject>

class SMACDriver : public QObject
{
    Q_OBJECT
public:
    explicit SMACDriver(SerialPortConfig *serialPortConfig, QObject *parent = nullptr);
    void init();
    void sendComand(QString comd);
    QString readData();
    void clearBuffer();
private:
    QByteArray creatCommand(QString data);
private:
    SCSerialPort *serialPort;
};

#endif // SMACDRIVER_H
