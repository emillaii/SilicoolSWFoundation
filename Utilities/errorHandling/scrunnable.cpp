#include "scrunnable.h"

void SCRunnable::reset()
{
    m_isStarted = false;
    m_isFinished = false;
    m_errMsg.clear();
}

void SCRunnable::run()
{
    m_isStarted = true;
    try
    {
        runImpl();
        m_isFinished = true;
    }
    catch (SilicoolException &se)
    {
        m_errMsg = se.what();
        m_isFinished = true;
    }
}
