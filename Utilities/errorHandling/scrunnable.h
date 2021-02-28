#ifndef SCRUNNABLE_H
#define SCRUNNABLE_H

#include "silicolerror.h"
#include "utilities_global.h"
#include <QRunnable>

class UTILITIESSHARED_EXPORT SCRunnable : public QRunnable
{
public:
    void reset();

    bool isStarted() const
    {
        return m_isStarted;
    }
    bool isFinished() const
    {
        return m_isFinished;
    }
    QString errMsg() const
    {
        return m_errMsg;
    }

    virtual void runImpl() = 0;

    // QRunnable interface
public:
    virtual void run() override;

private:
    bool m_isStarted = false;
    bool m_isFinished = false;
    QString m_errMsg;
};

#endif    // SCRUNNABLE_H
