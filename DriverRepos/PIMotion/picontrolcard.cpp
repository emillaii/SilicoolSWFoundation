#include "picontrolcard.h"

PIControlCard::PIControlCard(QObject *parent) : ControlCard(parent) {}

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

    PIControlCardConfig *piCoreCfg = PICardConfigManager::getIns().getPICoreCfg();

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

int PIControlCard::GetControllerID()
{
    return m_controllerID;
}

