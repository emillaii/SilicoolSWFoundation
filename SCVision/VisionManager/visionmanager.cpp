#include "visionmanager.h"

VisionManager::~VisionManager()
{
    cameraMap.clear();
    auto lscNames = lscs.keys();
    foreach (auto lscName, lscNames)
    {
        auto lsc = lscs[lscName];
        delete lsc;
    }
    lscs.clear();
    if (m_vision != nullptr)
    {
        m_vision->dispose();
        delete m_vision;
        m_vision = nullptr;
    }
}

void VisionManager::setContextProperty(QQmlApplicationEngine &engine)
{
    if (visionConfigManager == nullptr)
    {
        qFatal("visionConfigManager was not set!");
    }
    engine.rootContext()->setContextProperty("visionElementDefinition", visionConfigManager->visionElementDefinition());
    engine.rootContext()->setContextProperty("cameraConfigs", visionConfigManager->cameraConfigs());
    engine.rootContext()->setContextProperty("calibrationConfigs", visionConfigManager->calibrationConfigs());
    engine.rootContext()->setContextProperty("visionLocationConfigs", visionConfigManager->visionLocationConfigs());
    engine.rootContext()->setContextProperty("moveAndPrConfig", visionConfigManager->moveAndPrConfig());
    engine.rootContext()->setContextProperty("cameraNames", cameraNames());
    engine.rootContext()->setContextProperty("visionManager", this);
    engine.rootContext()->setContextProperty("visionConfigManager", visionConfigManager);
    foreach (auto cameraName, cameraNames())
    {
        engine.addImageProvider(cameraName, dynamic_cast<QQuickImageProvider *>(getCamera(cameraName)));
    }
}

void VisionManager::preInit()
{
    if (visionConfigManager == nullptr)
    {
        qFatal("Please set vision config manager first!");
    }
    if (visionFactory == nullptr)
    {
        qFatal("Please set vision factory first!");
    }

    auto lscNames = visionConfigManager->ved->lscNames();
    for (int i = 0; i < lscNames->count(); ++i)
    {
        QString lscName = lscNames->at(i).toString();
        if (!lscs.contains(lscName))
        {
            lscs[lscName] = visionFactory->createLSC(lscName);
        }
    }
    if (m_vision == nullptr)
    {
        m_vision = visionFactory->createVision();
    }
    auto cameraNames = visionConfigManager->ved->cameraNames();
    for (int i = 0; i < cameraNames->count(); i++)
    {
        QString cameraName = cameraNames->at(i).toString();
        if (!cameraMap.contains(cameraName))
        {
            auto cameraConfig = visionConfigManager->cameraConfigMap[cameraName];
            SCCamera *camera;
            if (cameraConfig->isDummyCamera())
            {
                camera = new DummyCamera(cameraName, cameraConfig, this);
            }
            else
            {
                camera = visionFactory->createCamera(cameraName, cameraConfig, this);
            }
            cameraMap[cameraName] = camera;
        }
    }

    foreach (auto lsc, lscs.values())
    {
        try
        {
            lsc->init();
        }
        catch (SilicoolException &se)
        {
            qCCritical(visionCate()) << se.what();
        }
    }

    foreach (auto camera, cameraMap.values())
    {
        try
        {
            initCamera(camera);
        }
        catch (SilicoolException &se)
        {
            qCCritical(visionCate()) << se.what();
        }
    }
}

void VisionManager::postInit(bool checkDongle)
{
    m_vision->init();
    if (checkDongle && !m_vision->checkDongle())
    {
        throw SilicolAbort(tr("Vision dongle was not detected!"));
    }
    auto vlds = visionConfigManager->ved->visionLocationDefinitions();
    for (int i = 0; i < vlds->count(); i++)
    {
        auto visionLocationName = vlds->getConfig<VisionLocationDefinition>(i)->locationName();
        if (!visionLocations.contains(visionLocationName))
        {
            auto visionLocationConfig = visionConfigManager->visionLocationConfigMap[visionLocationName];
            auto cameraName = visionLocationConfig->cameraName();
            if (!cameraMap.contains(cameraName))
            {
                throw SilicolAbort(tr("Undefined camera: %1").arg(cameraName), EX_LOCATION);
            }
            auto visionLocation = new VisionLocation(m_vision, cameraMap[cameraName], visionLocationConfig);
            visionLocations[visionLocationName] = visionLocation;
        }
    }

    for (int i = 0; i < vlds->count(); i++)
    {
        auto calibrationName = vlds->getConfig<VisionLocationDefinition>(i)->calibrationName();
        if (!calibrations.contains(calibrationName))
        {
            auto calibrationConfig = visionConfigManager->calibrationConfigMap[calibrationName];
            auto visionLocationName = calibrationConfig->locationName();
            if (!visionLocations.contains(visionLocationName))
            {
                throw SilicolAbort(tr("Undefined vision location: %1").arg(visionLocationName), EX_LOCATION);
            }
            auto calibration = new Calibration(calibrationConfig, visionLocations[visionLocationName]);
            calibrations[calibrationName] = calibration;
        }
    }

    for (int i = 0; i < vlds->count(); i++)
    {
        auto visionLocationName = vlds->getConfig<VisionLocationDefinition>(i)->locationName();
        auto visionLocationConfig = visionConfigManager->visionLocationConfigMap[visionLocationName];
        auto calibrationName = visionLocationConfig->calibrationName();
        if (!calibrations.contains(calibrationName))
        {
            throw SilicolAbort(tr("Undefined calibration: %1").arg(calibrationName), EX_LOCATION);
        }
        visionLocations[visionLocationName]->setCalibrationResult(calibrations[calibrationName]->config()->calibrationResult());
    }

    foreach (auto lsc, lscs.values())
    {
        lsc->init();
    }
    foreach (auto camera, cameraMap.values())
    {
        initCamera(camera);
    }
}

