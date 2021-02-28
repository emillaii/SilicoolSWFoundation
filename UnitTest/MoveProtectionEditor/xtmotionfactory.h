#ifndef XTMOTIONFACTORY_H
#define XTMOTIONFACTORY_H

#include "BasicElement/basicelementfactory.h"
#include <QObject>

class XTMotionFactory : public BasicElementFactory
{
public:
    XTMotionFactory();

    // BasicElementFactory interface
public:
    virtual SCAxis *createAxis(QString name, QObject *parent) override
    {
        return nullptr;
    }
    virtual SCDI *createDI(QString name, QObject *parent) override
    {
        return nullptr;
    }
    virtual SCDO *createDO(QString name, QObject *parent) override
    {
        return nullptr;
    }
};

#endif    // XTMOTIONFACTORY_H
