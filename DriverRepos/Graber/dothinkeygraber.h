#ifndef DOTHINKEYGRABER_H
#define DOTHINKEYGRABER_H

#include <QObject>
#include <Windows.h>
#include "scgraber.h"
#include "imagekit.h"
#include "dtccm2.h"
#include "errorHandling/silicolerror.h"
#include "qimage.h"

#define MAX_DEV 4

class DothinkeyGraber : public SCGraber
{
    struct CameraChannel
    {
        CameraChannel()
            : m_iDevID(-1)
            , m_fMclk(12.0f)
            , m_fAvdd(2.8f)
            , m_fDovdd(1.8f)
            , m_fDvdd(1.5f)
            , m_fAfvcc(0.028f)
            , m_vpp(0.0f)
            , m_uFocusPos(0)
        {}

        bool CloseCameraChannel();
        SensorTab current_sensor;
        char* m_charCurrentDevice;
        int m_iDevID;
        ULONG m_GrabSize;
        float m_fMclk;
        float m_fAvdd;
        float m_fDovdd;
        float m_fDvdd;
        float m_fAfvcc;
        float m_vpp;
        UINT m_uFocusPos;
        float m_fFrameFps;
        int m_iFrameErr;
        int m_iFrameCnt;
        int m_iPhyType;
        int m_iLanes;
    };

    Q_OBJECT
public:
    explicit DothinkeyGraber(QObject *parent = nullptr);
    ~DothinkeyGraber() override;

    void readIniFile(QString iniFilename);
    BOOL DothinkeyIsGrabbing();

    virtual void enumerateDevice() override;
    virtual void releaseDevice() override{}
    virtual void openDevice() override;
    virtual void closeDevice() override;
    virtual void startCamera() override;
    virtual QImage grabImage() override;
    virtual cv::Mat grabImageCV() override;

    void setDeviceName(QString deviceName);

private:
    BOOL SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd,
                        float Dvdd, float Dovdd, float Afvcc, float vpp);

signals:

public slots:

private:
    QMutex locker;
    char *m_devName[MAX_DEV];
    int m_devNum = 0;
    int m_devID = 0;
    QString m_currentDevName;
    bool m_isOpened = false;
    bool m_isGrabbing = false;
    CameraChannel m_CameraChannels[MAX_DEV];
    QStringList cmd_list;
    QStringList otp_list;
};

class iniParser
{
public:
    iniParser();
    ~iniParser();
public:
    void SetIniFilename(std::string sFilename);
    int ReadIniData(std::string sSection, std::string sSectionKey, int nDefault);
    BOOL GetI2CData(pSensorTab pSensor, QStringList &cmd_list, QStringList &otp_list);
protected:
    std::string sFilename;
};

#endif // DOTHINKEYGRABER_H
