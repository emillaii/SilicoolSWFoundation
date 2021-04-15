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

QByteArray SMACDriver::creatCommand(QString data)
{
    return (data + "\r\n").toLatin1();
}
