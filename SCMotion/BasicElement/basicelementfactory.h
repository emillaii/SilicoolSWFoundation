#ifndef BASICELEMENTFACTORY_H
#define BASICELEMENTFACTORY_H

#include "CompositeElement/scdispenser.h"
#include "axisconfig.h"
#include "controlcard.h"
#include "motionelementdefinition.h"
#include "scaxis.h"
#include "scdi.h"
#include "scdo.h"
#include "scmotion_global.h"
#include "softlandingpos.h"
#include <QMetaObject>

class SCMOTIONSHARED_EXPORT BasicElementFactory
{
public:
    virtual ~BasicElementFactory() {}
    virtual SCAxis *createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent = nullptr) = 0;
    virtual SCDI *createDI(QString name, QObject *parent = nullptr) = 0;
    virtual SCDO *createDO(QString name, QObject *parent = nullptr) = 0;
    virtual ControlCard *createControlCard() = 0;
    virtual SCDispenser *createDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent = nullptr)
    {
        Q_UNUSED(logCate)
        Q_UNUSED(dispenserConfig)
        Q_UNUSED(parent)
        qCCritical(motionCate()) << "create null dispenser...";
        return nullptr;
    }
    virtual IOConfig *createIOConfig(QObject *parent = nullptr)
    {
        return new IOConfig(parent);
    }
    virtual AxisConfig *createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent = nullptr)
    {
        Q_UNUSED(axisType)
        return new AxisConfig(parent);
    }
    virtual const QMetaObject *ioConfigMetaObj() const
    {
        return &IOConfig::staticMetaObject;
    }
    virtual const QMetaObject *axisConfigMetaObj() const
    {
        return &AxisConfig::staticMetaObject;
    }
    virtual const QMetaObject *extendedAxisConfigMetaObj() const
    {
        return &AxisConfig::staticMetaObject;
    }
    virtual const QMetaObject *extendedAxis2ConfigMetaObj() const
    {
        return &AxisConfig::staticMetaObject;
    }
    virtual const QMetaObject *extendedAxis3ConfigMetaObj() const
    {
        return &AxisConfig::staticMetaObject;
    }
    virtual const QMetaObject *extendedAxis4ConfigMetaObj() const
    {
        return &AxisConfig::staticMetaObject;
    }
};

#endif    // BASICELEMENTFACTORY_H
