#include "hikcamera.h"

HikCamera::HikCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent) : SCCamera(cameraName, cameraConfig, parent)
{
    connect(cameraConfig, &CameraConfig::exposureTimeChanged, this, &HikCamera::onExposureTimeChanged);
}

HikCamera::~HikCamera()
{
    if (m_pBufForDriver != nullptr)
    {
        delete m_pBufForDriver;
        m_pBufForDriver = nullptr;
    }
}

void HikCamera::openImpl()
{
    int nRet = MV_OK;
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    HIK_RESULT_HANDLE(MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList))

    bool foundCamera = false;
    for (size_t i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO *pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (nullptr == pDeviceInfo)
        {
            continue;
        }
        QString userDefinedName;
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            userDefinedName = reinterpret_cast<char *>(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
        }
        else    // MV_USB_DEVICE
        {
            userDefinedName = reinterpret_cast<char *>(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        }
        if (userDefinedName == config()->cameraName())
        {
            openCamera(pDeviceInfo);
            foundCamera = true;
            break;
        }
    }
    if (!foundCamera)
    {
        throw SilicolAbort(tr("Did not find camera: %1").arg(cameraName()));
    }
}

void HikCamera::closeImpl()
{
    StopGrabbing();
    MV_CC_CloseDevice(m_hDevHandle);
    destroyHandle();
}

QImage HikCamera::getImageImpl()
{
    if (!isGrabbing)
    {
        throw GeneralError("Vision", tr("Camera %1 is not grabbing data.").arg(cameraName()));
    }

    mallocBufferForDriver();

    MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
    const int MaxRetry = 3;
    int index = 0;
    while (true)
    {
        int nRet = MV_CC_TriggerSoftwareExecute(m_hDevHandle);
        if (nRet != MV_OK)
        {
            throw GeneralError("Vision",
                               tr("Execute software trigger failed! Camera: %1, Error code: %2").arg(cameraName()).arg(nRet, 8, 16, QChar('0')));
        }

        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        MV_CC_GetOneFrameTimeout(m_hDevHandle, m_pBufForDriver, m_nBufSizeForDriver, &stImageInfo, 1000);

        if (nRet != MV_OK)
        {
            throw GeneralError("Vision", tr("Grab frame failed! Camera: %1, Error code: %2").arg(cameraName()).arg(nRet, 8, 16, QChar('0')));
        }
        if (stImageInfo.nFrameLen != m_nBufSizeForDriver)
        {
            index++;
            if (stImageInfo.nFrameLen == 0 && index < MaxRetry)
            {
                //                qCWarning(visionCate()) << cameraName() << "package lost. Auto retry..." << index;
                continue;
            }
            throw GeneralError(
                "Vision", tr("Received buffer size error. Received size: %1, expected size: %2").arg(stImageInfo.nFrameLen).arg(m_nBufSizeForDriver));
        }
        break;
    }

    QImage imgBuff(stImageInfo.nWidth, stImageInfo.nHeight, QImage::Format_Indexed8);
    MV_CC_PIXEL_CONVERT_PARAM stConvertParam = { 0 };
    memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
    stConvertParam.nWidth = stImageInfo.nWidth;
    stConvertParam.nHeight = stImageInfo.nHeight;
    stConvertParam.pSrcData = m_pBufForDriver;
    stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;
    stConvertParam.enSrcPixelType = stImageInfo.enPixelType;
    stConvertParam.enDstPixelType = PixelType_Gvsp_Mono8;
    stConvertParam.pDstBuffer = imgBuff.bits();
    stConvertParam.nDstBufferSize = imgBuff.width() * imgBuff.height();
    int nRet = MV_CC_ConvertPixelType(m_hDevHandle, &stConvertParam);

    if (nRet != MV_OK)
    {
        throw SilicolAbort(tr("Convert image format failed! Camera: %1").arg(cameraName()), EX_LOCATION);
    }
    return imgBuff;
}

void HikCamera::onExposureTimeChanged(double exposureTime)
{
    if (isOpened())
    {
        MV_CC_SetFloatValue(m_hDevHandle, "ExposureTime", config()->exposureTime());
    }
}

int HikCamera::getIntValue(const char *strKey)
{
    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    HIK_RESULT_HANDLE(MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam))
    return stParam.nCurValue;
}

void HikCamera::createHandle(MV_CC_DEVICE_INFO *pstDeviceInfo)
{
    if (m_hDevHandle == nullptr)
    {
        int res = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
        if (res != MV_OK)
        {
            m_hDevHandle = nullptr;
            throw SilicolAbort(tr("MV_CC_CreateHandle failed! Error code: %1").arg(res, 8, 16, QChar('0')), EX_LOCATION);
        }
    }
}

void HikCamera::destroyHandle()
{
    if (m_hDevHandle != nullptr)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = nullptr;
    }
}

void HikCamera::openCamera(MV_CC_DEVICE_INFO *pstDeviceInfo)
{
    if (nullptr == pstDeviceInfo)
    {
        throw SilicolAbort(tr("Empty MV_CC_DEVICE_INFO!"), EX_LOCATION);
    }

    createHandle(pstDeviceInfo);
    int res = MV_CC_OpenDevice(m_hDevHandle);
    if (res != MV_OK)
    {
        destroyHandle();
        throw SilicolAbort(tr("Open camera failed! Camera: %1, Error code: %2").arg(cameraName()).arg(res, 8, 16, QChar('0')));
    }
    HIK_RESULT_HANDLE(MV_CC_SetTriggerMode(m_hDevHandle, MV_TRIGGER_MODE_ON))
    HIK_RESULT_HANDLE(MV_CC_SetTriggerSource(m_hDevHandle, MV_TRIGGER_SOURCE_SOFTWARE))
    HIK_RESULT_HANDLE(MV_CC_SetGrabStrategy(m_hDevHandle, MV_GrabStrategy_UpcomingImage))
    HIK_RESULT_HANDLE(MV_CC_SetFloatValue(m_hDevHandle, "ExposureTime", config()->exposureTime()))
    mallocBufferForDriver();
    StartGrabbing();
}

void HikCamera::StopGrabbing()
{
    if (isGrabbing)
    {
        isGrabbing = false;
        int res = MV_CC_StopGrabbing(m_hDevHandle);
        if (res != MV_OK)
        {
            qCCritical(visionCate()) << tr("Stop grabbing failed! Camera: %1, Error code: %2").arg(cameraName()).arg(res, 8, 16, QChar('0'));
        }
    }
}

void HikCamera::StartGrabbing()
{
    if (isGrabbing)
    {
        return;
    }
    HIK_RESULT_HANDLE(MV_CC_StartGrabbing(m_hDevHandle))
    isGrabbing = true;
}

void HikCamera::mallocBufferForDriver()
{
    if (nullptr == m_pBufForDriver)
    {
        m_nBufSizeForDriver = getIntValue("PayloadSize");
        qCInfo(visionCate()) << cameraName() << "PayloadSize" << m_nBufSizeForDriver;
        m_pBufForDriver = static_cast<unsigned char *>(malloc(m_nBufSizeForDriver));
        if (m_pBufForDriver == nullptr)
        {
            throw SilicolAbort(tr("Malloc m_pBufForDriver failed, run out of memory"), EX_LOCATION);
        }
    }
}
