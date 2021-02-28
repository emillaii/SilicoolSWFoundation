#ifndef SCASSERT_H
#define SCASSERT_H

#include "silicolerror.h"

#define SC_ASSERT(cond)                                                                                                                              \
    if (!(cond))                                                                                                                                     \
    {                                                                                                                                                \
        throw SilicolAbort(QString("Assert failed!\r\n%1\r\n%2, line %3, function: %4").arg(#cond).arg(__FILE__).arg(__LINE__).arg(__FUNCTION__));   \
    }

#endif    // SCASSERT_H
