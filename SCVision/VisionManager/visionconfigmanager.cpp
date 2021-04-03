#include "visionconfigmanager.h"

VisionConfigManager::VisionConfigManager(const QMetaObject &visionLocationConfigMetaObj, QString configDir, QObject *parent)
    : QObject(parent), m_visionLocationConfigMetaObj(&visionLocationConfigMetaObj), m_dutRelatedConfigDir(configDir)
{
    if (!m_dutRelatedConfigDir.endsWith("/") && !m_dutRelatedConfigDir.endsWith("\\"))
    {
        m_dutRelatedConfigDir += "/";
    }

    ved = new VisionElementDefinition(this);
    vedConfigFile = new ConfigFile("VisionElementDefinition", ved, vedConfigFileName);
    vedConfigFile->populate(true);

    m_calibrationConfigs = new ConfigObjectArray(&CalibrationConfig::staticMetaObject, this);
    calibrationConfigsFile = new ConfigFile("CalibrationConfig", m_calibrationConfigs, visionConfigDir + calibrationConfigsFileName);
    calibrationConfigsFile->populate();

    m_visionLocationConfigs = new ConfigObjectArray(&visionLocationConfigMetaObj, this);
    visionLocationConfigsFile = new ConfigFile("VisionLocation", m_visionLocationConfigs, m_dutRelatedConfigDir + visionLocationConfigsFileName);
    visionLocationConfigsFile->populate();

    m_cameraConfigs = new ConfigObjectArray(&CameraConfig::staticMetaObject, this);
    cameraConfigsFile = new ConfigFile("CameraConfigs", m_cameraConfigs, visionConfigDir + cameraConfigsFileName);
    cameraConfigsFile->populate();

    m_moveAndPrConfig = new MoveAndPrConfig(this);
    moveAndPrConfigFile = new ConfigFile("MoveAndPrConfig", m_moveAndPrConfig, visionConfigDir + moveAndPrConfigFileName);
    moveAndPrConfigFile->populate();

    m_cameraConfigs->setUIAddRemovable(false);
    m_visionLocationConfigs->setUIAddRemovable(false);
    m_calibrationConfigs->setUIAddRemovable(false);

    config2Dic();
    removeRedundantConfig();
    createLostConfig();
    setOptionalProperties();
}

void VisionConfigManager::config2Dic()
{
    for (int i = 0; i < m_calibrationConfigs->count(); i++)
    {
        auto calibrationConfig = m_calibrationConfigs->getConfig<CalibrationConfig>(i);
        calibrationConfigMap[calibrationConfig->calibrationName()] = calibrationConfig;
    }
    for (int i = 0; i < m_visionLocationConfigs->count(); i++)
    {
        auto visionLocationConfig = m_visionLocationConfigs->getConfig<VisionLocationConfig>(i);
        visionLocationConfigMap[visionLocationConfig->locationName()] = visionLocationConfig;
    }
    for (int i = 0; i < m_cameraConfigs->count(); i++)
    {
        auto cameraConfig = m_cameraConfigs->getConfig<CameraConfig>(i);
        cameraConfigMap[cameraConfig->cameraName()] = cameraConfig;
    }
}

void VisionConfigManager::createLostConfig()
{
    auto vlDefinitions = ved->visionLocationDefinitions();
    for (int i = 0; i < vlDefinitions->count(); i++)
    {
        auto vlDefinition = vlDefinitions->getConfig<VisionLocationDefinition>(i);
        if (!calibrationConfigMap.contains(vlDefinition->calibrationName()))
        {
            auto calibrationConfig = new CalibrationConfig();
            calibrationConfig->setCalibrationName(vlDefinition->calibrationName());
            calibrationConfig->setLocationName(vlDefinition->locationName());
            calibrationConfigMap[vlDefinition->calibrationName()] = calibrationConfig;
            m_calibrationConfigs->executeAddConfigObject(0, calibrationConfig);
        }
        if (!visionLocationConfigMap.contains(vlDefinition->locationName()))
        {
            auto vlConfig = new VisionLocationConfig();
            vlConfig->setCalibrationName(vlDefinition->calibrationName());
            vlConfig->setLocationName(vlDefinition->locationName());
            visionLocationConfigMap[vlConfig->locationName()] = vlConfig;
            m_visionLocationConfigs->executeAddConfigObject(0, vlConfig);
        }
    }

    auto cameraNames = ved->cameraNames();
    for (int i = 0; i < cameraNames->count(); i++)
    {
        QString cameraName = cameraNames->at(i).toString();
        if (!cameraConfigMap.contains(cameraName))
        {
            auto cameraConfig = new CameraConfig();
            cameraConfig->setCameraName(cameraName);
            cameraConfigMap[cameraName] = cameraConfig;
            m_cameraConfigs->executeAddConfigObject(0, cameraConfig);
        }
    }
}

