#include "picktrayduterror.h"

PickTrayDutError::PickTrayDutError(QString moduleName, QString errorMsg, QString suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(Retry_);
    appendErrorHandlingOption(Skip_);
}

PickTrayDutErrorHandler::PickTrayDutErrorHandler(SilicolFunc skipHandling) : skipHandling(skipHandling) {}

bool PickTrayDutErrorHandler::handleCustomError(QString errHandling)
{
    if (errHandling == Skip_)
    {
        skipHandling();
        return true;
    }
    return false;
}
