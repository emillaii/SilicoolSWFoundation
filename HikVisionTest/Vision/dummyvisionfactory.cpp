#include "dummyvisionfactory.h"

DummyVisionFactory::DummyVisionFactory(VisionConfigManager *visionConfigManager)
{
    m_wordopLscConfigs = new ConfigObjectArray(&WordopLSCConfig::staticMetaObject);
    wordopLscConfigFile = new ConfigFile("WordopLsc", m_wordopLscConfigs, "./config/wordopLscConfigs.json");
    wordopLscConfigFile->populate();

    for (int i = 0; i < m_wordopLscConfigs->count(); ++i)
    {
        auto lscConfig = m_wordopLscConfigs->getConfig<WordopLSCConfig>(i);
        wordopLscConfigMap[lscConfig->lscName()] = lscConfig;
    }

    auto lscNames = visionConfigManager->visionElementDefinition()->lscNames();
    for (int i = 0; i < lscNames->count(); ++i)
    {
        auto lscName = lscNames->at(i).toString();
        if (!wordopLscConfigMap.contains(lscName))
        {
            WordopLSCConfig *lscConfig = new WordopLSCConfig();
            lscConfig->setLscName(lscName);
            wordopLscConfigMap[lscName] = lscConfig;
            m_wordopLscConfigs->executeAddConfigObject(m_wordopLscConfigs->count(), lscConfig);
        }
    }
}

LightSourceController *DummyVisionFactory::createLSC(QString lscName)
{
    if (!wordopLscConfigMap.contains(lscName))
    {
        throw SilicolAbort(QObject::tr("Undefined light source controller name: %1").arg(lscName), EX_LOCATION);
    }
    return new WordopLightSourceController(wordopLscConfigMap[lscName]);
}

SCCamera *DummyVisionFactory::createCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent)
{
    switch (cameraConfig->cameraVendor())
    {
        case CameraConfig::Hik:
            return new HikCamera(cameraName, cameraConfig, parent);
        case CameraConfig::Basler:
            return new BaslerCamera(cameraName, cameraConfig, parent);
        case CameraConfig::DVP:
            return new DVP_Camera(cameraName, cameraConfig, parent);
    }
    qCCritical(visionCate()) << "Unknown camera vendor:" << cameraConfig->cameraVendor();
    return new DummyCamera(cameraName, cameraConfig, parent);
}
