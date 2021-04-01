#include "detapermotionelementfactory.h"

SCAxis *DetaperMotionElementFactory::createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent)
{
    Q_UNUSED(axisType)
    return new GTAxis(name, parent);
}

SCDI *DetaperMotionElementFactory::createDI(QString name, QObject *parent)
{
    return new GTDI(name, parent);
}

SCDO *DetaperMotionElementFactory::createDO(QString name, QObject *parent)
{
    return new GTDO(name, parent);
}

ControlCard *DetaperMotionElementFactory::createControlCard()
{
    return new GTControlCard();
}

SCDispenser *DetaperMotionElementFactory::createDispenser(const QLoggingCategory &logCate,
                                                        DispenserConfig *dispenserConfig,
                                                        QObject *parent)
{
    return new DummyDispenser(logCate, dispenserConfig, parent);
}

IOConfig *DetaperMotionElementFactory::createIOConfig(QObject *parent)
{
    return new GTIOConfig(parent);
}

AxisConfig *DetaperMotionElementFactory::createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent)
{
    Q_UNUSED(axisType);
    return new GTAxisConfig(parent);
}

const QMetaObject *DetaperMotionElementFactory::ioConfigMetaObj() const
{
    return &GTIOConfig::staticMetaObject;
}

const QMetaObject *DetaperMotionElementFactory::axisConfigMetaObj() const
{
    return &GTAxisConfig::staticMetaObject;
}
