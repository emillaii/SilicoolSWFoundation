#ifndef DUMMYMOTIONELEMENTFACTORY_H
#define DUMMYMOTIONELEMENTFACTORY_H

#include "BasicElement/basicelementfactory.h"
#include "BasicElement/dummyaxis.h"
#include "BasicElement/dummycontrolcard.h"
#include "BasicElement/dummydi.h"
#include "BasicElement/dummydo.h"
#include "CompositeElement/dummydispenser.h"

class DummyMotionElementFactory : public BasicElementFactory
{
public:
    DummyMotionElementFactory() {}

    // BasicElementFactory interface
public:
    virtual SCAxis *createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent) override;
    virtual SCDI *createDI(QString name, QObject *parent) override;
    virtual SCDO *createDO(QString name, QObject *parent) override;
    virtual ControlCard *createControlCard() override;
    virtual SCDispenser *
    createDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent) override;
    virtual IOConfig *createIOConfig(QObject *parent) override;
    virtual AxisConfig *createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent) override;
    virtual const QMetaObject *ioConfigMetaObj() const override;
    virtual const QMetaObject *axisConfigMetaObj() const override;
};

#endif    // DUMMYMOTIONELEMENTFACTORY_H
