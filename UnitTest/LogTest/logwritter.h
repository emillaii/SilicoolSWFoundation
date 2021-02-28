#ifndef LOGWRITTER_H
#define LOGWRITTER_H

#include "loging/Logger.h"
#include <QObject>
#include <QThread>

class LogWritter : public QThread
{
    Q_OBJECT

public:
    explicit LogWritter(QObject *parent = nullptr) : QThread(parent)
    {
        instanceCount++;
        name = QString("Writter%1").arg(instanceCount).toUtf8();
        cate = new QLoggingCategory(name);
        Logger::getIns()->registerCategory(cate);
    }

    void startWrite(int interval)
    {
        this->interval = interval;
        isRun = true;
        start();
    }

    void stopWrite()
    {
        isRun = false;
        wait();
    }

    // QThread interface
protected:
    void run()
    {
        int tick = 0;
        while (isRun)
        {
            if (interval > 0)
            {
                msleep(interval);
            }

            int index = tick % 4;
            if (index == 0)
            {
                qCDebug((*cate)) << name << tick;
            }
            else if (index == 1)
            {
                qCInfo((*cate)) << name << tick;
            }
            else if (index == 2)
            {
                qCCritical((*cate)) << name << tick;
            }
            else
            {
                qCWarning((*cate)) << name << tick;
            }
            tick++;
        }
    }

private:
    int interval;
    bool isRun = false;
    static int instanceCount;
    QByteArray name;
    QLoggingCategory *cate;
};

#endif    // LOGWRITTER_H
