#include "scdi.h"

SCDI::SCDI(QString name, QObject *parent) : QObject(parent), m_config(nullptr)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<SCDI>(name, this);
}

SCDI::~SCDI() {}

void SCDI::init()
{
    initImpl();
    m_isInit = true;
}

void SCDI::disableAll()
{
    disableWait();
    disableCheck();
}

void SCDI::enableAll()
{
    enableWait();
    enableCheck();
}

bool SCDI::get(int cacheInvalidationTime)
{
    if (!m_isInit)
    {
        qCCritical(motionCate()) << tr("Didital input %1 is not inited!").arg(name());
        return false;
    }

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastUpdateDiStateTimeStamp > cacheInvalidationTime)
    {
        bool state = getImpl();
        if (m_config->reversal())
        {
            state = !state;
        }
        lastDiState = state;
        lastUpdateDiStateTimeStamp = now;
    }
    return lastDiState;
}

bool SCDI::waitState(bool targetState, int timeout)
{
    if (isDisableWait)
    {
        qCDebug(motionCate()) << QString("DI %1 disabled wait function!").arg(name());
        return true;
    }

    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (timer.elapsed() > timeout)
        {
            return false;
        }
        if (get() == targetState)
        {
            return true;
        }
        QThread::msleep(1);
    }
}

bool SCDI::checkState(bool targetState, int cacheInvalidationTime)
{
    if (isDisableCheck)
    {
        qCDebug(motionCate()) << QString("Vacuum %1 disabled check function!").arg(name());
        return true;
    }
    return get(cacheInvalidationTime) == targetState;
}
