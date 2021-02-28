#ifndef DUMMYMOTIONFACTORY_H
#define DUMMYMOTIONFACTORY_H

#include "BasicElement/basicelementfactory.h"

class DummyMotionElementFactory : public BasicElementFactory
{

    // BasicElementFactory interface
public:
    virtual SCAxis *createAxis(QString name, AxisDefinition::AxisType axisType, QObject *parent) override
    {
        Q_UNUSED(name)
        Q_UNUSED(axisType)
        Q_UNUSED(parent)
        return nullptr;
    }
    virtual SCDI *createDI(QString name, QObject *parent) override
    {
        Q_UNUSED(name)
        Q_UNUSED(parent)
        return nullptr;
    }
    virtual SCDO *createDO(QString name, QObject *parent) override
    {
        Q_UNUSED(name)
        Q_UNUSED(parent)
        return nullptr;
    }

    virtual ControlCard *createControlCard() override
    {
        return nullptr;
    }
};

#endif    // DUMMYMOTIONFACTORY_H
