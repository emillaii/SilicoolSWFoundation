#ifndef GTRESULTHANDLER_H
#define GTRESULTHANDLER_H

#include "errorHandling/silicolerror.h"
#include "gts.h"
#include "motionutility.h"

#define checkResult1(calling)                                                                                          \
    {                                                                                                                  \
        short res = calling;                                                                                           \
        if (res != CMD_SUCCESS)                                                                                        \
        {                                                                                                              \
            throw SilicolAbort(QString("%1 failed! Error code: %2").arg(#calling).arg(res), EX_LOCATION);              \
        }                                                                                                              \
    }

#define checkResult2(calling)                                                                                          \
    {                                                                                                                  \
        short res = calling;                                                                                           \
        if (res != CMD_SUCCESS)                                                                                        \
        {                                                                                                              \
            throw GeneralError("GTMotion", QString("%1 failed! Error code: %2").arg(#calling).arg(res));               \
        }                                                                                                              \
    }

#define printError(res, errMsg)                                                                                        \
    if (res != CMD_SUCCESS)                                                                                            \
    {                                                                                                                  \
        qCCritical(motionCate()) << errMsg << "Error code:" << res;                                                    \
    }

#endif    // GTRESULTHANDLER_H
