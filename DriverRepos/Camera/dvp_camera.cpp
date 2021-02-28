#include "dvp_camera.h"

DVP_Camera::DVP_Camera(QString cameraName, CameraConfig *cameraConfig, QObject *parent) : SCCamera(cameraName, cameraConfig, parent) {}

void DVP_Camera::openImpl()
{
    closeImpl();
    DVP_RESULT_HANDLE(dvpOpenByUserId(config()->cameraName().toUtf8(), OPEN_NORMAL, &handle));
    DVP_RESULT_HANDLE(dvpSetTriggerState(handle, true));
    DVP_RESULT_HANDLE(dvpSetTriggerSource(handle, TRIGGER_SOURCE_SOFTWARE));
    DVP_RESULT_HANDLE(dvpSetExposure(handle, config()->exposureTime()));
    DVP_RESULT_HANDLE(dvpStart(handle));
}

void DVP_Camera::closeImpl()
{
    bool isValid = false;
    dvpIsValid(handle, &isValid);
    if (isValid)
    {
        dvpStreamState state;
        dvpGetStreamState(handle, &state);
        if (state == STATE_STARTED)
        {
            dvpStop(handle);
        }
        dvpClose(handle);
    }
}

QImage DVP_Camera::getImageImpl()
{
    dvpStatus status = dvpTriggerFire(handle);
    if (status != DVP_STATUS_OK)
    {
        throw GeneralError("Vision", tr("Execute software trigger failed! Camera: %1, Error code: %2").arg(cameraName()).arg(status));
    }
    dvpFrame frameInfo;
    void *buffer;
    status = dvpGetFrame(handle, &frameInfo, &buffer, 1000);
    if (status != DVP_STATUS_OK)
    {
        throw GeneralError("Vision", tr("Grab frame failed! Camera: %1, Error code: %2").arg(cameraName()).arg(status));
    }
    QImage img(frameInfo.iWidth, frameInfo.iHeight, QImage::Format_Indexed8);
    if (frameInfo.format == FORMAT_MONO && frameInfo.bits == BITS_8)
    {
        memcpy(img.bits(), buffer, frameInfo.uBytes);
    }
    return img;
}
