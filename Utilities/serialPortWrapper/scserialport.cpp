#include "scserialport.h"

SCSerialPort::SCSerialPort(const QString &deviceName) : deviceName(deviceName) {}

void SCSerialPort::setConfig(SerialPortConfig *config)
{
    this->config = config;
}

SCSerialPort::~SCSerialPort()
{
    if (serialPort != nullptr)
    {
        if (serialPort->isOpen())
        {
            serialPort->close();
        }
        delete serialPort;
        serialPort = nullptr;
    }
}

void SCSerialPort::initSerialPort()
{
    if (isInitSerialPort)
    {
        return;
    }
    runIns("initSerialPortImpl");
    isInitSerialPort = true;
}

void SCSerialPort::dispose()
{
    if (isInitSerialPort)
    {
        isInitSerialPort = false;
        runIns("disposeSerialPortImpl");
    }
}

void SCSerialPort::writeData(const QByteArray &data, bool waitDone)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }
    QVariantList args;
    args << data;
    runIns("writeDataImpl", args, waitDone);
}

void SCSerialPort::clearReadBuffer()
{
    runIns("clearReadBufferImpl");
}

QByteArray SCSerialPort::readData(const QString &endMark, int timeout)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }

    QVariantList args;
    args << endMark << timeout;
    QByteArray rsp = runIns<QByteArray>("readEndmarkImpl", args);
    if (rsp.isEmpty())
    {
        throw GeneralError(deviceName, QObject::tr("Read data timeout!"));
    }
    return rsp;
}

QByteArray SCSerialPort::readData(int length, int timeout)
{
    if (!isInitSerialPort || !serialPort->isOpen())
    {
        throw GeneralError(deviceName, QObject::tr("SerialPort is not opened!"));
    }

    QVariantList args;
    args << length << timeout;
    QByteArray rsp = runIns<QByteArray>("readLengthImpl", args);
    if (rsp.isEmpty())
    {
        throw GeneralError(deviceName, QObject::tr("Read data timeout!"));
    }
    return rsp;
}

void SCSerialPort::onReadyRead()
{
    while (serialPort->bytesAvailable() > 0)
    {
        readBuffer += serialPort->readAll();
    }
}

void SCSerialPort::initSerialPortImpl()
{
    if (serialPort == nullptr)
    {
        serialPort = new QSerialPort;
        connect(serialPort, &QSerialPort::readyRead, this, &SCSerialPort::onReadyRead);
    }
    SC_ASSERT(config);
    if (serialPort->isOpen())
    {
        serialPort->close();
    }
    serialPort->setPortName(config->portName());
    serialPort->setBaudRate(config->baudrate());
    if (!serialPort->open(QIODevice::ReadWrite))
    {
        throw SilicolAbort(
            QObject::tr("Open serial port failed! Device: %1 Port: %2, errMsg: %3").arg(deviceName).arg(config->portName()).arg(serialPort->error()));
    }
    serialPort->clear();
    serialPort->clearError();
}

void SCSerialPort::disposeSerialPortImpl()
{
    if (serialPort != nullptr)
    {
        serialPort->close();
    }
}

void SCSerialPort::writeDataImpl(QByteArray data)
{
    if (config->writeDataMinInterval() > 0)
    {
        qint64 writeInterval = QDateTime::currentMSecsSinceEpoch() - lastWriteDataTime;
        if (writeInterval < config->writeDataMinInterval())
        {
            HighPrecisionTimer::eventLoopSleep(config->writeDataMinInterval() - writeInterval);
        }
    }
    serialPort->write(data);
    serialPort->flush();

    lastWriteDataTime = QDateTime::currentMSecsSinceEpoch();
}

void SCSerialPort::clearReadBufferImpl()
{
    readBuffer.clear();
    if (serialPort != nullptr && serialPort->isOpen())
    {
        serialPort->clear(QSerialPort::Input);
    }
}

QByteArray SCSerialPort::readEndmarkImpl(QString endMark, int timeout)
{
    QElapsedTimer timer;
    timer.start();
    QEventLoop eventLoop;
    while (readBuffer.indexOf(endMark) < 0)
    {
        if (timeout > 0 && timer.elapsed() > timeout)
        {
            return QByteArray();
        }
        eventLoop.processEvents(QEventLoop::AllEvents, 1);
    }
    int dataLen = readBuffer.indexOf(endMark) + endMark.toUtf8().length();
    QByteArray rsp = readBuffer.mid(0, dataLen);
    readBuffer.remove(0, dataLen);
    return rsp;
}

QByteArray SCSerialPort::readLengthImpl(int length, int timeout)
{
    QElapsedTimer timer;
    timer.start();
    QEventLoop eventLoop;
    while (readBuffer.length() < length)
    {
        if (timeout > 0 && timer.elapsed() > timeout)
        {
            return QByteArray();
        }
        eventLoop.processEvents(QEventLoop::AllEvents, 1);
    }
    QByteArray rsp = readBuffer.mid(0, length);
    readBuffer.remove(0, length);
    return rsp;
}
