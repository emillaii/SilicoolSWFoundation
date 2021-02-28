#include "czSdkWrapper.h"

/////////////////////////////////// ÒÔÏÂSDKÎªÄÚ²¿Òþ²ØSDK ///////////////////////////////////

extern "C"
{
    namespace CmtiSdk
    {
        /**
        * @brief ÉèÖÃTest Pattern²ÎÊý
        * @param patternMode Ä£Ê½(Ä¬ÈÏ0Îª¹Ø±Õ)
        * @param patternFps  Ä£ÄâÊä³öFPS
        * @return int ´íÎóÂë
        */
        CMTISDK_API int Cmti_SetTestPattern(HDeviceClient hDevCli, uint16_t patternMode, uint16_t patternFps);

        /**
        * @brief »ñÈ¡Test Pattern²ÎÊý
        * @param [out]patternMode Ä£Ê½(Ä¬ÈÏ0Îª¹Ø±Õ)
        * @param [out]patternFps  Ä£ÄâÊä³öFPS
        * @return int ´íÎóÂë
        */
        CMTISDK_API int Cmti_GetTestPattern(HDeviceClient hDevCli, uint16_t *patternMode, uint16_t *patternFps);

    }
}

DeviceController *DeviceController::m_instance = NULL;
DeviceController *DeviceController::GetInstance()
{
    //CmtiSdk::Cmti_DisableNonCztekNic();
    if (m_instance == NULL) {
        m_instance = new DeviceController();
    }
    return m_instance;
}

void DeviceController::FreeInstance()
{
    if (m_instance != NULL) {
        delete m_instance;
        m_instance = NULL;
    }
}

DeviceController::DeviceController()
{
    CmtiSdk::Cmti_GetDeviceController(&m_deviceController);
}

DeviceController::~DeviceController()
{
    CmtiSdk::Cmti_DestroyDeviceController(m_deviceController);
    m_deviceController = NULL;
}

DeviceClient *DeviceController::GetDeviceClient(const std::string & devName)
{
    CmtiSdk::HDeviceClient hDevCli = nullptr;
    CmtiSdk::Cmti_GetDeviceClient(m_deviceController, devName.c_str(), &hDevCli);
    if (m_mapHandle2DeviceClient.find(hDevCli) == m_mapHandle2DeviceClient.end()) {
        m_mapHandle2DeviceClient.insert(std::make_pair(hDevCli, new DeviceClient(hDevCli)));
    }
    return m_mapHandle2DeviceClient[hDevCli];
}

void DeviceController::ToggleDiscovery()
{
    CmtiSdk::Cmti_ToggleDiscovery(m_deviceController);
}

int DeviceController::EnumerateDevice(std::vector<std::string> &deviceNameList)
{
    const int MAX_SUPPORT_DEV_NUM = 8;
    char *pDeviceNameList[MAX_SUPPORT_DEV_NUM] = { NULL };
    for (int i = 0; i < MAX_SUPPORT_DEV_NUM; i++)
        pDeviceNameList[i] = new char[64];
    int devNum = MAX_SUPPORT_DEV_NUM;
    int ec = CmtiSdk::Cmti_EnumerateDevice(m_deviceController, pDeviceNameList, &devNum);
    if (ERR_NoError == ec)
    {
        for (int i = 0; i < devNum; i++)
            deviceNameList.push_back(std::string(pDeviceNameList[i]));
    }
    for (int i = 0; i < MAX_SUPPORT_DEV_NUM; i++)
        delete[] pDeviceNameList[i];
    return ec;
}

