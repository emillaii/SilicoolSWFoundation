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
public:
    PIControlCard(QObject *parent = nullptr);

public:
    virtual void preInit() override {}
    virtual void postInit() override;
    virtual void dispose() override;
    int GetControllerID();
private:
    bool isConnect = false;
    int m_controllerID = -1;
};

#endif // PICONTROLCARD_H
