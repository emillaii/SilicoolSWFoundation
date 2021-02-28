#ifndef MOTIONUTILITY_H
#define MOTIONUTILITY_H

#include "commonmethod.h"
#include "errorHandling/actionerror.h"
#include "loging/Logger.h"
#include "scmotion_global.h"

SILICOOL_DECLARE_LOGGING_CATEGORY(motionCate, SCMOTIONSHARED_EXPORT)

class SCMOTIONSHARED_EXPORT MotionElement : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        Axis,
        DI,
        DO,
        Cylinder,
        Vacuum,

        // Axis module
        SingleAxis,
        XYModule,
        XYZModule
    };
    Q_ENUM(Type)

    static const EnumHelper<Type> &TypeEnumInfo()
    {
        static const EnumHelper<Type> ins(staticMetaObject, "Type");
        return ins;
    }
};

inline QString nominate(MotionElement::Type type, QString name)
{
    return MotionElement::TypeEnumInfo().enumToName(type) + QString("_") + name;
}

#endif    // MOTIONUTILITY_H
