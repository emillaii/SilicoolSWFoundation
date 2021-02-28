#include "comparisonoperation.h"

ComparisonOperation *ComparisonOperation::createInstance(ComparisonOperation::Type type, QObject *parent)
{
    switch (type)
    {
        case LessThanPos:
        {
            return new LessThanPosCO(parent);
        }
        case GreaterThanPos:
        {
            return new GreaterThanPosCO(parent);
        }
        case InPosRange:
        {
            return new InPosRangeCO(parent);
        }
        case NotInPosRange:
        {
            return new NotInPosRangeCO(parent);
        }
        case NotInState:
        {
            return new NotInStateCO(parent);
        }
    }
    qCritical() << "Unknown ComparisonOperation::Type:" << type;
    return nullptr;
}
