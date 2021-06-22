#include "acsdo.h"

bool ACSDO::getImpl()noexcept
{
    int value = 0;
    HANDLE acscHandle = (HANDLE)ACSCardConfigManage::getIns().getACSCardCfg()->handle();
    checkACSCom(acsc_GetOutput(acscHandle, 0, 0, &value, NULL), "acsc_GetOutput");
    return value;
}

void ACSDO::setImpl(bool state)noexcept
{
    HANDLE acscHandle = (HANDLE)ACSCardConfigManage::getIns().getACSCardCfg()->handle();
    checkACSCom(acsc_SetOutput(acscHandle, 0, 0, state ? 1 : 0, NULL), "acsc_SetOutput");
}
