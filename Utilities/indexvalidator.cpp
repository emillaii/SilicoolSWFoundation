#include "indexvalidator.h"

void validateIndex(int &index, int maxValue, const QString &errIndexDescription)
{
    if (qAbs(index) > maxValue)
    {
        throw SilicolAbort(QString("%1. Given index: %2, Valid index: [-%3, -1] or [1, %3]").arg(errIndexDescription).arg(index).arg(maxValue));
    }
    if (index < 0)
    {
        index += maxValue;
    }
    else if (index > 0)
    {
        index--;
    }
}
