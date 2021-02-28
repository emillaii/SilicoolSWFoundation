#include "czgraber.h"
#include <fstream>
#include "czimageproc.h"

CZGraber::CZGraber(QObject *parent) : SCGraber(parent)
{
}

CZGraber::~CZGraber()
{
}

void CZGraber::enumerateDevice()
{
    std::vector<std::string> deviceNameList;
    DeviceController::GetInstance()->EnumerateDevice(deviceNameList);

    int index = CB_ERR;
    for (size_t i = 0; i < deviceNameList.size(); i++)
    {
        qInfo("[CZGraber] enumerateDevice: %s", deviceNameList[i].c_str());
    }
    if (deviceNameList.size() == 0) {
        throw  SilicolAbort(tr("CZ Grabber cannot find device!"));
    } else {
        //Select the first enum device (Suppose no mutliple device in this use case)
        m_currentDevice = deviceNameList[0];
    }
}

void CZGraber::openDevice()
{
    if (m_currentDevice.size() == 0) {
        throw  SilicolAbort(tr("CZ Grabber cannot find device! Please enum device first"));
    }
    m_czDeviceClient = DeviceController::GetInstance()->GetDeviceClient(this->m_currentDevice);
    if (m_czDeviceClient != nullptr) {
        qInfo("GetDeviceClient success");
    }
}

void CZGraber::setupFrameBuffer(const T_FrameParam &frameParam)
{
    qInfo("setup FrameBuffer: width %d height: %d size: %d imageMode: %d imageFormat: %d", frameParam.Width, frameParam.Height,
                                          frameParam.Size, frameParam.ImageMode,
                                          frameParam.ImageFormat);
    if ((m_threadParam.FrameParam.Width != frameParam.Width) || (m_threadParam.FrameParam.Height != frameParam.Height) || (m_threadParam.FrameParam.ImageFormat != frameParam.ImageFormat)) {
        if (m_threadParam.ImageBuffer != nullptr) {
            qInfo("delete image buffer");
            //delete[] m_threadParam.ImageBuffer;
            m_threadParam.ImageBuffer = nullptr;
        }
        if (m_threadParam.Rgb32BufferPool != nullptr) {
            qInfo("delete Rgb32 buffer");
            //delete m_threadParam.Rgb32BufferPool;
            m_threadParam.Rgb32BufferPool = nullptr;
        }
    }
    if (m_threadParam.ImageBuffer == nullptr)
        qInfo("new image buffer");
        m_threadParam.ImageBuffer = new uint8_t[frameParam.Size];
    if (m_threadParam.Rgb32BufferPool == nullptr)
#if 0
        m_threadParam.Rgb32BufferPool = new PingPongBuffer(frameParam.Height * frameParam.Width * 3);
#else
        qInfo("new Rgb32 buffer");
        m_threadParam.Rgb32BufferPool = new BufferCircularQueue(3, frameParam.Height * frameParam.Width * 3);
#endif
    m_threadParam.FrameParam = frameParam;
}

bool CZGraber::checkIsPlaying()
{
    return m_isPlaying;
}

void CZGraber::getExposureRange(uint *maxExpoVal, uint *minExpoVal)
{
    // The ExposureRange is defined in first line of section [ExposureParams]
    // The first number shall be minExpo, but saved in addr, as the role is (addr,value)...
    *minExpoVal = m_sensorSetting.ExposureParams[0].Addr;
    *maxExpoVal = m_sensorSetting.ExposureParams[0].Value;
}

void CZGraber::setExposureCoarse(int expoCoarse)
{
    QByteArray expoByte;
    expoByte.resize(sizeof(int));
    memcpy(expoByte.data(), &expoCoarse, sizeof(int));
    int expoHigh = expoByte[1];
    int expoLow = expoByte[0];

    // write I2C register
    std::vector<T_RegConf> regList;
    T_RegConf regConf_High;
    regConf_High.Addr = m_sensorSetting.ExposureParams[1].Addr;
    regConf_High.Value = expoHigh;
    regConf_High.Delay_ms = 50;
    regList.push_back(regConf_High);

    T_RegConf regConf_Low;
    regConf_Low.Addr = m_sensorSetting.ExposureParams[2].Addr;
    regConf_Low.Value = expoLow;
    regConf_Low.Delay_ms = 50;
    regList.push_back(regConf_Low);

    m_czDeviceClient->WriteDiscreteI2c(m_sensorSetting.I2cParam.Addr, m_sensorSetting.I2cParam.Speed,m_sensorSetting.I2cParam.RegBitsMode,regList);
}

