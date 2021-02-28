#include "singletonthread.h"

QThread &SingletonThd::serialPortThd()
{
    static AutoStartThread instance;
    return instance.thd;
}

QThread &SingletonThd::timerThread()
{
    static AutoStartThread instance;
    return instance.thd;
}
