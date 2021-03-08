#include "pihexapod.h"

PIHexapod::PIHexapod()
{
//    controllerID = PI_ConnectTCPIP(ip.toUtf8().data(), port);
//    if(controllerID == -1)
//    {
//        throw SilicolAbort("open PI controller failed or it is already connected!", EX_LOCATION);
//    }
}


void PIHexapod::setRotationCenter(double *pos)
{
    BOOL ret = PI_KSD(controllerID, coordSystemName.toUtf8().data(), axisNameList.toUtf8().data(), pos);
    if (ret != 0)
    {
        throw ActionError(axisNameList, QString("PI_KSD() in setRotationCenter() failed!"));
    }
}

void PIHexapod::setConnectMsg(int id, bool connect)
{
    isConnected = connect;
    controllerID = id;
}