void CZGraber::readIniFile(QString filename)
{
   filename.replace("file:///", "");
   qInfo("readIniFile: %s", filename.toStdString().c_str());
   if (!m_czIniFile.LoadSensorSettingFromFile(&m_sensorSetting, filename.toStdString().c_str())) {
       qWarning("[CZGraber::readIniFile] Read Ini file fail");
       return;
   }
   qInfo("Chipname : %s", m_sensorSetting.ChipName);
   qInfo("[CZGraber::readIniFile] Read Ini file success");
}

void CZGraber::startCamera()
{
    if (m_czDeviceClient ==  nullptr) {
        throw  SilicolAbort(tr("CZ Grabber is not connected"));
    }

    if (m_sensorSetting.Id < 1) {
        throw  SilicolAbort(tr("Ini file error"));
    }

    if (m_czDeviceClient == nullptr) {
        throw  SilicolAbort(tr("CZ Grabber is not opened"));
    }

    // Power on
    std::vector<T_Power> powerList;
    int count = m_sensorSetting.PowerCount;
    for (int i = 0; i < count; i++) {
        T_Power power;
        power.Id = m_sensorSetting.Powers[i].Id;
        power.Value = m_sensorSetting.Powers[i].Value;
        power.Delay_ms = m_sensorSetting.Powers[i].Delay_ms;
        powerList.push_back(power);
    }
    int ec = m_czDeviceClient->SetSensorPower(powerList);
    if (ec < 0) {
        throw  SilicolAbort(tr("SetSensorPower error"));
    }
    // Sensor clock
    ec = m_czDeviceClient->SetSensorClock(m_sensorSetting.Mclk_kHz / 100);
    if (ec < 0) {
        throw  SilicolAbort(tr("SetSensorClock error"));
    }
    // Gpio
    ushort resetPwdnPin = IO_Reset | IO_Pwdn1;
    ushort resetPwdnLevel = 0;
    if (m_sensorSetting.Reset)
        resetPwdnLevel |= IO_Reset;
    if (m_sensorSetting.Pwdn)
        resetPwdnLevel |= IO_Pwdn1;
    ec = m_czDeviceClient->SetSensorGpioPinLevel(resetPwdnPin, resetPwdnLevel);
    if (ec < 0) {
         throw  SilicolAbort(tr("SetSensorGpioPinLevel error"));
    }
    // Check sensor online
    T_I2CCommParam i2cParam = m_sensorSetting.I2cParam;
    count = m_sensorSetting.FlagRegisterCount;
    std::vector<T_RegConf> regList;
    for (int i = 0; i < count; i++) {
        T_RegConf regConf;
        regConf.Addr = m_sensorSetting.FlagRegisters[i].Addr;
        regConf.Value = 0;
        regConf.Mask = m_sensorSetting.FlagRegisters[i].Mask;
        regList.push_back(regConf);
    }
    ec = m_czDeviceClient->ReadDiscreteI2c(i2cParam.Addr, i2cParam.Speed * 100, i2cParam.RegBitsMode, regList);
    for (int i = 0; i < count; i++) {
        if ((regList[i].Value & m_sensorSetting.FlagRegisters[i].Mask) != (m_sensorSetting.FlagRegisters[i].Value & m_sensorSetting.FlagRegisters[i].Mask))
        {
            throw  SilicolAbort(tr("I2C Read Flag Reg Failed."));
        }
    }

    E_ImageFormat outImageFormat = E_ImageFormat::ImgFmt_RAW10;
    //setOutImageFormat(m_option->m_OutRawFormat, outImageFormat);

    qInfo("ImageFormat: %d ImageMode: %d PixelWidth: %d PixelHeight: %d outputImageFormat: %d",
          m_sensorSetting.ImageFormat, m_sensorSetting.ImageMode,
                  m_sensorSetting.PixelWidth, m_sensorSetting.PixelHeight, outImageFormat);

    ec = m_czDeviceClient->SetFrameParam(m_sensorSetting.ImageFormat, m_sensorSetting.ImageMode,
        m_sensorSetting.PixelWidth, m_sensorSetting.PixelHeight, outImageFormat);
    if (ec < 0) {
        throw  SilicolAbort(tr("Set Pic Format Failed."));
    }
    // Set mipi param
    ec = m_czDeviceClient->SetMipiParam(m_sensorSetting.Lanes, m_sensorSetting.MipiFreq, 0);
    if (ec != 0) {
        throw  SilicolAbort(tr("Set MIPI Param Failed."));
    }
    // Set interface type
    ec = m_czDeviceClient->SetSensorInterface((uint)m_sensorSetting.InterfaceType);
    if (ec != 0) {
        throw  SilicolAbort(tr("Set Interface Type Failed."));
    }

    // Setting full mode registers
    count = m_sensorSetting.FullModeParamCount;
    regList.clear();
    for (int i = 0; i < count; i++) {
        T_RegConf regConf;
        regConf.Addr = m_sensorSetting.FullModeParams[i].Addr;
        regConf.Value = m_sensorSetting.FullModeParams[i].Value;
        regConf.Delay_ms = m_sensorSetting.FullModeParams[i].Delay_ms;
        regList.push_back(regConf);
    }
    ec = m_czDeviceClient->WriteDiscreteI2c(i2cParam.Addr, i2cParam.Speed * 100, i2cParam.RegBitsMode, regList);
    if (ec < 0) {
        throw  SilicolAbort(tr("I2C Write Light Reg Failed."));
    }

    // Start video
    ec = m_czDeviceClient->VideoControl(1);
    if (ec < 0) {
        throw  SilicolAbort(tr("Video On Failed."));
    }

    // Setup buffers
    uint imageFormat, imageMode, width, height, size;
    ec = m_czDeviceClient->GetFrameParam(imageFormat, imageMode, width, height, size);
    if (ec < 0) {
        throw  SilicolAbort(tr("Get Pic Format Failed."));
    }

    m_isPlaying = true;
    T_FrameParam frameParam;
    frameParam.ImageFormat = (E_ImageFormat)imageFormat;
    frameParam.ImageMode = (E_ImageMode)imageMode;
    frameParam.Width = width;
    frameParam.Height = height;
    frameParam.Size = size;
    setupFrameBuffer(frameParam);
    qInfo("End of start camera");
}

