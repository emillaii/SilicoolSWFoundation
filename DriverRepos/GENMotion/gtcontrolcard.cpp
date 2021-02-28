#include "gtcontrolcard.h"

GTControlCard::GTControlCard(QObject *parent) : ControlCard(parent) {}

void GTControlCard::postInit()
{
    if (!isOpen)
    {
        checkResult1(GTN_Open(5, 1));
        isOpen = true;
    }

    if(!isInitEcat)
    {
        checkResult1(GTN_InitEcatComm(EthercatCore));
        if(!silicoolWait(5000, [this]{
            short connState;
            GTN_IsEcatReady(EthercatCore, &connState);
            return connState == 1;
        }))
        {
            throw SilicolAbort(tr("Wait ethercat communication ready timeout!"));
        }
        isInitEcat = true;
    }

    checkResult1(GTN_StartEcatComm(EthercatCore));

    ConfigObjectArray *gtCoreCfgs = GTCoreCfgManager::getIns().getGtCoreCfgs();
    for (int i = 0; i < gtCoreCfgs->count(); i++)
    {
        GTCoreConfig *gtCoreCfg = gtCoreCfgs->getConfig<GTCoreConfig>(i);
        if (!QFile::exists(gtCoreCfg->setupFileName()))
        {
            throw SilicolAbort(tr("Core config file not exist: %1").arg(gtCoreCfg->setupFileName()));
        }
        short coreNo = i + 1;
        checkResult1(GTN_Reset(coreNo));
        checkResult1(GTN_LoadConfig(coreNo, gtCoreCfg->setupFileName().toUtf8().data()));
        checkResult1(GTN_SetResCount(coreNo, MC_AXIS, gtCoreCfg->axisCount()));
        checkResult1(GTN_SetResCount(coreNo, MC_PULSE, gtCoreCfg->axisCount()));
        checkResult1(GTN_SetResCount(coreNo, MC_GPI, gtCoreCfg->diCount()));
        checkResult1(GTN_SetResCount(coreNo, MC_GPO, gtCoreCfg->doCount()));
        checkResult1(GTN_ClrSts(coreNo, 1, gtCoreCfg->axisCount()));
    }

    checkResult1(GTN_EcatIOSynch(1));
    short res = GTN_EcatIOSynch(2);
    printError(res, "GTN_EcatIOSynch(2)");
}

void GTControlCard::dispose()
{
    GTN_TerminateEcatComm(EthercatCore);
    GTN_Close();
}
