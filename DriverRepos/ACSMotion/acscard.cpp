#include "acscard.h"

ACSCard::ACSCard(QObject *parent) : ControlCard(parent)
{

}

void ACSCard::postInit()
{
    if(isConnect)
    {
        qInfo() << "check has connected";
        return;
    }

    ACSCardConfig *acsCoreCfg = ACSCardConfigManage::getIns().getACSCardCfg();

    if(acsCoreCfg->ip()== "" || acsCoreCfg->port()== -1)
    {
        throw SilicolAbort("ACSC ip or port error!", EX_LOCATION);
    }

    m_handle = acsc_OpenCommEthernetTCP(acsCoreCfg->ip().toUtf8().data(), acsCoreCfg->port());
    if(m_handle == ACSC_INVALID)
    {
        throw SilicolAbort("ACSC controller failed or it is already connected!", EX_LOCATION);
    }
    acsCoreCfg->setHandle((int)m_handle);
    qInfo() << tr("connect to ACSC successful!! control ID: %1").arg((int)m_handle);
    isConnect = true;

    acsc_RunBuffer(m_handle, 1, NULL, NULL);
}

void ACSCard::dispose()
{
    acsc_CloseComm(m_handle);
}

int ACSCard::getHandle()
{
    return (int)m_handle;
}
