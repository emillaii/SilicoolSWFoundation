#ifndef DUMMYMOTIONELEMENTFACTORY_H
#define DUMMYMOTIONELEMENTFACTORY_H

#include "../DriverRepos/GTMotion/gtaxis.h"
#include "../DriverRepos/GTMotion/gtcontrolcard.h"
#include "../DriverRepos/GTMotion/gtdi.h"
#include "../DriverRepos/GTMotion/gtdispenser.h"
#include "../DriverRepos/GTMotion/gtdo.h"
#include "BasicElement/basicelementfactory.h"

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
    virtual IOConfig *createIOConfig(QObject *parent) override;
    virtual AxisConfig *createAxisConfig(AxisDefinition::AxisType axisType, QObject *parent) override;
    virtual const QMetaObject *ioConfigMetaObj() const override;
    virtual const QMetaObject *axisConfigMetaObj() const override;
    virtual SCDispenser *createDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent) override;
};

#endif    // DUMMYMOTIONELEMENTFACTORY_H
