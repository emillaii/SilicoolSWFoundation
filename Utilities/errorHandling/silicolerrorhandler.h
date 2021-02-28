#ifndef SILICOLERRORHANDLER_H
#define SILICOLERRORHANDLER_H

#include "../uiHelper/uioperation.h"
#include "errorreporter.h"
#include "silicolerror.h"

class UTILITIESSHARED_EXPORT ErrorHandler
{
public:
    template <typename T>
    static T tryToHandleGeneralError(std::function<T()> func, SilicolFunc actionBeforeRetry = nullptr)
    {
        while (true)
        {
            try
            {
                return func();
            }
            catch (GeneralError &ge)
            {
                QString errorHandlingOption = getErrorHandlingOption(ge);
                if (errorHandlingOption == Retry_)
                {
                    if (actionBeforeRetry != nullptr)
                    {
                        actionBeforeRetry();
                    }
                    continue;
                }
                else if (errorHandlingOption == Ignore_)
                {
                    return T();
                }
                else
                {
                    throw SilicolAbort(QObject::tr("User selected abort!"));
                }
            }
        }
    }

    static void tryToHandleError(SilicolFunc func, SilicolFunc actionBeforeRetry = nullptr);

    static QString getErrorHandlingOption(SilicolError &se);
};

#endif    // SILICOLERRORHANDLER_H
