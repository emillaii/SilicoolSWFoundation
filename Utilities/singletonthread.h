#ifndef SINGLETONTHREAD_H
#define SINGLETONTHREAD_H

#include "utilities_global.h"
#include <QThread>

class SingletonThd;

class AutoStartThread
{
public:
    AutoStartThread()
    {
        thd.start();
    }

    ~AutoStartThread()
    {
        thd.quit();
        thd.wait();
    }

private:
    friend class SingletonThd;

    QThread thd;
};

class UTILITIESSHARED_EXPORT SingletonThd
{
public:
    static QThread &serialPortThd();

    static QThread &timerThread();
};

#endif    // SINGLETONTHREAD_H
