#include "ignorecondition.h"

IgnoreCollisionCondition *IgnoreCollisionCondition::createInstance(IgnoreCollisionCondition::Type type, QObject *parent)
{
    switch (type)
    {
        case Axis:
            return new AxisIgnoreCollisionCondition(parent);
        case Cylinder:
            return new CylIgnoreCollisionCondition(parent);
    }
    qCritical() << "Unknown IgnoreCollisionCondition::Type:" << type;
    return nullptr;
}
