#ifndef HIKVISIONLOCATIONCONFIG_H
#define HIKVISIONLOCATIONCONFIG_H

#include "VisionLocation/visionlocationconfig.h"
#include "hikvision_global.h"

class HIKVISIONSHARED_EXPORT HikVisionLocationConfig : public VisionLocationConfig
{
    Q_OBJECT

    //Q_PROPERTY(double test READ test WRITE setTest NOTIFY testChanged)

public:
    HikVisionLocationConfig(QObject *parent = nullptr) : VisionLocationConfig(parent)
    {
        init();
    }
};

#endif    // HIKVISIONLOCATIONCONFIG_H