int DeviceController::GetDevicePropList(const std::string &devName, std::map<std::string, std::string> &propMap)
{
    const int MAX_SUPPORTED_PROP_NUM = 30;
    char *propNameList[MAX_SUPPORTED_PROP_NUM] = { NULL };
    char *propValList[MAX_SUPPORTED_PROP_NUM]  = { NULL };
    for (int i = 0; i < MAX_SUPPORTED_PROP_NUM; i++)
    {
        propNameList[i] = new char[32];
        propValList[i]  = new char[256];
    }
    int propNum = MAX_SUPPORTED_PROP_NUM;
    int ec = CmtiSdk::Cmti_GetDevicePropList(m_deviceController, devName.c_str(), propNameList, propValList, &propNum);
    if (ERR_NoError == ec)
    {
        for (int i = 0; i < propNum; i++)
            propMap.insert(std::make_pair(std::string(propNameList[i]), std::string(propValList[i])));
    }
    for (int i = 0; i < MAX_SUPPORTED_PROP_NUM; i++)
    {
        delete[] propNameList[i];
        delete[] propValList[i];
    }
    return ec;
}

DeviceClient::DeviceClient(CmtiSdk::HDeviceClient hDevClient) :
    m_hDevClient(hDevClient)
{}

DeviceClient::~DeviceClient()
{}

//int DeviceClient::GetDeviceProperty(const std::string& propName, std::string& propVal) const
//{
//    char szPropVal[64];
//    int ec = CmtiSdk::Cmti_GetDevicePropertyByHandle(m_hDevClient, propName.c_str(), szPropVal, sizeof(szPropVal));
//    propVal = std::string(szPropVal);
//    return ec;
//}

int DeviceClient::GetCaptureFps(float &fps)
{
    return CmtiSdk::Cmti_GetCaptureFps(m_hDevClient, &fps);
}

int DeviceClient::GetTransmitFps(float &fps)
{
    return CmtiSdk::Cmti_GetTransmitFps(m_hDevClient, &fps);
}

int DeviceClient::GetReceiveFps(float &fps)
{
    return CmtiSdk::Cmti_GetReceiveFps(m_hDevClient, &fps);
}

int DeviceClient::GetTransmitFer(float &fer)
{
    return CmtiSdk::Cmti_GetTransmitFer(m_hDevClient, &fer);
}

//int DeviceClient::SearchI2cAddress(uint8_t u8I2cAddr[], int& nCount)
//{
//    return CmtiSdk::Cmti_SearchI2cAddress(m_hDevClient, u8I2cAddr, &nCount);
//}
//
//int DeviceClient::WriteSingleI2c(uint32_t i2cAddr, uint32_t speedkHz, uint32_t mode, uint32_t regAddr, uint32_t regData)
//{
//    return CmtiSdk::Cmti_WriteSingleI2c(m_hDevClient, i2cAddr, speedkHz, mode, regAddr, regData);
//}
//
//int DeviceClient::ReadSingleI2c(uint32_t i2cAddr, uint32_t speedkHz, uint32_t mode, uint32_t regAddr, uint32_t& regData)
//{
//    return CmtiSdk::Cmti_ReadSingleI2c(m_hDevClient, i2cAddr, speedkHz, mode, regAddr, &regData);
//}

int DeviceClient::WriteDiscreteI2c(uint i2cAddr, uint speedkHz, uint mode, const std::vector<T_RegConf> &regList)
{
    int count = static_cast<int>(regList.size());
    uint *regAddr  = new uint[count];
    uint *regVal   = new uint[count];
    uint *regDelay = new uint[count];
    for (int i = 0; i < count; i++)
    {
        regAddr[i]  = regList[i].Addr;
        regVal[i]   = regList[i].Value;
        regDelay[i] = regList[i].Delay_ms;
    }
    int ec = CmtiSdk::Cmti_WriteDiscreteI2c(m_hDevClient, i2cAddr, speedkHz, mode, regAddr, regVal, regDelay, count);
    delete[] regAddr;
    delete[] regVal;
    delete[] regDelay;

    return ec;
}

