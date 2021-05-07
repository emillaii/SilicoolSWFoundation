#ifndef STOPPEDERROR_H
#define STOPPEDERROR_H

#include "silicolerror.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT StoppedError : public SilicolError
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
