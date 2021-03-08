#ifndef HIKVISIONLOCATIONCONFIG_H
#define HIKVISIONLOCATIONCONFIG_H

#include "VisionLocation/visionlocationconfig.h"

class HikVisionLocationConfig : public VisionLocationConfig
{
    Q_OBJECT

public:
    HikVisionLocationConfig(QObject *parent = nullptr) : VisionLocationConfig(parent)
    {
        init();
    }
};

#endif    // HIKVISIONLOCATIONCONFIG_H