int DeviceClient::ReadDiscreteI2c(uint i2cAddr, uint speedkHz, uint mode, std::vector<T_RegConf> &regList)
{
    size_t count = regList.size();
    uint *regAddr = new uint[count];
    uint *regVal  = new uint[count];
    for (size_t i = 0; i < count; i++)
    {
        regAddr[i] = regList[i].Addr;
        regVal[i]  = 0;
    }
    int ec = CmtiSdk::Cmti_ReadDiscreteI2c(m_hDevClient, i2cAddr, speedkHz, mode, regAddr, regVal, (int)count);
    for (size_t i = 0; i < count; i++)
    {
        regList[i].Value = regVal[i];
    }
    delete[] regAddr;
    delete[] regVal;

    return ec;
}

int DeviceClient::WriteContinuousI2c(uint i2cAddr, uint speedkHz, uint regAddr, uint regAddrSize,
                                     const uchar *data, uint dataSize)
{
    int ec = CmtiSdk::Cmti_WriteContinuousI2c(m_hDevClient, i2cAddr, speedkHz, regAddr, regAddrSize, data, dataSize);
    return ec;
}

int DeviceClient::ReadContinuousI2c(uint i2cAddr, uint speedkHz, uint regAddr, uint regAddrSize,
                                    uchar *data, uint dataSize)
{
    int ec = CmtiSdk::Cmti_ReadContinuousI2c(m_hDevClient, i2cAddr, speedkHz, regAddr, regAddrSize, data, dataSize);
    return ec;
}

int DeviceClient::SetSensorClock(uint clk100kHz)
{
    return CmtiSdk::Cmti_SetSensorClock(m_hDevClient, clk100kHz);
}

int DeviceClient::SetSensorInterface(uint intf)
{
    return CmtiSdk::Cmti_SetSensorInterface(m_hDevClient, intf);
}

int DeviceClient::SetMipiParam(uint laneNum, uint freqMHz, uint virtualChannel)
{
    return CmtiSdk::Cmti_SetMipiParam(m_hDevClient, laneNum, freqMHz, virtualChannel);
}

int DeviceClient::SetSensorGpioPinLevel(ushort pin, ushort level)
{
    return CmtiSdk::Cmti_SetSensorGpioPinLevel(m_hDevClient, pin, level);
}

int DeviceClient::SetSensorGpioPinDir(ushort pin, ushort dir)
{
    return CmtiSdk::Cmti_SetSensorGpioPinDir(m_hDevClient, pin, dir);
}

int DeviceClient::SetSensorPower(const std::vector<T_Power> &powerList)
{
    size_t count = powerList.size();
    uint *powerId = new uint[count];
    uint *voltage = new uint[count];
    uint *delay = new uint[count];
    for (size_t i = 0; i < count; i++)
    {
        powerId[i] = powerList[i].Id;
        voltage[i] = powerList[i].Value;
        delay[i] = powerList[i].Delay_ms;
    }
    int ec = CmtiSdk::Cmti_SetSensorPower(m_hDevClient, powerId, voltage, delay, (int)count);
    delete[] powerId;
    delete[] voltage;
    delete[] delay;
    return ec;
}

int DeviceClient::GetSensorPower(std::vector<T_Power> &powerList)
{
    size_t count = powerList.size();
    uint *powerId = new uint[count];
    uint *voltage = new uint[count];
    for (size_t i = 0; i < count; i++)
    {
        powerId[i] = powerList[i].Id;
        voltage[i] = 0;
    }
    int ec = CmtiSdk::Cmti_GetSensorPower(m_hDevClient, powerId, voltage, (int)count);
    for (size_t i = 0; i < count; i++)
    {
        powerList[i].Value = voltage[i];
    }
    delete[] powerId;
    delete[] voltage;
    return ec;
}

int DeviceClient::GetSensorFeedBackVoltage(std::vector<T_Power> &powerList)
{
    size_t count = powerList.size();
    int *powerId = new int[count];
    int *voltage = new int[count];
    for (size_t i = 0; i < count; i++)
    {
        powerId[i] = powerList[i].Id;
        voltage[i] = 0;
    }
    int ec = CmtiSdk::Cmti_GetSensorFeedBackVoltage(m_hDevClient, powerId, voltage, (int)count);
    for (size_t i = 0; i < count; i++)
    {
        powerList[i].Value = voltage[i];
    }
    delete[] voltage;
    delete[] powerId;
    return ec;
}