QImage CZGraber::grabImage()
{
    QMutexLocker locker(&mutex);
    if(!m_isPlaying) {
        qWarning("Camerais not playing");
        QImage image; return image;
    }
    uint64 headTimestamp = 0;
    if (m_czDeviceClient ==  nullptr) {
        QImage image; return image;
    }
    if (m_sensorSetting.Id < 1) {
        QImage image; return image;
    }
    if (m_czDeviceClient == nullptr) {
        QImage image; return image;
    }
    int ec = ERR_NoError;
    uchar *pImageBuffer = nullptr;
    //uint8_t *pRgbBuf = new uint8_t[m_threadParam.FrameParam.Height * m_threadParam.FrameParam.Width * 3];
    if (pRgbBuf1 == nullptr) {
        pRgbBuf1 = new uint8_t[m_threadParam.FrameParam.Height * m_threadParam.FrameParam.Width * 3];
    }
    uint8_t *pTempRaw8Buf = new uint8_t[m_threadParam.FrameParam.Height * m_threadParam.FrameParam.Width];
    pImageBuffer = m_threadParam.ImageBuffer;
    ec = m_czDeviceClient->GrabLatestFrame(m_threadParam.ImageBuffer, m_threadParam.FrameParam.Size, headTimestamp);
    if (ec < 0) {
        qInfo("GrabLatestFrame failed with error code:%d", ec);
        QImage image; return image;
    }
    if (ec == ERR_NoError)
    {
         decodeRawImage(m_threadParam.FrameParam, Option::OutRawFormat_UnPacked, pImageBuffer, pTempRaw8Buf, pRgbBuf1);
    }
    QImage image(pRgbBuf1, m_threadParam.FrameParam.Width, m_threadParam.FrameParam.Height, QImage::Format_RGB888);
    delete []pTempRaw8Buf;
    //delete []pRgbBuf;
    return image;
}

