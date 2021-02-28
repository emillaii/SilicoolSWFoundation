#ifndef XTDO_H
#define XTDO_H

#include "BasicElement/scdo.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include <QObject>

class XtDO : public SCDO
{
    Q_OBJECT
public:
    explicit XtDO(QString name, QObject *parent = nullptr);

    explicit XtDO(QString name, int id, QObject *parent = nullptr);

    int getId() const;

    // SCDO interface
protected:
    virtual void initImpl() override;

    virtual bool getImpl() noexcept override;

    virtual void setImpl(bool state) noexcept override;

private:
    int id = -1;
    IOConfig *dummyConfig = nullptr;
};

#endif    // XTDO_H
