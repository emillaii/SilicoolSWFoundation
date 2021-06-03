#ifndef PICONTROLCARD_H
#define PICONTROLCARD_H

#include "BasicElement/controlcard.h"
#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "errorHandling/silicolerror.h"
#include "loghelper.h"
#include "loging/Logger.h"
#include "motionutility.h"
#include "processkiller.h"
#include <QtConcurrent>
#include "PI_GCS2_DLL.h"
#include "picontrolcardconfig.h"

class PIControlCard : public ControlCard
{
    Q_OBJECT
private:
    PIControlCard(QObject *parent = nullptr):ControlCard (parent){}

public:
    static PIControlCard *getIns()
    {
        static PIControlCard instance;
        return &instance;
    }

public:
    virtual void preInit() override {}
    virtual void postInit() override;
    virtual void dispose() override;
    int getControllerID();
public:
    Q_INVOKABLE void setCoord();
private:
    bool isConnect = false;
    int m_controllerID = -1;
    PIControlCardConfig *piCoreCfg;
};

#endif // PICONTROLCARD_H