cv::Mat CZGraber::grabImageCV()
{
    QMutexLocker locker(&mutex);
    uint64 headTimestamp = 0;
    if (m_czDeviceClient ==  nullptr) {
        throw  SilicolAbort(tr("CZ Grabber is not connected"));
    }
    if (m_sensorSetting.Id < 1) {
        throw  SilicolAbort(tr("Ini file error"));
    }
    if (m_czDeviceClient == nullptr) {
        throw  SilicolAbort(tr("CZ Grabber is not opened"));
    }
    int ec = ERR_NoError;
    uchar *pImageBuffer = nullptr;
    if (pRgbBuf2 == nullptr) {
        pRgbBuf2 = new uint8_t[m_threadParam.FrameParam.Height * m_threadParam.FrameParam.Width * 3];
    }
    uint8_t *pTempRaw8Buf = new uint8_t[m_threadParam.FrameParam.Height * m_threadParam.FrameParam.Width];
    pImageBuffer = m_threadParam.ImageBuffer;
    ec = m_czDeviceClient->GrabLatestFrame(m_threadParam.ImageBuffer, m_threadParam.FrameParam.Size, headTimestamp);
    if (ec < 0) {
        qInfo("GrabLatestFrame failed with error code:%d", ec);
        throw  SilicolAbort(tr("GrabLatestFrame Failed."));
    }
    if (ec == ERR_NoError)
    {
         decodeRawImage(m_threadParam.FrameParam, Option::OutRawFormat_UnPacked, pImageBuffer, pTempRaw8Buf, pRgbBuf2);
    }
    cv::Mat img(m_threadParam.FrameParam.Height, m_threadParam.FrameParam.Width, CV_8UC3, pRgbBuf2);

    delete[] pTempRaw8Buf;
    return img;
}

void CZGraber::closeDevice()
{
    if (m_czDeviceClient ==  nullptr) {
        throw  SilicolAbort(tr("CZ Grabber is not connected"));
    }
    if (m_sensorSetting.Id < 1) {
        throw  SilicolAbort(tr("Ini file error"));
    }

    if (m_czDeviceClient == nullptr) {
        throw  SilicolAbort(tr("CZ Grabber is not opened"));
    }

    // Stop video
    int ec = this->m_czDeviceClient->VideoControl(0);

    m_czDeviceClient->SetSensorGpioPinLevel(IO_Pwdn1, 0);
    Sleep(5);
    m_czDeviceClient->SetSensorGpioPinLevel(IO_Reset, 0);
    Sleep(5);
    m_czDeviceClient->SetSensorClock(0);
    Sleep(5);
    // Power off
    std::vector<T_Power> powerList;
    for (int i = 0; i < m_sensorSetting.PowerCount; i++) {
        T_Power power;
        power.Id = m_sensorSetting.Powers[i].Id;
        power.Value = 0;
        power.Delay_ms = m_sensorSetting.Powers[i].Delay_ms;
        powerList.push_back(power);
    }
    ec = m_czDeviceClient->SetSensorPower(powerList);
    m_isPlaying = false;
    qInfo("Video Has Been Off.");
}

int CZGraber::decodeRawImage(const T_FrameParam &frameParam, Option::E_OutRawFormat imageFormat, const uchar *pSrcBuf, uchar *pTempRaw8, uchar *pRgb32Buf)
{
    ImageProc imageProc;
    switch (imageFormat) {
        case Option::OutRawFormat_Packed: {
            qInfo("OutRawFormat_Packed");
            imageProc.PakcedRaw10ToRaw8(pSrcBuf, pTempRaw8, frameParam.Width, frameParam.Height);
            imageProc.Raw8ToRGB24(pTempRaw8, pRgb32Buf, (ImageProc::EBayerPattern)frameParam.ImageMode, frameParam.Width, frameParam.Height);
            break;
        }
        case Option::OutRawFormat_UnPacked: {
            imageProc.RawToRaw8((const ushort *)pSrcBuf, pTempRaw8, frameParam.Width, frameParam.Height, 2);
            imageProc.Raw8ToRGB24(pTempRaw8, pRgb32Buf, (ImageProc::EBayerPattern)frameParam.ImageMode, frameParam.Width, frameParam.Height);
            break;
        }
        case Option::OutRawFormat_Raw8: {
            imageProc.Raw8ToRGB24(pSrcBuf, pRgb32Buf, (ImageProc::EBayerPattern)frameParam.ImageMode, frameParam.Width, frameParam.Height);
            break;
        }
        default:
            break;
    }
    return 0;
}

