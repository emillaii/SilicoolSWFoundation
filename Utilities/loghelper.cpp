#include "loghelper.h"

SILICOOL_CREATE_LOGGING_CATEGORY(timerCate, "Timer")

SCTimer::SCTimer(const QString &msg, bool showLog) : m_msg(msg), m_showLog(showLog)
{
    timer.start();
}

SCTimer::SCTimer(const QString &msg, const QLoggingCategory &logCate, bool showLog) : m_msg(msg), m_logCate(&logCate), m_showLog(showLog)
{
    timer.start();
}

SCTimer::~SCTimer()
{
    if (m_showLog)
    {
        if (m_logCate == nullptr)
        {
            qCDebug(timerCate()) << QString("%1 Timecost: %2ms").arg(m_msg).arg(timer.elapsedMs());
        }
        else
        {
            qCDebug((*m_logCate)) << QString("%1 Timecost: %2ms").arg(m_msg).arg(timer.elapsedMs());
        }
    }
}

SCTimerEx::SCTimerEx(QObject *target, const char *propertyName, bool enable) : target(target), propertyName(propertyName), enable(enable)
{
    timer.start();
}

SCTimerEx::~SCTimerEx()
{
    if (enable)
    {
        if (!target->setProperty(propertyName, timer.elapsedMs()))
        {
            qCritical() << QString("Set %1::%2 failed!").arg(target->metaObject()->className()).arg(propertyName);
        }
    }
}
