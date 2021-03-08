#ifndef SCRUNNABLE_H
#define SCRUNNABLE_H

#include "silicolerror.h"
#include "utilities_global.h"
#include <QRunnable>

class UTILITIESSHARED_EXPORT SCRunnable : public QRunnable
{
public:
    void reset();
    void cancel();

    bool isStarted() const
    {
        return m_isStarted;
    }
    bool isFinished() const
    {
        return m_isFinished;
    }
    bool isCanceled() const
    {
        return m_isCanceled;
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
    bool m_isCanceled = false;
    QString m_errMsg;
};

#endif    // SCRUNNABLE_H
