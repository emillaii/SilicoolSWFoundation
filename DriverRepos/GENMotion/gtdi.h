#ifndef GTDI_H
#define GTDI_H

#include "BasicElement/scdi.h"
#include "gtioconfig.h"
#include "gtresulthandler.h"
#include "gts.h"

class GTDI : public SCDI
{
    Q_OBJECT
public:
    GTDI(QString name, QObject *parent = nullptr);

    // SCDI interface
protected:
    virtual bool getImpl() noexcept override;
    virtual void initImpl() override;

private:
    GTIOConfig *gtioConfig = nullptr;
};

#endif    // GTDI_H
