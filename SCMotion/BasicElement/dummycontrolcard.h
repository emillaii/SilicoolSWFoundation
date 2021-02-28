#ifndef DUMMYCONTROLCARD_H
#define DUMMYCONTROLCARD_H

#include "controlcard.h"

class SCMOTIONSHARED_EXPORT DummyControlCard : public ControlCard
{
    Q_OBJECT

public:
    DummyControlCard(QObject *parent = nullptr) : ControlCard(parent) {}

    // ControlCard interface
public:
    virtual void preInit() override {}
    virtual void postInit() override {}
    virtual void dispose() override {}
};

#endif    // DUMMYCONTROLCARD_H
