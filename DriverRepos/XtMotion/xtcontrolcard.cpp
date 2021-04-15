#include "xtcontrolcard.h"

XtControlCard::XtControlCard()
{
    controlCardConfig = new XtControlCardConfig(this);
    controlCardConfigFile = new ConfigFile("XtControlCard", controlCardConfig, controlCardConfigFileName, false);
}

XtControlCard::~XtControlCard()
{
    delete controlCardConfigFile;
    delete controlCardConfig;
}

void XtControlCard::preInit()
{
    qCInfo(motionCate(), FUNC);

    controlCardConfigFile->populate(true);
    if (XT_Controler_Extend::Profile_Load(const_cast<LPWSTR>(controlCardConfig->profileName().toStdWString().c_str()))
        != 1)
    {
        throw SilicolAbort(QObject::tr("Load profile failed! %1").arg(controlCardConfig->profileName()), EX_LOCATION);
    }
}

void XtControlCard::postInit()
{
    bool executed = false;
    QString errMsg;
    QtConcurrent::run([&] {
        try
        {
            postInitImpl();
        }
        catch (SilicoolException &se)
        {
            errMsg = se.what();
        }
        executed = true;
    });
    silicoolWait(-1, [&executed] { return executed; });
    if (!errMsg.isEmpty())
    {
        throw SilicolAbort(errMsg);
    }
}

void XtControlCard::dispose()
{
    qCInfo(motionCate(), FUNC);

//    killProcess(QCoreApplication::applicationPid());
//    abort();

    XtVcm::releaseAllVcm();

    if (isInit)
    {
        //  关不掉
        XT_Controler_Extend::Profile_DeInit_Controller();
        XT_Controler::CloseMotionControlerServer();
        XT_Controler::ReleaseDevice();
        isInit = false;
    }
}

void XtControlCard::postInitImpl()
{
    qCInfo(motionCate(), FUNC);

    if (isInit)
    {
        qCInfo(motionCate(), "Control card has been inited.");
        return;
    }

    XT_Controler::InitDevice_PC_Local_Controler(
        controlCardConfig->startControllerServer(),
        const_cast<LPWSTR>(controlCardConfig->controllerServerDir().toStdWString().c_str()));
    //    XT_Controler::SetConnectServerTimeOutTime(3000);
    if (XT_Controler::beCurConnectServerAndInterfaceBoard() != 1)
    {
        if (XT_Controler::ConnectControlServer(
                const_cast<LPWSTR>(controlCardConfig->controllerIP().toStdWString().c_str()), 0)
            != 1)
        {
            throw SilicolAbort(QObject::tr("Connect control server failed!"), EX_LOCATION);
        }
    }
    XT_Controler::ReBuildSystem();
    QThread::msleep(350);
    if (XT_Controler::ConnectControlServer(const_cast<LPWSTR>(controlCardConfig->controllerIP().toStdWString().c_str()),
                                           0)
        != 1)
    {
        throw SilicolAbort(QObject::tr("Connect control server failed!"), EX_LOCATION);
    }

    quint64 serverVersion, slaverVersion;
    XT_Controler::GetControlServerVersionNum(serverVersion, slaverVersion);
    qCInfo(motionCate(), MSG_ARG("Get version", serverVersion, slaverVersion));

    if (XT_Controler_Extend::Profile_Init_Controller(1) != 1)
    {
        throw SilicolAbort(QObject::tr("Init controller failed!"), EX_LOCATION);
    }


//    XT_Controler_Extend::Stop_Buffer_Sync();
    XtVcm::initAllVcm();
//    XT_Controler_Extend::Start_Buffer_Sync(-1);

    isInit = true;
    return;
}
