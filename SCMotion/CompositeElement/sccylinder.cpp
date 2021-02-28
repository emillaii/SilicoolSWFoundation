#include "sccylinder.h"

SCCylinder::SCCylinder(QString name, QObject *parent) : QObject(parent)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<SCCylinder>(name, this);
}

void SCCylinder::setConfig(CylinderConfig *config)
{
    m_config = config;
}

void SCCylinder::init()
{
    if (m_config->falseOutput().isEmpty() && m_config->trueOutput().isEmpty())
    {
        throw SilicolAbort(tr("%1`s falseOutput and trueOutput can not be emtpy at the same time!").arg(name()));
    }

    isSingleOutputControl = m_config->falseOutput().isEmpty();
    trueOutput = MotionElementContainer::getIns()->getItem<SCDO>(m_config->trueOutput());
    if (!isSingleOutputControl)
    {
        falseOutput = MotionElementContainer::getIns()->getItem<SCDO>(m_config->falseOutput());
    }
    noFalseInputSNR = m_config->noFalseInputSNR();
    if (noFalseInputSNR)
    {
        if (m_config->delayAfterMoveToFalse() < 50)
        {
            throw SilicolAbort(tr("If false input sensor is not attached, delayAfterMoveToFalse must greater than 50! "
                                  "CylinderName: %1")
                                   .arg(m_config->name()));
        }
    }
    else
    {
        falseInput = MotionElementContainer::getIns()->getItem<SCDI>(m_config->falseInput());
    }
    noTrueInputSNR = m_config->noTrueInputSNR();
    if (noTrueInputSNR)
    {
        if (m_config->delayAfterMoveoToTrue() < 50)
        {
            throw SilicolAbort(tr("If true input sensor is not attached, delayAfterMoveoToTrue must greater than 50! "
                                  "CylinderName: %1")
                                   .arg(m_config->name()));
        }
    }
    else
    {
        trueInput = MotionElementContainer::getIns()->getItem<SCDI>(m_config->trueInput());
    }
}

SCCylinder::CylinderState SCCylinder::get()
{
    if (noFalseInputSNR || noTrueInputSNR)
    {
        CylinderState outputState = getOutputState();
        if (outputState == CylinderState::False)
        {
            if (!noFalseInputSNR && !falseInput->get())
            {
                return CylinderState::Moving;
            }
        }
        if (outputState == CylinderState::True)
        {
            if (!noTrueInputSNR && !trueInput->get())
            {
                return CylinderState::Moving;
            }
        }
        return outputState;
    }
    else
    {
        CylinderState outputState = getOutputState();
        CylinderState inputState = getInputState();
        if (outputState == CylinderState::False && inputState == CylinderState::False)
        {
            return CylinderState::False;
        }
        if (outputState == CylinderState::True && inputState == CylinderState::True)
        {
            return CylinderState::True;
        }
        return CylinderState::Moving;
    }
}

void SCCylinder::set(bool state, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(state, waitDone));

    if (checkState(state))
    {
        return;
    }

    SCTimer timer(QString("Set cylinder %1 to %2 state.").arg(name()).arg(state), motionCate(), waitDone);

    GlobalState::getIns().waitNotPaused(name());

    setImpl(state);
    if (waitDone)
    {
        wait(state);
    }
}

void SCCylinder::wait(bool state)
{
    ErrorHandler::tryToHandleError(
        [&] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (checkState(state))
                {
                    int delay = state ? m_config->delayAfterMoveoToTrue() : m_config->delayAfterMoveToFalse();
                    if (delay > 0)
                    {
                        QThread::msleep(delay);
                    }
                    return;
                }
                QThread::msleep(1);
                if (timer.elapsed() > m_config->timeout())
                {
                    throw ActionError("Cylinder", QObject::tr("Wait cylinder %1 being %2 state timeout!").arg(name()).arg(state));
                }
            }
        },
        [&] { setImpl(state); });
}

QVariantMap SCCylinder::getDetailState()
{
    QVariantMap detailState;
    detailState["cylState"] = get();
    detailState["falseInputState"] = noFalseInputSNR ? false : falseInput->get(5);
    detailState["trueInputState"] = noTrueInputSNR ? false : trueInput->get(5);
    detailState["isSingleOutputControl"] = isSingleOutputControl;
    detailState["falseOutputState"] = isSingleOutputControl ? false : falseOutput->get(5);
    detailState["trueOutputState"] = trueOutput->get(5);
    return detailState;
}

void SCCylinder::setFalseOutput(bool state)
{
    if (isSingleOutputControl)
    {
        return;
    }
    falseOutput->set(state);
}

void SCCylinder::setTrueOutput(bool state)
{
    trueOutput->set(state);
}

bool SCCylinder::checkState(bool targetState)
{
    CylinderState currentState = get();
    if ((currentState == True && targetState) || (currentState == False && !targetState))
    {
        return true;
    }
    return false;
}

void SCCylinder::setImpl(bool state)
{
    if (moveProtection != nullptr)
    {
        ErrorHandler::tryToHandleError([&] { moveProtection->checkCylIsAbleToMove(name(), state); });
    }

    if (isSingleOutputControl)
    {
        trueOutput->set(state);
    }
    else
    {
        if (state)
        {
            falseOutput->set(false);
            QThread::msleep(5);
            trueOutput->set(true);
        }
        else
        {
            trueOutput->set(false);
            QThread::msleep(5);
            falseOutput->set(true);
        }
    }
}

SCCylinder::CylinderState SCCylinder::getOutputState()
{
    if (isSingleOutputControl)
    {
        if (trueOutput->get())
        {
            return CylinderState::True;
        }
        else
        {
            return CylinderState::False;
        }
    }
    else
    {
        bool trueOutputState = trueOutput->get();
        bool falseOutputState = falseOutput->get();
        if (trueOutputState && !falseOutputState)
        {
            return CylinderState::True;
        }
        if (falseOutputState && !trueOutputState)
        {
            return CylinderState::False;
        }
        return CylinderState::Moving;
    }
}

SCCylinder::CylinderState SCCylinder::getInputState()
{
    bool falseInputState = falseInput->get();
    bool trueInputState = trueInput->get();
    if (falseInputState)
    {
        if (trueInputState)
        {
            qCritical() << "Both false input and true input were in actived state!"
                        << "Cylinder:" << name();
            return Moving;
        }
        else
        {
            return False;
        }
    }
    else
    {
        if (trueInputState)
        {
            return True;
        }
        else
        {
            return Moving;
        }
    }
}

void SCCylinder::startLoopTest()
{
    if (isLoopTest)
    {
        return;
    }
    isLoopTest = true;

    AutoResetBool b(&isLoopTest, false);
    while (isLoopTest)
    {
        set(true);
        set(false);
    }
}

void SCCylinder::stopLoopTest()
{
    isLoopTest = false;
}
