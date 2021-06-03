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

    if(!setCoord(piCoreCfg->coordName(), piCoreCfg->coord()))
    {
        throw SilicolAbort("Set PI coord failed, please check command!", EX_LOCATION);
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

bool PIControlCard::setCoord(QString coordName, QString coord)
{
    if(coordName == "")
    {
        qWarning()<< "coordName is null!";
        return false;
    }
    if(!coord.contains(","))
    {
        qWarning()<< "coord com do not contains [,]!";
        return false;
    }

    QStringList coordCom = coord.split(",");
    if(coordCom.count() != 6)
    {
        qWarning()<< "coord com values is not 6!";
        return false;
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
            qWarning()<< "coord cast QString to Double failed! QString: "<< coordCom[i];
            return false;
        }
        center[i] = castValue;
    }

    if(!PI_KSD(m_controllerID, coordName.toUtf8(), "X Y Z U V W", center))
    {
        qWarning()<< "perform PI_KSD creat a new coordinate failed!";
        return false;
    }

    return PI_KEN(m_controllerID, coordName.toUtf8());
}

