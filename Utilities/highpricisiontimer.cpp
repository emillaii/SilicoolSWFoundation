#include "highpricisiontimer.h"

HighPrecisionTimer::HighPrecisionTimer()
{
    if (isHighPrecisionSupported())
    {
        frequency = performanceFrequency();
    }
    else
    {
        timer = new QElapsedTimer;
    }
    start();
}

HighPrecisionTimer::~HighPrecisionTimer()
{
    delete timer;
}

void HighPrecisionTimer::start()
{
    if (isHighPrecisionSupported())
    {
        startTime = performanceCounter();
    }
    else
    {
        timer->restart();
    }
}

double HighPrecisionTimer::elapsedMs()
{
    if (isHighPrecisionSupported())
    {
        return (performanceCounter() - startTime) * 1000.0 / frequency;
    }
    else
    {
        return timer->elapsed();
    }
}

void HighPrecisionTimer::uSleep(qint64 us)
{
    if (isHighPrecisionSupported())
    {
        qint64 currentTime = performanceCounter();
        while ((performanceCounter() - currentTime) * 1000000.0 / frequency < us)
        {
        }
    }
    else
    {
        qint64 ms = (us - 1) / 1000 + 1;
        QElapsedTimer tmpTimer;
        tmpTimer.start();
        while (tmpTimer.elapsed() < ms)
        {
        }
    }
}

void HighPrecisionTimer::eventLoopSleep(int timeMs)
{
    QTimer _timer;
    _timer.setSingleShot(true);
    _timer.setTimerType(Qt::PreciseTimer);
    QEventLoop eventLoop;
    QObject::connect(&_timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
    _timer.start(timeMs);
    eventLoop.exec();
}

bool HighPrecisionTimer::isHighPrecisionSupported()
{
    static bool isQueried = false;
    static bool isSuppoerted = false;
    if (!isQueried)
    {
        LARGE_INTEGER frequecy;
        isSuppoerted = QueryPerformanceFrequency(&frequecy);
        isQueried = true;
    }
    return isSuppoerted;
}

qint64 HighPrecisionTimer::performanceFrequency()
{
    static qint64 fre = -1;
    if (fre == -1)
    {
        LARGE_INTEGER frequecy;
        QueryPerformanceFrequency(&frequecy);
        fre = frequecy.QuadPart;
    }
    return fre;
}

qint64 HighPrecisionTimer::performanceCounter()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}
