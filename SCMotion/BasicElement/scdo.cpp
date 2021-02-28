#include "scdo.h"

SCDO::SCDO(QString name, QObject *parent) : QObject(parent), m_config(nullptr)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<SCDO>(name, this);
}

void SCDO::init()
{
    initImpl();
    isInit = true;
}

bool SCDO::get(int cacheInvalidationTime)
{
    if (!isInit)
    {
        qCCritical(motionCate()) << tr("Didital output %1 is not inited!").arg(name());
        return false;
    }

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastUpdateDoStateTimeStamp > cacheInvalidationTime)
    {
        bool state = getImpl();
        if (m_config->reversal())
        {
            state = !state;
        }
        lastDoState = state;
        lastUpdateDoStateTimeStamp = now;
    }
    return lastDoState;
}

void SCDO::set(bool state)
{
    if (!isInit)
    {
        qCCritical(motionCate()) << tr("Didital output %1 is not inited!").arg(name());
        return;
    }

//    qCDebug(motionCate(), N_FUNC_ARG(state));
    if (m_config->reversal())
    {
        state = !state;
    }
    setImpl(state);
}
