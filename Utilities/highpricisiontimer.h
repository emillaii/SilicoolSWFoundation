#ifndef HIGHPRICISIONTIMER_H
#define HIGHPRICISIONTIMER_H

#include "utilities_global.h"
#include <QElapsedTimer>
#include <QEventLoop>
#include <QObject>
#include <QTimer>
#include <Windows.h>

class UTILITIESSHARED_EXPORT HighPrecisionTimer
{
public:
    HighPrecisionTimer();

    ~HighPrecisionTimer();

    void start();

    double elapsedMs();

    void uSleep(qint64 us);

    static void eventLoopSleep(int timeMs);

    static bool isHighPrecisionSupported();

    static qint64 performanceFrequency();

    qint64 performanceCounter();

private:
    qint64 frequency;
    QElapsedTimer *timer = nullptr;
    qint64 startTime = 0;
};

#endif    // HIGHPRICISIONTIMER_H
