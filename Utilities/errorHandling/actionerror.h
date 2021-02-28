#ifndef ACTIONERROR_H
#define ACTIONERROR_H

#include "silicolerrorhandler.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT ActionError : public SilicolError
{
public:
    ActionError(const QString &moduleName, const QString &errorMsg, const QString &suggestion = "");

    // SilicolError interface
protected:
    virtual void implemented() override {}
};

#endif    // ACTIONERROR_H
