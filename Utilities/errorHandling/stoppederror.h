#ifndef STOPPEDERROR_H
#define STOPPEDERROR_H

#include "silicolerror.h"

class StoppedError : public SilicolError
{
public:
    StoppedError(const QString &workerName) : SilicolError(workerName, "Stopped")
    {
        appendErrorHandlingOption("Yes");
    }

    // SilicolError interface
protected:
    void implemented() {}
};

#endif    // STOPPEDERROR_H
