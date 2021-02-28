#ifndef PICKTRAYDUTERROR_H
#define PICKTRAYDUTERROR_H

#include "silicolerror.h"

class PickTrayDutError : public SilicolError
{
public:
    PickTrayDutError(QString moduleName, QString errorMsg, QString suggestion = "");
};

class PickTrayDutErrorHandler : public ErrorHandler
{

public:
    PickTrayDutErrorHandler(SilicolFunc skipHandling);

    // ErrorHandler interface
protected:
    virtual bool handleCustomError(QString errHandling) override;

private:
    SilicolFunc skipHandling;
};

#endif    // PICKTRAYDUTERROR_H
