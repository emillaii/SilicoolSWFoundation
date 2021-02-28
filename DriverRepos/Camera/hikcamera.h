#ifndef HIKCAMERA_H
#define HIKCAMERA_H

#include "MvCameraControl.h"
#include "sccamera.h"
#include <QElapsedTimer>
#include <QObject>

class HikCamera : public SCCamera
{
    Q_OBJECT

public:
    explicit HikCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent = nullptr);

    ~HikCamera() override;

    // SCCamera interface
public:
    virtual void openImpl() override;
    virtual void closeImpl() override;

protected:
    virtual QImage getImageImpl() override;

private:
    int getIntValue(const char *strKey);

    void createHandle(MV_CC_DEVICE_INFO *pstDeviceInfo);

    void destroyHandle();

    void openCamera(MV_CC_DEVICE_INFO *pstDeviceInfo);

    void StopGrabbing();

    void StartGrabbing();

    void mallocBufferForDriver();

private:
    void *m_hDevHandle = nullptr;
    bool isGrabbing = false;
    unsigned char *m_pBufForDriver = nullptr;    // 用于从驱动获取图像的缓存
    unsigned int m_nBufSizeForDriver;
};

#endif    // HIKCAMERA_H
