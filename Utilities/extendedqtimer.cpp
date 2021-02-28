#include "extendedqtimer.h"

ExtendedQTimer::ExtendedQTimer(QObject *parent) : QTimer(parent)
{
    connect(this, &ExtendedQTimer::startSig, this, &ExtendedQTimer::onStartReq);
    connect(this, &ExtendedQTimer::stopSig, this, &ExtendedQTimer::stop);
}

void ExtendedQTimer::reqStart()
{
    emit startSig(-1, {});
}

void ExtendedQTimer::reqStart(int msec)
{
    emit startSig(msec, {});
}

void ExtendedQTimer::reqStop()
{
    emit stopSig({});
}

void ExtendedQTimer::onStartReq(int msec)
{
    if (msec < 0)
    {
        start();
    }
    else
    {
        start(msec);
    }
}
