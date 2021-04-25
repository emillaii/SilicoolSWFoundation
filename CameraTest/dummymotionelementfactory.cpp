#include "dummymotionelementfactory.h"

SCAxis *DummyMotionElementFactory::createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent)
{
    Q_UNUSED(axisType)
    return new DummyAxis(name, parent);
}

SCDI *DummyMotionElementFactory::createDI(QString name, QObject *parent)
{
    return new DummyDI(name, parent);
}

SCDO *DummyMotionElementFactory::createDO(QString name, QObject *parent)
{
    return new DummyDO(name, parent);
}

ControlCard *DummyMotionElementFactory::createControlCard()
{
    return new DummyControlCard();
}

SCDispenser *DummyMotionElementFactory::createDispenser(const QLoggingCategory &logCate,
                                                        DispenserConfig *dispenserConfig,
                                                        QObject *parent)
{
    return new DummyDispenser(logCate, dispenserConfig, parent);
}

IOConfig *DummyMotionElementFactory::createIOConfig(QObject *parent)
{
    return new DummyIOConfig(parent);
}

AxisConfig *DummyMotionElementFactory::createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent)
{
    Q_UNUSED(axisType);
    return new AxisConfig(parent);
}

const QMetaObject *DummyMotionElementFactory::ioConfigMetaObj() const
{
    return &DummyIOConfig::staticMetaObject;
}

const QMetaObject *DummyMotionElementFactory::axisConfigMetaObj() const
{
    return &AxisConfig::staticMetaObject;
}
