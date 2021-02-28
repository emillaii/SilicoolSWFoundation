#ifndef SERIALPORTTEST_H
#define SERIALPORTTEST_H

#include "configManager/configfile.h"
#include "serialPortWrapper/scserialport.h"
#include "singletonthread.h"
#include <QObject>
#include <QtConcurrent>

class SerialPortTest : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortTest(QObject *parent = nullptr) : QObject(parent)
    {
        config = new SerialPortConfig();
        configFile = new ConfigFile("", config, "./serialPortConfig.json", false);
        configFile->populate(true);
        serialPort = new SCSerialPort("Test");
        serialPort->setConfig(config);
        serialPort->moveToThread(&SingletonThd::serialPortThd());
    }

public slots:
    void init()
    {
        try
        {
            serialPort->initSerialPort();
        }
        catch (SilicoolException &se)
        {
            qCritical() << se.what();
        }
    }
    void clearReadBuffer()
    {
        serialPort->clearReadBuffer();
    }
    void readEndMark(QString endMark, int timeout)
    {
        QtConcurrent::run(
            [endMark, timeout, this] { qDebug() << "readEndMark" << QThread::currentThreadId() << serialPort->readData(endMark, timeout); });
    }
    void readLength(int length, int timeout)
    {
        QtConcurrent::run([=] { qDebug() << "readLength" << QThread::currentThreadId() << serialPort->readData(length, timeout); });
    }
    void writeData(QString data)
    {
        QtConcurrent::run([data, this] {
            qDebug() << "writeData" << QThread::currentThreadId();
            serialPort->writeData(data.toUtf8());
        });
    }

private:
    ConfigFile *configFile;
    SCSerialPort *serialPort;
    SerialPortConfig *config;
};

#endif    // SERIALPORTTEST_H
