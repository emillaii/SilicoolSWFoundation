#ifndef GTDO_H
#define GTDO_H

#include "BasicElement/scdo.h"
#include "gtioconfig.h"
#include "gtresulthandler.h"
#include "gts.h"
#include <QMutex>

class GTDO : public SCDO
{
    Q_OBJECT
public:
    GTDO(QString name, QObject *parent = nullptr);
    GTIOConfig *getGtioConfig() const;

    // SCDO interface
protected:
    virtual bool getImpl() noexcept override;
    virtual void setImpl(bool state) noexcept override;
    virtual void initImpl() override;

private:
    static QMutex locker;
    GTIOConfig *gtioConfig = nullptr;
};

#endif    // GTDO_H