void VisionManager::stopAllCameraShowRealtimeImage()
{
    foreach (auto camera, cameraMap.values())
    {
        camera->stopShowRealtimeImage();
    }
}

void VisionManager::handleSingleCameraLiveView(QString cameraName)
{
    if (!cameraMap.contains(cameraName))
    {
        qCCritical(visionCate()) << tr("Undefined camera: ") << cameraName;
        return;
    }
    foreach (auto camera, cameraMap.values())
    {
        camera->stopShowRealtimeImage();
    }
    cameraMap[cameraName]->startShowRealtimeImage(false);
}

QStringList VisionManager::cameraNames() const
{
    return cameraMap.keys();
}

VisionLocation *VisionManager::getVisionLocation(QString visionLocationName)
{
    if (!visionLocations.contains(visionLocationName))
    {
        throw SilicolAbort(tr("Undefined vision location: %1").arg(visionLocationName), EX_LOCATION);
    }
    return visionLocations[visionLocationName];
}

Calibration *VisionManager::getCalibration(QString calibrationName)
{
    if (!calibrations.contains(calibrationName))
    {
        throw SilicolAbort(tr("Undefined calibration: %1").arg(calibrationName), EX_LOCATION);
    }
    return calibrations[calibrationName];
}

QObject *VisionManager::getCamera(QString cameraName) const
{
    if (cameraMap.contains(cameraName))
    {
        return cameraMap[cameraName];
    }
    else
    {
        qCCritical(visionCate()) << "Undefined camera:" << cameraName;
        return nullptr;
    }
}

void VisionManager::calibrate(QString calibrationName)
{
    if (!calibrations.contains(calibrationName))
    {
        throw SilicolAbort(tr("Undefined calibration: ") + calibrationName, EX_LOCATION);
    }
    calibrations[calibrationName]->performCalibration();
}

void VisionManager::performPr(QString visionLocationName)
{
    if (!visionLocations.contains(visionLocationName))
    {
        qCCritical(visionCate()) << tr("Undefined vision location:") << visionLocationName;
        return;
    }
    PrOffset prOffset;
    visionLocations[visionLocationName]->performPR(prOffset);
}

void VisionManager::startContinuallyPerformPr(QString visionLocationName)
{
    if (!visionLocations.contains(visionLocationName))
    {
        qCCritical(visionCate()) << tr("Undefined vision location:") << visionLocationName;
        return;
    }
    visionLocations[visionLocationName]->startContinuallyPerformPr();
}

void VisionManager::startContinuallyPerformObjSharpnessCalc(QString visionLocationName)
{
    if (!visionLocations.contains(visionLocationName))
    {
        qCCritical(visionCate()) << tr("Undefined vision location:") << visionLocationName;
        return;
    }
    visionLocations[visionLocationName]->startContinuallyPerformObjSharpnessCalc();
}

void VisionManager::stopContinuallyCalculation(QString visionLocationName)
{
    if (!visionLocations.contains(visionLocationName))
    {
        qCCritical(visionCate()) << tr("Undefined vision location:") << visionLocationName;
        return;
    }
    visionLocations[visionLocationName]->stopContinuallyCalculation();
}

