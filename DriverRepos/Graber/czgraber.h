#ifndef CZGRABER_H
#define CZGRABER_H

#include <QObject>
#include <Windows.h>
#include "scgraber.h"
#include "errorHandling/silicolerror.h"
#include "qimage.h"

#include "CmtiSdk.h"
#include "czsdkwrapper.h"
#include "czsensorinifile.h"
#include "buffercircularqueue.h"

#define MAX_DEV 4

class Option
{
public:
    enum E_CaptureMode {
        CaptureMode_GrabFrame,
        CaptureMode_GrabLatestFrame,
        CaptureMode_DeEnqueueBuffer,
        CaptureMode_DeEnqueueLatestBuffer,
    };
    enum E_OutRawFormat {
        OutRawFormat_Packed,
        OutRawFormat_UnPacked,
        OutRawFormat_Raw8
    };

    enum E_LanguageChange
    {
        CHINESE,
        ENGLISH
    };
    enum E_CurrentMeasureFuncVersion
    {
        CurrentMeasureFuncVersion_1 = 0,
        CurrentMeasureFuncVersion_2
    };
};

struct T_ThreadParam
{
    Option *m_Option;
    T_FrameParam FrameParam;
    uchar *ImageBuffer;
    T_SensorSetting *pSensorSetting;
    BufferCircularQueue *Rgb32BufferPool;
};

class CZGraber : public SCGraber
{
    Q_OBJECT
public:
    explicit CZGraber(QObject *parent = nullptr);
    ~CZGraber() override;

    void readIniFile(QString filename);
    void setupFrameBuffer(const T_FrameParam &frameParam);
    bool checkIsPlaying();

    void getExposureRange(uint *maxExpoVal, uint *minExpoVal);
    void setExposureCoarse(int expoCoarse);

public:
    virtual void enumerateDevice() override;
    virtual void releaseDevice() override{}
    virtual void openDevice() override;
    virtual void closeDevice() override;
    virtual void startCamera() override;
    virtual QImage grabImage() override;
    virtual cv::Mat grabImageCV() override;
signals:

public slots:

private:
    QMutex mutex;
    std::string m_currentDevice = "";
    DeviceClient * m_czDeviceClient = nullptr;
    CzSensorIniFile m_czIniFile;
    T_SensorSetting m_sensorSetting;
    T_ThreadParam m_threadParam;
    bool m_isPlaying = false;
    uint8_t *pRgbBuf1 = nullptr;
    uint8_t *pRgbBuf2 = nullptr;
    int decodeRawImage(const T_FrameParam &frameParam, Option::E_OutRawFormat imageFormat, const uchar *pSrcBuf,  uchar *pTempRaw8, uchar *pRgb32Buf);

};

#endif // CZGRABER_H
