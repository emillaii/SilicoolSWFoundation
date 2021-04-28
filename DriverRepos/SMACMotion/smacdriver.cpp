#include "smacdriver.h"

SMACDriver::SMACDriver(SerialPortConfig *serialPortConfig, QObject *parent)
{
    serialPort = new SCSerialPort("SMAC");
    serialPort->setConfig(serialPortConfig);
    serialPort->moveToThread(&SingletonThd::serialPortThd());
}

void SMACDriver::init()
{
    qWarning() << tr("Init SMAC");
    serialPort->initSerialPort();
}

void SMACDriver::sendComand(QString comd)
{
    serialPort->writeData(creatCommand(comd));
}

QString SMACDriver::readData()
{
    QString returnData = serialPort->readData("\r");
    serialPort->clearReadBuffer();
    return  returnData;
}

void SMACDriver::clearBuffer()
{
    serialPort->clearReadBuffer();
}

QByteArray SMACDriver::creatCommand(QString data)
{
    return (data + "\r").toLatin1();
}
