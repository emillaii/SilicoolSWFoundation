#ifndef LOGWRITTERMANAGER_H
#define LOGWRITTERMANAGER_H

#include "logwritter.h"
#include <QObject>

class LogWritterManager : public QObject
{
    Q_OBJECT
public:
    explicit LogWritterManager(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void create(int count)
    {
        destroy();
        for (int i = 0; i < count; i++)
        {
            writers.append(new LogWritter());
        }
    }

    Q_INVOKABLE void destroy()
    {
        while (writers.count() > 0)
        {
            auto writer = writers.takeFirst();
            writer->stopWrite();
            delete writer;
        }
    }

    Q_INVOKABLE void start(int interval)
    {
        foreach (auto writer, writers)
        {
            writer->startWrite(interval);
        }
    }
    Q_INVOKABLE void stop()
    {
        foreach (auto writer, writers)
        {
            writer->stopWrite();
        }
    }

private:
    QList<LogWritter *> writers;
};

#endif    // LOGWRITTERMANAGER_H
