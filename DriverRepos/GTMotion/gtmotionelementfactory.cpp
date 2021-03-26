#include "gtmotionelementfactory.h"

SCAxis *GTMotionElementFactory::createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent)
{
    switch (axisType)
    {
        case AxisDefinition::GeneralAxis:
        {
            return new GTAxis(name, parent);
        }
        case AxisDefinition::ExtendedAxis3:
        {
            return new PACVcm(name, parent);
        }
    }
    qCritical() << "Unknown axis type: " << axisType;
    return nullptr;
}

SCDI *GTMotionElementFactory::createDI(QString name, QObject *parent)
{
    return new GTDI(name, parent);
}

SCDO *GTMotionElementFactory::createDO(QString name, QObject *parent)
{
    return new GTDO(name, parent);
}

ControlCard *GTMotionElementFactory::createControlCard()
{
    return new GTControlCard();
}

IOConfig *GTMotionElementFactory::createIOConfig(QObject *parent)
{
    return new GTIOConfig(parent);
}

AxisConfig *GTMotionElementFactory::createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent)
{
    switch (axisType)
    {
        case AxisDefinition::GeneralAxis:
        {
            return new GTAxisConfig(parent);
        }
        case AxisDefinition::ExtendedAxis3:
        {
            return new PACVcmConfig(parent);
        }
    }
    qCritical() << "Unknown axis type:" << axisType;
    return nullptr;
}

const QMetaObject *GTMotionElementFactory::ioConfigMetaObj() const
{
    return &GTIOConfig::staticMetaObject;
}

const QMetaObject *GTMotionElementFactory::axisConfigMetaObj() const
{
    return &GTAxisConfig::staticMetaObject;
}

const QMetaObject *GTMotionElementFactory::extendedAxis3ConfigMetaObj() const
{
    return &PACVcmConfig::staticMetaObject;
}

SCDispenser *GTMotionElementFactory::createDispenser(const QLoggingCategory &logCate,
                                                     DispenserConfig *dispenserConfig,
                                                     QObject *parent)
{
    return new GTDispenser(logCate, dispenserConfig, parent);
}
