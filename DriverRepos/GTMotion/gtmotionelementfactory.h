#ifndef GTMOTIONELEMENTFACTORY_H
#define GTMOTIONELEMENTFACTORY_H

#include "../PACMotion/pacvcm.h"
#include "BasicElement/basicelementfactory.h"
#include "gtaxis.h"
#include "gtcontrolcard.h"
#include "gtdi.h"
#include "gtdispenser.h"
#include "gtdo.h"

class GTMotionElementFactory : public BasicElementFactory
{
public:
    GTMotionElementFactory() {}

    // BasicElementFactory interface
public:
    virtual SCAxis *createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent) override;
    virtual SCDI *createDI(QString name, QObject *parent) override;
    virtual SCDO *createDO(QString name, QObject *parent) override;
    virtual ControlCard *createControlCard() override;
    virtual IOConfig *createIOConfig(QObject *parent) override;
    virtual AxisConfig *createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent) override;
    virtual const QMetaObject *ioConfigMetaObj() const override;
    virtual const QMetaObject *axisConfigMetaObj() const override;
    virtual const QMetaObject *extendedAxis3ConfigMetaObj() const override;
    virtual SCDispenser *
    createDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent) override;
};

#endif    // GTMOTIONELEMENTFACTORY_H
