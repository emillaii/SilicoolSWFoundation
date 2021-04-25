#include "dummymotionelementfactory.h"

SCAxis *DummyMotionElementFactory::createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent)
{
    switch (axisType)
    {
        case AxisDefinition::GeneralAxis:
        {
            return new GTAxis(name, parent);
        }
    }
    qCritical() << "Unknown axis type: " << axisType;
    return nullptr;
}

SCDI *DummyMotionElementFactory::createDI(QString name, QObject *parent)
{
    return new GTDI(name, parent);
}

SCDO *DummyMotionElementFactory::createDO(QString name, QObject *parent)
{
    return new GTDO(name, parent);
}

ControlCard *DummyMotionElementFactory::createControlCard()
{
    return new GTControlCard();
}

IOConfig *DummyMotionElementFactory::createIOConfig(QObject *parent)
{
    return new GTIOConfig(parent);
}

AxisConfig *DummyMotionElementFactory::createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent)
{
    switch (axisType)
    {
        case AxisDefinition::GeneralAxis:
        {
            return new GTAxisConfig(parent);
        }
    }
    qCritical() << "Unknown axis type:" << axisType;
    return nullptr;
}

const QMetaObject *DummyMotionElementFactory::ioConfigMetaObj() const
{
    return &GTIOConfig::staticMetaObject;
}

const QMetaObject *DummyMotionElementFactory::axisConfigMetaObj() const
{
    return &GTAxisConfig::staticMetaObject;
}

SCDispenser *DummyMotionElementFactory::createDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent)
{
    return new GTDispenser(logCate, dispenserConfig, parent);
}
