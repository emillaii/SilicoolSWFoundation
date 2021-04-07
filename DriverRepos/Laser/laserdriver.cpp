#include "laserdriver.h"

LaserDriver::LaserDriver(SerialPortConfig *serialPortConfig, QObject *parent) : QObject(parent)
{
    serialPort = new SCSerialPort("Laser");
    serialPort->setConfig(serialPortConfig);
    serialPort->moveToThread(&SingletonThd::serialPortThd());

    sp2Data[LaserStationConfig::SP200Us] = "+00000";
    sp2Data[LaserStationConfig::SP500Us] = "+00001";
    sp2Data[LaserStationConfig::SP1Ms] = "+00002";
    sp2Data[LaserStationConfig::SP2Ms] = "+00003";
}

void LaserDriver::init(LaserStationConfig::SamplePeriod samplePeriod, int stationNum)
{
    QMutexLocker t(&locker);

    auto sp = static_cast<LaserStationConfig::SamplePeriod>(samplePeriod);
    serialPort->initSerialPort();
    executeCommand("WIN", "+00001", stationNum);       // 初始化Laser
    executeCommand("WSP", sp2Data[sp], stationNum);    // 采样周期
    executeCommand("WHM", "+00000", stationNum);       // 测量模式：Normal
    executeCommand("WAV", "+00000", stationNum);       // 滑动平均窗口宽度：0
    executeCommand("WHC", "+00000", stationNum);       // 警告延迟次数设为0
    executeCommand("WAD", "+00001", stationNum);       // 警告时输出固定值：+99999
    qDebug() << tr("Init laser successful! Station:") << stationNum;
}

void LaserDriver::dispose()
{
    serialPort->dispose();
}

double LaserDriver::readHeight(int stationNum)
{
    return ErrorHandler::tryToHandleGeneralError<double>([&] { return readHeightNoErrHandling(stationNum); },
                                                         [this] { serialPort->clearReadBuffer(); });
}

double LaserDriver::readHeightNoErrHandling(int stationNum)
{
    QMutexLocker t(&locker);

    serialPort->writeData(createComand("RMD", "", stationNum));
    QString data = extractResponseData(serialPort->readData("\r"), "RMD", stationNum);
    return data.toDouble() / 10000;
}

void LaserDriver::executeCommand(QString command, QString data, int stationNum)
{
    ErrorHandler::tryToHandleGeneralError<void>(
        [&] {
            serialPort->writeData(createComand(command, data, stationNum));
            extractResponseData(serialPort->readData("\r"), command, stationNum);
        },
        [this] { serialPort->clearReadBuffer(); });
}

QByteArray LaserDriver::createComand(QString cmdName, QString data, int stationNum)
{
    QString strStation = QString::number(stationNum);
    if (strStation.length() < 2)
    {
        strStation.prepend('0');
    }
    return (QString("%") + strStation + "#" + cmdName + data + "**\r").toLatin1();
}

QString LaserDriver::extractResponseData(QString response, QString targetCmd, int targetStation)
{
    if (!response.startsWith('%'))
    {
        throw GeneralError("Laser", tr("Unknown response: %1").arg(response));
    }
    int station = response.mid(1, 2).toInt();
    if (station != targetStation)
    {
        throw GeneralError("Laser", tr("UnExpected response station: %1, command station: %2").arg(station).arg(targetStation));
    }
    if (response.mid(3, 1) == "!")
    {
        throw GeneralError("Laser", tr("Laser error, Code: %1").arg(response.mid(4, 2)));
    }
    if (response.mid(4, 3) != targetCmd)
    {
        throw GeneralError("Laser", tr("UnExpected response command: %1, command: %2").arg(response.mid(4, 3)).arg(targetCmd));
    }
    QString data = response.mid(7, response.length() - 10);
    return data;
}