int DeviceClient::SetFrameParam(uint imgFmt, uint imgMode, uint width, uint height, uint outImgFmt)
{
    return CmtiSdk::Cmti_SetFrameParam(m_hDevClient, imgFmt, imgMode, width, height, outImgFmt);
}

int DeviceClient::GetFrameParam(uint &imgFmt, uint &imgMode, uint &width, uint &height, uint &size)
{
    return CmtiSdk::Cmti_GetFrameParam(m_hDevClient, &imgFmt, &imgMode, &width, &height, &size);
}

int DeviceClient::SetRoiParam(const T_Rect roiRect[], uint roiCount)
{
    return CmtiSdk::Cmti_SetRoiParam(m_hDevClient, roiRect, roiCount);
}

int DeviceClient::VideoControl(uint ctrl)
{
    return CmtiSdk::Cmti_VideoControl(m_hDevClient, ctrl);
}

int DeviceClient::SetGrabTimeout(uint grabTimeout)
{
    return CmtiSdk::Cmti_SetGrabTimeout(m_hDevClient, grabTimeout);
}

int DeviceClient::GetSystemTimestamp(uint64 &timestamp)
{
    return CmtiSdk::Cmti_GetSystemTimestamp(m_hDevClient, &timestamp);
}

int DeviceClient::SkipFrame(int count)
{
    return CmtiSdk::Cmti_SkipFrame(m_hDevClient, count);
}

int DeviceClient::GrabFrame(uchar *pbuffer, int bufferLen, uint64 & timestamp)
{
    return CmtiSdk::Cmti_GrabFrame(m_hDevClient, pbuffer, bufferLen, &timestamp);
}

int DeviceClient::GrabLatestFrame(uchar *pbuffer, int bufferLen, uint64 & timestamp)
{
    return CmtiSdk::Cmti_GrabLatestFrame(m_hDevClient, pbuffer, bufferLen, &timestamp);
}

int DeviceClient::DequeueFrameBuffer(int & bufIdx, uchar * & pbuffer, uint64 & timestamp)
{
    return CmtiSdk::Cmti_DequeueFrameBuffer(m_hDevClient, &bufIdx, &pbuffer, &timestamp);
}

int DeviceClient::DequeueFrameBuffer2(int & bufIdx, uchar * & pbuffer, uint64 &headTimestamp, uint64 &tailTimestamp, uint32 &frameSequence)
{
    return CmtiSdk::Cmti_DequeueFrameBuffer2(m_hDevClient, &bufIdx, &pbuffer, &headTimestamp, &tailTimestamp, &frameSequence);
}

int DeviceClient::DequeueLatestFrameBuffer(int & bufIdx, uchar * & pbuffer, uint64 & timestamp)
{
    return CmtiSdk::Cmti_DequeueLatestFrameBuffer(m_hDevClient, &bufIdx, &pbuffer, &timestamp);
}

int DeviceClient::EnqueueFrameBuffer(int bufIdx)
{
    return CmtiSdk::Cmti_EnqueueFrameBuffer(m_hDevClient, bufIdx);
}

int DeviceClient::SetOsTestConfig(uint supplyVol, uint supplyCurrent, const uint pinId[],
                                  const uint openStdVol[], const uint shortStdVol[], uint count)
{
    return CmtiSdk::Cmti_SetOsTestConfig(m_hDevClient, supplyVol, supplyCurrent, pinId, openStdVol, shortStdVol, count);
}

int DeviceClient::SetOsTestCurrentDirection(bool positive)
{
    return CmtiSdk::Cmti_SetOsTestCurrentDirection(m_hDevClient, positive);
}

int DeviceClient::ReadOsTestResult(const uint pinId[], uint openVol[], uint shortVol[], uint result[],
                                   uint count)
{
    return CmtiSdk::Cmti_ReadOsTestResult(m_hDevClient, pinId, openVol, shortVol, result, count);
}

