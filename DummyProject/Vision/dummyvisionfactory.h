#ifndef DUMMYVISIONFACTORY_H
#define DUMMYVISIONFACTORY_H

#include "Camera/baslercamera.h"
#include "Camera/dvp_camera.h"
#include "Camera/hikcamera.h"
#include "LightSourceController/wordoplightsourcecontroller.h"
#include "VisionManager/visionconfigmanager.h"
#include "adaptvision.h"
#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "dummycamera.h"
#include "visionfactory.h"
#include <QObject>

class DummyVisionFactory : public VisionFactory
{
public:
    DummyVisionFactory(VisionConfigManager *visionConfigManager);

    ConfigObjectArray *wordopLscConfig() const
    {
        return m_wordopLscConfigs;
    }

    // VisionFactory interface
public:
    virtual LightSourceController *createLSC(QString lscName) override;

    virtual SCCamera *createCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent) override;

    virtual SCVision *createVision() override
    {
        return new AdaptVision();
    }

private:
    ConfigFile *wordopLscConfigFile;
    ConfigObjectArray *m_wordopLscConfigs;
    QMap<QString, WordopLSCConfig *> wordopLscConfigMap;
};

#endif    // DUMMYVISIONFACTORY_H
