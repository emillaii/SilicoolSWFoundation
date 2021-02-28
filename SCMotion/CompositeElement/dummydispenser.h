#ifndef DUMMYDISPENSER_H
#define DUMMYDISPENSER_H

#include "scdispenser.h"

class SCMOTIONSHARED_EXPORT DummyDispenser : public SCDispenser
{
    Q_OBJECT
public:
    DummyDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent = nullptr)
        : SCDispenser(logCate, dispenserConfig, parent)
    {
    }

    // SCDispenser interface
protected:
    virtual void enterInterpolationMode() override {}
    virtual void exitInterpolationMode() override {}
    virtual bool isFinished() override
    {
        return true;
    }
    virtual void executePath(QVector<PathEndPoint> &path) override
    {
        Q_UNUSED(path)
    }
};

#endif    // DUMMYDISPENSER_H