int DeviceClient::GetCurrent(const uint powerId[], const uint currentRange[], float current[], uint count)
{
    return CmtiSdk::Cmti_GetCurrent(m_hDevClient, powerId, currentRange, current, count);
}

int DeviceClient::GetCurrentV2(const int powerIds[], const int upperLimit[], const uint16 atuoHighPrecision[], float current_nA[], int count)
{
    return CmtiSdk::Cmti_GetCurrentV2(m_hDevClient, powerIds, upperLimit, atuoHighPrecision, current_nA, count);
}

int DeviceClient::SetOvercurrentParam(const int powerId[], const int currentThrd_mA[], const int debounceInterval_ms[], int count)
{
    return CmtiSdk::Cmti_SetOvercurrentParam(m_hDevClient, powerId, currentThrd_mA, debounceInterval_ms, count);
}

int DeviceClient::GetOvercurrentParam(const int powerId[], int currentThrd_mA[], int debounceInterval_ms[], int count)
{
    return CmtiSdk::Cmti_GetOvercurrentParam(m_hDevClient, powerId, currentThrd_mA, debounceInterval_ms, count);
}

int DeviceClient::QueryOvercurrent(int powerId[], int &count)
{
    return CmtiSdk::Cmti_QueryOvercurrent(m_hDevClient, powerId, &count);
}

int DeviceClient::SetBeepOn(uint delay)
{
    return CmtiSdk::Cmti_SetBeepOn(m_hDevClient, delay);
}

int DeviceClient::ImageProcessingControl(uint ctrl, uint pluginID)
{
    return ERR_NotImplemented;
}

int DeviceClient::SetImageProcessingParam(uint pluginID, void *param, int size)
{
    return ERR_NotImplemented;
}

int DeviceClient::SendUpgradeFile(const char *filePathName)
{
    return CmtiSdk::Cmti_SendUpgradeFile(m_hDevClient, filePathName);
}

int DeviceClient::PowerPinOsTest(const int powerId[], const int supplyVoltage_mV[], const int upperLimitCurrent_nA[], const int autoHighPrecision[], int leakCurrent_nA[], int count)
{
    return CmtiSdk::Cmti_PowerPinOsTest(m_hDevClient, powerId, supplyVoltage_mV, upperLimitCurrent_nA, autoHighPrecision, leakCurrent_nA, count);
}

int DeviceClient::SetI2cPullupResistor(int sclPullupResistor, int sdaPullupResistor)
{
    return CmtiSdk::Cmti_SetI2cPullupResistor(m_hDevClient, sclPullupResistor, sdaPullupResistor);
}

int DeviceClient::GetI2cPullupResistor(int &sclPullupResistor, int &sdaPullupResistor)
{
    return CmtiSdk::Cmti_GetI2cPullupResistor(m_hDevClient, &sclPullupResistor, &sdaPullupResistor);
}

int DeviceClient::SetMipiClockMode(bool discontinuous)
{
    return CmtiSdk::Cmti_SetMipiClockMode(m_hDevClient, discontinuous);
}

int DeviceClient::GetMipiClockMode(bool &discontinuous)
{
    return CmtiSdk::Cmti_GetMipiClockMode(m_hDevClient, &discontinuous);
}

int DeviceClient::SetTestPattern(uint16_t mode, uint16_t fps)
{
    return CmtiSdk::Cmti_SetTestPattern(m_hDevClient, mode, fps);
}

int DeviceClient::GetTestPattern(uint16_t &mode, uint16_t &fps)
{
    return CmtiSdk::Cmti_GetTestPattern(m_hDevClient, &mode, &fps);
}

int DeviceClient::SetTransmitDelay(uint delay)
{
    return CmtiSdk::Cmti_SetTransmitDelay(m_hDevClient, delay);
}

int DeviceClient::GetTransmitDelay(uint &delay)
{
    return CmtiSdk::Cmti_GetTransmitDelay(m_hDevClient, &delay);
}
