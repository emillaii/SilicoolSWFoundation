#include "scvacuum.h"

SCVacuum::SCVacuum(QString name, QObject *parent) : QObject(parent)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<SCVacuum>(name, this);
    schedulingTimer.setSingleShot(true);
    connect(&schedulingTimer, &QTimer::timeout, this, &SCVacuum::closeBlowOutput, Qt::DirectConnection);
}

VacuumConfig *SCVacuum::config() const
{
    return m_config;
}

void SCVacuum::setConfig(VacuumConfig *config)
{
    m_config = config;
}

void SCVacuum::init()
{
    hasBlowOutput = !m_config->blowOutputName().isEmpty();
    feedbackInput = MotionElementContainer::getIns()->getItem<SCDI>(m_config->feedbackInputName());
    vacuumizeOutput = MotionElementContainer::getIns()->getItem<SCDO>(m_config->vacuumizeOutputName());
    if (hasBlowOutput)
    {
        blowOutput = MotionElementContainer::getIns()->getItem<SCDO>(m_config->blowOutputName());
    }
}

void SCVacuum::disableAll()
{
    disableSetAndWait();
    disableCheck();
}

void SCVacuum::enableAll()
{
    enableSetAndWait();
    enableCheck();
}

void SCVacuum::set(bool state, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(state, waitDone));

    if (isDisableSetAndWait)
    {
        qCDebug(motionCate()) << QString("Vacuum %1 disabled set function!").arg(name());
        return;
    }

    if (get() == state && vacuumizeOutput->get() == state)
    {
        return;
    }

    SCTimer timer(QString("Set vacuum %1 to %2 state.").arg(name()).arg(state), motionCate(), waitDone);

    ErrorHandler::tryToHandleError([&] {
        vacuumizeOutput->set(state);
        if (!state && hasBlowOutput)
        {
            if (m_config->blowTime() > 0 || m_config->blowTime2() > 0)
            {
                if (m_config->blowDelayAfterCloseVacuum() > 0)
                {
                    QThread::msleep(m_config->blowDelayAfterCloseVacuum());
                }
                blowOutput->set(true);
                if (m_config->blowTime() > 0)
                {
                    QThread::msleep(m_config->blowTime());
                }
                if (m_config->blowTime2() > 0)
                {
                    schedulingTimer.reqStart(m_config->blowTime2());
                }
                else
                {
                    blowOutput->set(false);
                }
            }
        }
        if (waitDone)
        {
            if (!waitUntilTimeout(state))
            {
                throw ActionError("Vacuum", QObject::tr("Set vacuum %1 to %2 state timeout!").arg(name()).arg(state));
            }
        }
    });
}

void SCVacuum::wait(bool state)
{
    if (isDisableSetAndWait)
    {
        qCDebug(motionCate()) << QString("Vacuum %1 disabled wait function!").arg(name());
        return;
    }

    ErrorHandler::tryToHandleError([&] {
        if (waitUntilTimeout(state))
        {
            return;
        }
        else
        {
            throw ActionError("Vacuum", QObject::tr("Wait vacuum %1 being %2 state timeout!").arg(name()).arg(state));
        }
    });
}

bool SCVacuum::waitUntilTimeout(bool state)
{
    return waitUntilTimeout(state, m_config->timeout());
}

bool SCVacuum::waitUntilTimeout(bool state, int timeout)
{
    if (isDisableSetAndWait)
    {
        qCDebug(motionCate()) << QString("Vacuum %1 disabled wait function!").arg(name());
        return true;
    }

    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (get() == state)
        {
            return true;
        }
        QThread::msleep(1);
        if (timer.elapsed() > timeout)
        {
            return false;
        }
    }
}

bool SCVacuum::checkFeedbackSignal(bool state)
{
    if (isDisableCheck)
    {
        qCDebug(motionCate()) << QString("Vacuum %1 disabled check function!").arg(name());
        return true;
    }
    return get() == state;
}

QVariantMap SCVacuum::getDetailState()
{
    QVariantMap detailState;
    detailState["feedbackInputState"] = feedbackInput->get(5);
    detailState["vacuumizeOutputState"] = vacuumizeOutput->get(5);
    detailState["hasBlowOutput"] = hasBlowOutput;
    detailState["blowOutputState"] = hasBlowOutput ? blowOutput->get(5) : false;
    return detailState;
}

void SCVacuum::setBlow(bool state)
{
    if (hasBlowOutput)
    {
        blowOutput->set(state);
    }
}
