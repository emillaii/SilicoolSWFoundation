#include "gtcontrolcard.h"

GTControlCard::GTControlCard(QObject *parent) : ControlCard(parent) {}

void GTControlCard::postInit()
{
    auto gtCardConfig = GTCardCfgManager::getIns().getGtCardCfg();
    if (!isOpen)
    {
        checkResult1(GTN_Open(5, gtCardConfig->cardOpenMode()));
        isOpen = true;
    }
    auto *gtCoreCfgs = gtCardConfig->gtCoreCfgs();
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
}

void GTControlCard::dispose()
{
    GTN_Close();
}
