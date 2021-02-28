#include "silicolerrorhandler.h"

void ErrorHandler::tryToHandleError(SilicolFunc func, SilicolFunc actionBeforeRetry)
{
    while (true)
    {
        try
        {
            func();
            return;
        }
        catch (SilicolError &se)
        {
            qCritical() << "Catch error:" << se.what();

            QString errHandling = getErrorHandlingOption(se);
            if (errHandling == Abort_)
            {
                throw SilicolAbort("User selected abort!");
            }
            else if (errHandling == Ignore_)
            {
                return;
            }
            else if (errHandling == Retry_)
            {
                if (actionBeforeRetry != nullptr)
                {
                    actionBeforeRetry();
                }
                continue;
            }
            else
            {
                throw UnhandledError(se, errHandling);
            }
        }
    }
}

QString ErrorHandler::getErrorHandlingOption(SilicolError &se)
{
    if (!se.autoHandlingOption.isEmpty())
    {
        qInfo() << "Apply autoHandlingOption:" << se.autoHandlingOption;
        return se.autoHandlingOption;
    }
    else
    {
        QString title = se.moduleName + " Error";
        QString content = se.errorMsg;
        if (!se.suggestion.isEmpty())
        {
            content += "\r\nSuggestion: " + se.suggestion;
        }

        QString errId = ErrorReporter::getIns().reportError(se.moduleName, se.errorMsg);
        while (true)
        {
            QString errHandling
                = UIOperation::getIns()->getUIResponse(title, content, MsgBoxIcon::Error, se.errorHandlingOptions);
            if (errHandling == Ignore_
                && UIOperation::getIns()->getUIResponse(
                       "Please confirm",
                       QObject::tr("Detect that you try to ignore this error\r\n(%1)\r\nwhich may cause dangerous "
                                   "behavior.\r\nAre you sure to ignore this error?")
                           .arg(content),
                       MsgBoxIcon::Warning, YesNoBtns)
                       == NoBtn)
            {
                continue;
            }
            else
            {
                ErrorReporter::getIns().errorHandled(errId);
                return errHandling;
            }
        }
    }
}