void VisionConfigManager::removeRedundantConfig()
{
    QStringList definedCalibrationNames;
    QStringList definedVisionLocationNames;
    QStringList definedCameraNames;
    auto vlDefinitions = ved->visionLocationDefinitions();
    for (int i = 0; i < vlDefinitions->count(); i++)
    {
        auto vlDefinition = vlDefinitions->getConfig<VisionLocationDefinition>(i);
        if (!definedCalibrationNames.contains(vlDefinition->calibrationName()))
        {
            definedCalibrationNames.append(vlDefinition->calibrationName());
        }
        if (!definedVisionLocationNames.contains(vlDefinition->locationName()))
        {
            definedVisionLocationNames.append(vlDefinition->locationName());
        }
    }
    auto cameraNames = ved->cameraNames();
    for (int i = 0; i < cameraNames->count(); i++)
    {
        definedCameraNames.append(cameraNames->at(i).toString());
    }

    for (int i = m_calibrationConfigs->count() - 1; i >= 0; i--)
    {
        auto calibrationName = m_calibrationConfigs->getConfig<CalibrationConfig>(i)->calibrationName();
        if (!definedCalibrationNames.contains(calibrationName))
        {
            m_calibrationConfigs->remove(i);
            if (calibrationConfigMap.contains(calibrationName))
            {
                calibrationConfigMap.remove(calibrationName);
            }
        }
    }
    for (int i = m_visionLocationConfigs->count() - 1; i >= 0; i--)
    {
        auto visionLocationName = m_visionLocationConfigs->getConfig<VisionLocationConfig>(i)->locationName();
        if (!definedVisionLocationNames.contains(visionLocationName))
        {
            m_visionLocationConfigs->remove(i);
            if (visionLocationConfigMap.contains(visionLocationName))
            {
                visionLocationConfigMap.remove(visionLocationName);
            }
        }
    }
    for (int i = m_cameraConfigs->count() - 1; i >= 0; i--)
    {
        auto cameraName = m_cameraConfigs->getConfig<CameraConfig>(i)->cameraName();
        if (!definedCameraNames.contains(cameraName))
        {
            m_cameraConfigs->remove(i);
            if (cameraConfigMap.contains(cameraName))
            {
                cameraConfigMap.remove(cameraName);
            }
        }
    }
}

void VisionConfigManager::setOptionalProperties()
{
    QVariantList lscNames = ved->lscNames()->members();
    foreach (auto cameraConfig, cameraConfigMap.values())
    {
        cameraConfig->setOptionalLscName(lscNames);
    }

    QStringList cameraNames = cameraConfigMap.keys();
    foreach (auto visionLocationConfig, visionLocationConfigMap.values())
    {
        visionLocationConfig->setOptionalCamera(cameraNames);
    }
}

void VisionConfigManager::handleDutChanged(QString dutRelatedConfigDir)
{
    if (!dutRelatedConfigDir.endsWith("/") && !dutRelatedConfigDir.endsWith("\\"))
    {
        dutRelatedConfigDir += "/";
    }

    ConfigObjectArray newVlConfigs(m_visionLocationConfigMetaObj);
    ConfigFile newVlConfigFile("", &newVlConfigs, dutRelatedConfigDir + visionLocationConfigsFileName);
    newVlConfigFile.populate();

    visionLocationConfigsFile->setAutoSave(false);

    for (int i = 0; i < newVlConfigs.count(); i++)
    {
        auto newVlConfig = newVlConfigs.getConfig<VisionLocationConfig>(i);
        if (visionLocationConfigMap.contains(newVlConfig->locationName()))
        {
            auto currentVlConfig = visionLocationConfigMap[newVlConfig->locationName()];
            if (currentVlConfig->calibrationName() == newVlConfig->calibrationName())
            {
                QJsonValue jv;
                newVlConfig->write(jv);
                currentVlConfig->read(jv);
            }
            else
            {
                qCCritical(visionCate()) << QString(
                    tr("Load new vision location config failed! Location name: %1, new calibration name: %2, current calibration name: %3")
                        .arg(currentVlConfig->locationName())
                        .arg(newVlConfig->calibrationName())
                        .arg(currentVlConfig->calibrationName()));
            }
        }
    }

    m_dutRelatedConfigDir = dutRelatedConfigDir;
    visionLocationConfigsFile->resetFileName(m_dutRelatedConfigDir + visionLocationConfigsFileName);
    visionLocationConfigsFile->setAutoSave(true);
}
