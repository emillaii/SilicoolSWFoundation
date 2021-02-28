#ifndef XTDI_H
#define XTDI_H

#include "BasicElement/scdi.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include <QObject>

class XtDI : public SCDI
{
    Q_OBJECT

public:
    static int generateNewDiID();

    XtDI(QString name, QObject *parent = nullptr);

    XtDI(QString name, int id, QObject *parent = nullptr);

    // SCDI interface
protected:
    virtual bool getImpl() noexcept override;
    virtual void initImpl() override;

private:
    int id = -1;
    static int diCount;
    IOConfig *dummyConfig = nullptr;
};

#endif    // XTDI_H
