#ifndef INDEXVALIDATOR_H
#define INDEXVALIDATOR_H

#include "errorHandling/silicolerror.h"
#include "utilities_global.h"
#include <QObject>

UTILITIESSHARED_EXPORT void validateIndex(int &index, int maxValue, const QString &errIndexDescription);

#endif    // INDEXVALIDATOR_H
