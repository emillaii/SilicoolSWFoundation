#include "silicolerror.h"

void scTerminate()
{
    ExceptionInfoManager::getIns().outputUnprintedException();
    abort();
}

ExceptionInfo::ExceptionInfo(const QString &exceptionId, const QString &exceptionMsg)
    : exceptionId(exceptionId), exceptionMsg(exceptionMsg), hasPrintMsg(false), instanceCount(1)
{
}

void ExceptionInfoManager::exceptionDestructed(const QString &exceptionId)
{
    ExceptionInfo *exInfo = &(exceptionInfos[exceptionId]);
    exInfo->instanceCount--;
    if (exInfo->instanceCount <= 0)
    {
        if (!exInfo->hasPrintMsg)
        {
            qCritical() << exInfo->exceptionMsg;
        }
        exceptionInfos.remove(exceptionId);
    }
}

void ExceptionInfoManager::outputUnprintedException()
{
    foreach (const ExceptionInfo &exInfo, exceptionInfos.values())
    {
        if (!exInfo.hasPrintMsg)
        {
            qCritical() << exInfo.exceptionMsg;
        }
    }
}

SilicolError::SilicolError(const QString &moduleName, const QString &errorMsg, const QString &suggestion)
    : SilicoolException(moduleName + " Error. Error message: " + errorMsg),
      moduleName(moduleName),
      errorMsg(errorMsg),
      suggestion(suggestion)
{
}

void SilicolError::appendErrorHandlingOption(const QString &option)
{
    errorHandlingOptions.append(option);
}

SilicolAbort::SilicolAbort(const QString &abortMsg, const QString &location)
    : SilicoolException(location.isEmpty() ? abortMsg : QString("%1\r\nLocation:%2").arg(abortMsg).arg(location))
{
}
