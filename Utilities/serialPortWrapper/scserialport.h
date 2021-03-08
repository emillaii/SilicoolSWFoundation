#ifndef SCSERIALPORT_H
#define SCSERIALPORT_H

#include "TaskEngine/objectlivedthreadinstructionexecutor.h"
#include "configManager/configobject.h"
#include "errorHandling/scassert.h"
#include "errorHandling/silicolerror.h"
#include "highpricisiontimer.h"
#include "utilities_global.h"
#include <QObject>
#include <QSerialPort>

class UTILITIESSHARED_EXPORT SerialPortConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString portName READ portName WRITE setPortName NOTIFY portNameChanged)
    Q_PROPERTY(int baudrate READ baudrate WRITE setBaudrate NOTIFY baudrateChanged)
    Q_PROPERTY(int writeDataMinInterval READ writeDataMinInterval WRITE setWriteDataMinInterval NOTIFY writeDataMinIntervalChanged)

public:
    Q_INVOKABLE SerialPortConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        QVariantList comNames;
        for (int i = 1; i < 30; i++)
        {
            comNames.append(QString("COM%1").arg(i));
        }
        setOptionalProperty("portName", comNames);

        QVariantList baudrates;
        baudrates << 9600 << 19200 << 38400 << 115200 << 230400 << 460800 << 921600;
        setOptionalProperty("baudrate", baudrates);

        init();
    }

    QString portName() const
    {
        return m_portName;
    }

    int baudrate() const
    {
        return m_baudrate;
    }

    int writeDataMinInterval() const
    {
        return m_writeDataMinInterval;
    }

public slots:
    void setPortName(QString portName)
    {
        if (m_portName == portName)
            return;

        m_portName = portName;
        emit portNameChanged(m_portName);
    }

    void setBaudrate(int baudrate)
    {
        if (m_baudrate == baudrate)
            return;

        m_baudrate = baudrate;
        emit baudrateChanged(m_baudrate);
    }

    void setWriteDataMinInterval(int writeDataMinInterval)
    {
        if (m_writeDataMinInterval == writeDataMinInterval)
            return;

        m_writeDataMinInterval = writeDataMinInterval;
        emit writeDataMinIntervalChanged(m_writeDataMinInterval);
    }

signals:
    void portNameChanged(QString portName);

    void baudrateChanged(int baudrate);

    void writeDataMinIntervalChanged(int writeDataMinInterval);

private:
    QString m_portName;
    int m_baudrate;
    int m_writeDataMinInterval = -1;
};

class UTILITIESSHARED_EXPORT SCSerialPort : public ObjectLivedThreadInstructionExecutor
{
    Q_OBJECT

public:
    explicit SCSerialPort(const QString &deviceName);

    void setConfig(SerialPortConfig *config);

    virtual ~SCSerialPort();

    void initSerialPort();

    void writeData(const QByteArray &data, bool waitDone = true);

    void clearReadBuffer();

    QByteArray readData(const QString &endMark, int timeout = 10000);

    QByteArray readData(int length, int timeout = 10000);

private slots:
    void onReadyRead();
    void initSerialPortImpl();
    void writeDataImpl(QByteArray data);
    void clearReadBufferImpl();
    QByteArray readEndmarkImpl(QString endMark, int timeout);
    QByteArray readLengthImpl(int length, int timeout);

private:
    bool isInitSerialPort = false;
    QString deviceName;
    SerialPortConfig *config = nullptr;
    QSerialPort *serialPort = nullptr;
    QByteArray readBuffer;
    qint64 lastWriteDataTime = 0;
};

#endif    // SCSERIALPORT_H
