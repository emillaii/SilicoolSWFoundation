#include "picontrolcard.h"

void PIControlCard::postInit()
{
    if(isConnect)
    {
        if(PI_IsConnected(m_controllerID))
        {
            qInfo() << "check has connected";
            return;
        }
        isConnect = false;
    }

    piCoreCfg = PICardConfigManager::getIns().getPICoreCfg();
    if(piCoreCfg->ip()== "" || piCoreCfg->port()== -1)
    {
        throw SilicolAbort("PI Hexapos ip or port error!", EX_LOCATION);
    }

    m_controllerID = PI_ConnectTCPIP(piCoreCfg->ip().toUtf8().data(), piCoreCfg->port());
    if(m_controllerID == -1)
    {
        throw SilicolAbort("open PI controller failed or it is already connected!", EX_LOCATION);
    }
    piCoreCfg->setControllerID(m_controllerID);
    qInfo() << tr("connect to PI successful!! control ID: %1").arg(m_controllerID);
    isConnect = true;
}

void PIControlCard::dispose()
{
    PI_CloseConnection ( m_controllerID );
}

int PIControlCard::getControllerID()
{
    return m_controllerID;
}

void PIControlCard::setCoord()
{
    QString coordName =piCoreCfg->coordName();
    QString coord = piCoreCfg->coord();

    if(!isConnect)
    {
        throw SilicolAbort("Please connect PI controller first!");
    }

    if(coordName == "")
    {
        throw SilicolAbort("coordName is null!");
    }
    if(!coord.contains(","))
    {
        throw SilicolAbort("coord com do not contains [,]!");
    }

    QStringList coordCom = coord.split(",");
    if(coordCom.count() != 6)
    {
        throw SilicolAbort("coord com values is not 6!");
    }
    double center[6] = {0,0,0,0,0,0};
    bool qstringCastToDoubleFlag = false;
    double castValue = -1;
    for (int i = 0; i< coordCom.count(); i++)
    {
        qstringCastToDoubleFlag = false;
        castValue = coordCom[i].toDouble(&qstringCastToDoubleFlag);
        if(!qstringCastToDoubleFlag)
        {
            throw SilicolAbort("coord cast QString to Double failed !");
            return;
        }
        center[i] = castValue;
    }

    if(!PI_KSD(m_controllerID, coordName.toUtf8(), "X Y Z U V W", center))
    {
        PI_KEN(m_controllerID, "DEMO");
    }
    else
    {
       PI_KEN(m_controllerID, coordName.toUtf8());
       return;
    }

    if(!PI_KSD(m_controllerID, coordName.toUtf8(), "X Y Z U V W", center))
    {
        qWarning()<< "perform PI_KSD creat a new coordinate failed!";
        return ;
    }

    PI_KEN(m_controllerID, coordName.toUtf8());
}

