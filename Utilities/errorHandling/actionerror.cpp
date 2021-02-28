#include "actionerror.h"

ActionError::ActionError(const QString &moduleName, const QString &errorMsg, const QString &suggestion)
    : SilicolError(moduleName, errorMsg, suggestion)
{
    appendErrorHandlingOption(Retry_);
    appendErrorHandlingOption(Ignore_);
    appendErrorHandlingOption(Abort_);
}
