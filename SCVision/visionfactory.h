#ifndef VISIONFACTORY_H
#define VISIONFACTORY_H

#include "lightsourcecontroller.h"
#include "sccamera.h"
#include "scvision.h"
#include "scvision_global.h"

class SCVISIONSHARED_EXPORT VisionFactory
{
public:
    virtual ~VisionFactory() {}
    virtual LightSourceController *createLSC(QString lscName) = 0;
    virtual SCCamera *
    createCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent = nullptr) = 0;
    virtual SCVision *createVision() = 0;
};

#endif    // VISIONFACTORY_H
