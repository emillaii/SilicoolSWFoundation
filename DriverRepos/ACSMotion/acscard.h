#ifndef ACSCARD_H
#define ACSCARD_H

#include "BasicElement/controlcard.h"
#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "errorHandling/silicolerror.h"
#include "loghelper.h"
#include "loging/Logger.h"
#include "motionutility.h"
#include "processkiller.h"
#include <QtConcurrent>
#include "ACSC.h"
#include "acscardconfig.h"

class ACSCard : public ControlCard
{
    Q_OBJECT
public:
    ACSCard(QObject *parent = nullptr);

public:
    virtual void preInit() override {}
    virtual void postInit() override;
    virtual void dispose() override;
    int getHandle();
private:
    bool isConnect = false;
    HANDLE m_handle = ACSC_INVALID;
};

#endif // ACSCARD_H
