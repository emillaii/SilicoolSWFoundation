#ifndef DVP_CAMERA_H
#define DVP_CAMERA_H

#include "DVPCamera.h"
#include "sccamera.h"
#include <QObject>

#define DVP_RESULT_HANDLE(calling)                                                                                                                   \
    {                                                                                                                                                \
        dvpStatus res = calling;                                                                                                                     \
        if (res != DVP_STATUS_OK)                                                                                                                    \
        {                                                                                                                                            \
            throw SilicolAbort(QString("%1 failed! Error code: %2, camera: %3").arg(#calling).arg(res).arg(cameraName()), EX_LOCATION);              \
        }                                                                                                                                            \
    }

class DVP_Camera : public SCCamera
{
    Q_OBJECT
public:
    explicit DVP_Camera(QString cameraName, CameraConfig *cameraConfig, QObject *parent = nullptr);

    // SCCamera interface
public:
    void openImpl() override;
    void closeImpl() override;

protected:
    QImage getImageImpl() override;

private:
    dvpHandle handle = 0;
};

#endif    // DVP_CAMERA_H
