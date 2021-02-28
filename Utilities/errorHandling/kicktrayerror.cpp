#include "kicktrayerror.h"

KickTrayError::KickTrayError(const QString &moduleName, const QString &errMsg) : SilicolError(moduleName, errMsg)
{
    appendErrorHandlingOption(Retry_);
    appendErrorHandlingOption(ReKick);
    appendErrorHandlingOption(Abort_);
}