void VisionManager::performPrResult(QString calibrationName, QString visionLocationName, bool perfomOriginPrResult)
{
    if (!calibrations.contains(calibrationName))
    {
        qCCritical(visionCate()) << tr("Undefined calibration:") << calibrationName;
        return;
    }
    if (!visionLocations.contains(visionLocationName))
    {
        qCCritical(visionCate()) << tr("Undefined vision location:") << visionLocationName;
        return;
    }
    PrOffset prOffset;
    if (!visionLocations[visionLocationName]->performPR(prOffset))
    {
        return;
    }
    if (perfomOriginPrResult)
    {
        calibrations[calibrationName]->performPROriginResult(prOffset);
    }
    else
    {
        calibrations[calibrationName]->performPRResult(prOffset);
    }
}

void VisionManager::moveCamera(QString cameraName, double px, double py)
{
    if (!cameraMap.contains(cameraName))
    {
        qCCritical(visionCate()) << "Undefined camera:" << cameraName;
        return;
    }
    QString calibrationForMoveCamera = cameraMap[cameraName]->config()->calibrationForMoveCamera();
    if (calibrationForMoveCamera.isEmpty())
    {
        return;
    }
    if (!calibrations.contains(calibrationForMoveCamera))
    {
        qCCritical(visionCate()) << "Undefined calibration:" << calibrationForMoveCamera;
        return;
    }
    auto calibration = calibrations[calibrationForMoveCamera];
    QPointF offset = calibration->getOneXPxielDistance() * px + calibration->getOneYPxielDistance() * py;
    calibration->performPRResult(PrOffset(offset.x(), offset.y(), 0));
}

void VisionManager::startMoveAndPr()
{
    if (runMoveAndPr)
    {
        return;
    }

    runMoveAndPr = true;
    AutoResetBool t(&runMoveAndPr, false);

    QString vlName(visionConfigManager->m_moveAndPrConfig->visionLocationName());
    auto vl = getVisionLocation(vlName);
    auto moveConfigs = visionConfigManager->m_moveAndPrConfig->axisMoveConfigs();

    QList<QString> axisNames;
    QList<double> pos1;
    QList<double> pos2;
    QList<double> prPos;
    for (int i = 0; i < moveConfigs->count(); i++)
    {
        auto moveConfig = moveConfigs->getConfig<AxisMoveConfig>(i);
        axisNames.append(moveConfig->axisName());
        pos1.append(moveConfig->pos1());
        pos2.append(moveConfig->pos2());
        prPos.append(moveConfig->prPos());
    }

    QDir dir("./moveAndPrData");
    if (!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }
    QString combinedAxisName = combineString("_", axisNames);
    QString now = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz");
    QFile logFile(QString("./moveAndPrData/%1_%2_%3.csv").arg(combinedAxisName).arg(vlName).arg(now));
    logFile.open(QIODevice::WriteOnly);
    QString title = "OffsetX,OffsetY,OffsetTheta\r\n";
    logFile.write(title.toUtf8());
    logFile.flush();

    PrOffset prOffset;
    auto moveAndPrCfg = visionConfigManager->m_moveAndPrConfig;
    for (int i = 0; i < moveAndPrCfg->times(); i++)
    {
        if (!runMoveAndPr)
        {
            logFile.close();
            return;
        }
        MotionManager::getIns().moveMultiAxes(axisNames, pos1);
        MotionManager::getIns().moveMultiAxes(axisNames, pos2);
        MotionManager::getIns().moveMultiAxes(axisNames, prPos);
        if (moveAndPrCfg->delayBeforePr() > 0)
        {
            QThread::msleep(moveAndPrCfg->delayBeforePr());
        }
        if (!vl->performPR(prOffset))
        {
            throw SilicolAbort(tr("%1 perform PR failed!").arg(vlName));
        }
        logFile.write((prOffset.toPureString() + "\r\n").toUtf8());
        logFile.flush();
    }
    logFile.close();
}

void VisionManager::stopMoveAndPr()
{
    runMoveAndPr = false;
}

void VisionManager::initCamera(SCCamera *camera)
{
    auto lscName = camera->config()->lscName();
    if (!lscs.contains(lscName))
    {
        throw SilicolAbort(tr("Undefined light source controller name: %1").arg(lscName), EX_LOCATION);
    }
    camera->setLsc(lscs[lscName]);
    int brightness = 0;
    if (camera->config()->lightSourceChannel() >= 0)
    {
        if (lscs[lscName]->getBrightness(camera->config()->lightSourceChannel(), brightness))
        {
            camera->setLightSourceBrightness(brightness);
        }
    }
    if (camera->config()->secondLightSourceChannel() >= 0)
    {
        if (lscs[lscName]->getBrightness(camera->config()->secondLightSourceChannel(), brightness))
        {
            camera->setSecondLightSourceBrightness(brightness);
        }
    }
    camera->open();
    camera->startShowRealtimeImage();
}
