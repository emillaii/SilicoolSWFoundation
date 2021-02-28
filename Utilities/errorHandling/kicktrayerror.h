#ifndef KICKTRAYERROR_H
#define KICKTRAYERROR_H

#include "silicolerror.h"
#include "utilities_global.h"

const QString ReKick = "ReKick";

class UTILITIESSHARED_EXPORT KickTrayError : public SilicolError
{
public:
    KickTrayError(const QString &moduleName, const QString &errMsg);

    // SilicolError interface
protected:
    virtual void implemented() override {}
};

#endif    // KICKTRAYERROR_H
