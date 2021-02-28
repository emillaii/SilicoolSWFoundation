#ifndef DUMMYDO_H
#define DUMMYDO_H

#include "dummydi.h"
#include "scdo.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT DummyDO : public SCDO
{
    Q_OBJECT
public:
    explicit DummyDO(QString name, QObject *parent = nullptr);

    // SCDO interface
protected:
    virtual bool getImpl() noexcept override;
    virtual void setImpl(bool state) noexcept override;
    virtual void initImpl() override {}

private:
    bool currentValue = false;
};

#endif    // DUMMYDO_H
