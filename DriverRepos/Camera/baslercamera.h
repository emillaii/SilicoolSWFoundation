#ifndef BASLERCAMERA_H
#define BASLERCAMERA_H

#include "pylon/PylonIncludes.h"
#include "sccamera.h"

using namespace Pylon;
using namespace GenApi_3_1_Basler_pylon;

class BaslerCamera : public SCCamera
{
public:
    BaslerCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent = nullptr);

    // SCCamera interface
public:
    virtual void openImpl() override;
    virtual void closeImpl() override;

protected:
    virtual QImage getImageImpl() override;

private:
    CInstantCamera camera;
};

#endif    // BASLERCAMERA_H
