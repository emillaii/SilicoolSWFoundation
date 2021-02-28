#ifndef DUMMYCAMERA_H
#define DUMMYCAMERA_H

#include "sccamera.h"

class SCVISIONSHARED_EXPORT DummyCamera : public SCCamera
{
    Q_OBJECT

public:
    DummyCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent = nullptr) : SCCamera(cameraName, cameraConfig, parent) {}

    // SCCamera interface
public:
    virtual void openImpl() override
    {
        qCInfo(visionCate()) << QString("Open dummy camera %1...").arg(cameraName());
    }
    virtual void closeImpl() override
    {
        qCInfo(visionCate()) << QString("Close dummy camera %1...").arg(cameraName());
    }

protected:
    virtual QImage getImageImpl() override
    {
        return QImage(config()->imageWidth(), config()->imageHeight(), QImage::Format_Indexed8);
    }
};

#endif    // DUMMYCAMERA_H
