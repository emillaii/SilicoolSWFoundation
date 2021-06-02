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

    if(!setKSD(piCoreCfg->ksd()))
    {
        throw SilicolAbort("KSD command error,please check!", EX_LOCATION);
    }
}

void PIControlCard::dispose()
{
    PI_CloseConnection ( m_controllerID );
}

int PIControlCard::getControllerID()
{
    return m_controllerID;
}

bool PIControlCard::setKSD(QString ksd)
{
    if(!ksd.contains(","))
    {
        qWarning()<< "ksd com do not contains [,]!";
        return false;
    }

    QStringList ksdCom = ksd.split(",");
    if(ksdCom.count() != 6)
    {
        qWarning()<< "ksd com values is not 6!";
        return false;
    }
    double center[6] = {0,0,0,0,0,0};
    bool qstringCastToDoubleFlag = false;
    double castValue = -1;
    for (int i = 0; i< ksdCom.count(); i++)
    {
        qstringCastToDoubleFlag = false;
        castValue = ksdCom[i].toDouble(&qstringCastToDoubleFlag);
        if(!qstringCastToDoubleFlag)
        {
            qWarning()<< "ksd cast QString to Double failed! QString: "<< ksdCom[i];
        }
        center[i] = castValue;
    }
    return  PI_KSD(m_controllerID, "SCAOA_CoordSystem", "X Y Z U V W", center) == 0;
}

