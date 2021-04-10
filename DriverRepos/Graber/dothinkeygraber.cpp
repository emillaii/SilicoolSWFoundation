#include "dothinkeygraber.h"
#include <fstream>

bool DothinkeyGraber::CameraChannel::CloseCameraChannel()
{
    if (this->m_iDevID>= 0) {
        CloseDevice(m_iDevID);
        return true;
    }
    return false;
}

DothinkeyGraber::DothinkeyGraber(QObject *parent) : SCGraber(parent)
{
    for (int i = 0; i < MAX_DEV; i++)
    {
        m_devName[i] = new char[1024];
    }
}

DothinkeyGraber::~DothinkeyGraber()
{
    for (int i = 0; i < MAX_DEV; i++) {
        delete m_devName[i];
    }
}

void DothinkeyGraber::readIniFile(QString iniFilename)
{
    iniFilename.replace("file:///", "");
    qInfo()<< iniFilename;
    SensorTab *pCurrentSensor;
    pCurrentSensor = &(this->m_CameraChannels[m_devID].current_sensor);
    iniParser *iniParser_ = new iniParser();
    iniParser_->SetIniFilename(iniFilename.toUtf8().data());
    m_CameraChannels[m_devID].m_fMclk = (float)iniParser_->ReadIniData("Sensor", "mclk", 0x01) / 1000;
    m_CameraChannels[m_devID].m_fAvdd = (float)iniParser_->ReadIniData("Sensor", "avdd", 0x00) / 1000;
    m_CameraChannels[m_devID].m_fDovdd = (float)iniParser_->ReadIniData("Sensor", "dovdd", 0x00) / 1000;
    m_CameraChannels[m_devID].m_fDvdd = (float)iniParser_->ReadIniData("Sensor", "dvdd", 0x00) / 1000;
    m_CameraChannels[m_devID].m_iPhyType = iniParser_->ReadIniData("Sensor", "PhyType", 0);
    m_CameraChannels[m_devID].m_iLanes = iniParser_->ReadIniData("Sensor", "LANES", 0);

    pCurrentSensor->width = iniParser_->ReadIniData("Sensor", "width", 0);
    pCurrentSensor->height = iniParser_->ReadIniData("Sensor", "height", 0);
    pCurrentSensor->type = iniParser_->ReadIniData("Sensor", "type", 2);
    pCurrentSensor->port = iniParser_->ReadIniData("Sensor", "port", 0);
    pCurrentSensor->pin = iniParser_->ReadIniData("Sensor", "pin", 0);
    pCurrentSensor->SlaveID = iniParser_->ReadIniData("Sensor", "SlaveID", 0);
    pCurrentSensor->mode = iniParser_->ReadIniData("Sensor", "mode", 0);
    pCurrentSensor->FlagReg = iniParser_->ReadIniData("Sensor", "FlagReg", 0);
    pCurrentSensor->FlagMask = iniParser_->ReadIniData("Sensor", "FlagMask", 0xff);
    pCurrentSensor->FlagData = iniParser_->ReadIniData("Sensor", "FlagData", 0);
    pCurrentSensor->FlagReg1 = iniParser_->ReadIniData("Sensor", "FlagReg1", 0);
    pCurrentSensor->FlagMask1 = iniParser_->ReadIniData("Sensor", "FlagMask1", 0x0);
    pCurrentSensor->FlagData1 = iniParser_->ReadIniData("Sensor", "FlagData1", 0);
    pCurrentSensor->outformat = iniParser_->ReadIniData("Sensor", "outformat", 0x00);
    pCurrentSensor->mclk = iniParser_->ReadIniData("Sensor", "mclk", 0x01);
    pCurrentSensor->avdd = iniParser_->ReadIniData("Sensor", "avdd", 0x00);
    pCurrentSensor->dovdd = iniParser_->ReadIniData("Sensor", "dovdd", 0x00);
    pCurrentSensor->dvdd = iniParser_->ReadIniData("Sensor", "dvdd", 0x00);

    pCurrentSensor->ParaList = new USHORT[8192 * 4];
    pCurrentSensor->ParaListSize = 0;
    pCurrentSensor->SleepParaList = new USHORT[8192 * 4];
    pCurrentSensor->SleepParaListSize = 0;
    cmd_list.clear();
    otp_list.clear();
    iniParser_->GetI2CData(pCurrentSensor, cmd_list, otp_list);
    delete iniParser_;
}

BOOL DothinkeyGraber::DothinkeyIsGrabbing()
{
    return m_isGrabbing;
}

void DothinkeyGraber::enumerateDevice()
{
    int deviceNum = 0;
    int ret = EnumerateDevice(m_devName, MAX_DEV, &deviceNum);
    m_devNum = deviceNum;
    if (DT_ERROR_OK != ret)
    {
        throw  SilicolAbort(tr("enumerateDevice command failed!"));
    }
    if (deviceNum == 0)
    {
        throw  SilicolAbort(tr("Dothinkey cannot find device!"));
    }
    if (deviceNum == 1) m_currentDevName = m_devName[0];
    for (int i = 0; i < deviceNum; i++)
    {
        if (m_devName[i] != nullptr)
        {
            QString str = "DothinkeyGraber Device Found: ";
            str.append(m_devName[i]);
            qInfo(str.toStdString().c_str());
        }
    }
}

void DothinkeyGraber::openDevice()
{
    if(m_devName[m_devID] == nullptr)
    {
        throw SilicolAbort(tr("device name is null! please enumerate device first!"));
    }
    if (m_devName[m_devID] != m_currentDevName)
    {
        throw SilicolAbort(tr("device name mismatch!"));
    }

    m_CameraChannels[m_devID].CloseCameraChannel();
    int iDevID = -1;
    if (OpenDevice(m_devName[m_devID], &iDevID, m_devID) == DT_ERROR_OK)
    {
        m_isOpened = true;
        qInfo("Open device success");
    }
    else
    {
        m_isOpened = false;
        throw SilicolAbort(tr("Open device failed!"));
    }

    BYTE pSN[32];
    int iBufferSize = 32;
    int pRetLen = 0;
    if (GetDeviceSN(pSN, iBufferSize, &pRetLen, iDevID) == DT_ERROR_OK)
    {
        std::string s(reinterpret_cast<const char *>(pSN), 32);
        if (s.length() > 0) {
            qInfo("Device SN:%s", s.data());
            m_CameraChannels[m_devID].m_iDevID = iDevID;
        }
    }
}

void DothinkeyGraber::closeDevice()
{
    m_isOpened = false;
    m_CameraChannels[m_devID].CloseCameraChannel();
}

void DothinkeyGraber::startCamera()
{
    int res;
    SensorTab *pSensor = nullptr;
    ULONG *grabSize = nullptr;
    int iDevID = -1;
    float fMclk = 0;
    float fAvdd = 0;
    float fDvdd = 0;
    float fDovdd = 0;
    float fAfvcc = 0;
    UINT vpp;
    pSensor = &(m_CameraChannels[m_devID].current_sensor);
    iDevID = m_CameraChannels[m_devID].m_iDevID;
    grabSize = &(m_CameraChannels[m_devID].m_GrabSize);
    fMclk = m_CameraChannels[m_devID].m_fMclk;
    fAvdd = m_CameraChannels[m_devID].m_fAvdd;
    fDvdd = m_CameraChannels[m_devID].m_fDvdd;
    fDovdd = m_CameraChannels[m_devID].m_fDovdd;
    fAfvcc = m_CameraChannels[m_devID].m_fAfvcc;
    vpp = m_CameraChannels[m_devID].m_vpp;

    MipiCtrlEx_t sMipiCtrlEx;
    int iRet = GetMipiCtrlEx(&sMipiCtrlEx, iDevID);
    if (iRet != DT_ERROR_OK)
    {
        throw SilicolAbort(tr("GetMipiCtrlEx fail!"));
    }
    qInfo("[DothinkeyStartCamera] Set LaneCnt: %d Set PhyType: %d", m_CameraChannels[m_devID].m_iLanes, m_CameraChannels[m_devID].m_iPhyType);
    sMipiCtrlEx.byLaneCnt = m_CameraChannels[m_devID].m_iLanes;
    sMipiCtrlEx.byPhyType = m_CameraChannels[m_devID].m_iPhyType;
    iRet = SetMipiCtrlEx(&sMipiCtrlEx, iDevID);
    if (iRet != DT_ERROR_OK)
    {
        throw SilicolAbort(tr("SetMipiCtrlEx fail!"));
    }

    SetSoftPinPullUp(IO_NOPULL, 0);
    if (SetSensorClock(false, (USHORT)(0 * 10), iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        throw SilicolAbort(tr("Set sensor clock failed!"));
    }
    Sleep(1);

    if (SetVoltageMclk(*pSensor, iDevID, fMclk, fAvdd, fDvdd, fDovdd, fAfvcc, vpp) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        throw SilicolAbort(tr("Set Voltage and Mclk failed!"));
    }
    qInfo("[DothinkeyStartCamera] Start Camera Success!");

    //I2C init
    res = SetSensorI2cRate(I2C_400K, iDevID);
    qDebug("[DothinkeyStartCamera]SetSensorI2cRate(I2C_400K, iDevID) = %d",res);
    res = SetSensorI2cRapid(false, iDevID);
    qDebug("[DothinkeyStartCamera]SetSensorI2cRapid(TRUE, iDevID) = %d",res);
    SetMipiImageVC(0, true, 1, iDevID);
    //check sensor is on line or not ,if on line,init sensor to work....
    res =SensorEnable(pSensor->pin ^ 0x02, 1, iDevID); //reset
    qDebug("[DothinkeyStartCamera]SensorEnable(pSensor->pin ^ 0x02, 1, iDevID) = %d",res);
    Sleep(20);
    res =SensorEnable(pSensor->pin, 1, iDevID);
    qDebug("[DothinkeyStartCamera]SensorEnable(pSensor->pin, 1, iDevID) = %d",res);
    Sleep(50);

    //check sensor is on line...
//    res = SensorIsMe(pSensor, CHANNEL_A, 0, iDevID);
//    qDebug("[DothinkeyStartCamera]SensorIsMe(pSensor, CHANNEL_A, 0, iDevID) = %d",res);
//    if  (res != DT_ERROR_OK)
//    {
//        throw SilicolAbort(tr("Sensor is not ok!"));
//    }
    //init sensor....
    int result = InitSensor(pSensor->SlaveID,
            pSensor->ParaList,
            pSensor->ParaListSize,
            pSensor->mode, iDevID);
    if (result != DT_ERROR_OK)
    {
        qCritical("[DothinkeyStartCamera] Init Sensor Failed! size: %d mode: %d  result: %d\r\n", pSensor->ParaListSize, pSensor->mode, result);
        throw SilicolAbort(tr("Init Sensor Failed!"));

    }

    if (pSensor->type == D_YUV || pSensor->type == D_YUV_SPI || pSensor->type == D_YUV_MTK_S)
        SetYUV422Format(pSensor->outformat, iDevID);
    else
        SetRawFormat(pSensor->outformat, iDevID);
    res = InitRoi(0, 0, pSensor->width, pSensor->height, 0, 0, 1, 1, pSensor->type, TRUE, iDevID);
    qDebug("[DothinkeyStartCamera]InitRoi(0, 0, pSensor->width, pSensor->height, 0, 0, 1, 1, pSensor->type, TRUE, iDevID) = %d",res);
    res = SetSensorPort(pSensor->port, pSensor->width, pSensor->height, iDevID);
    qDebug("[DothinkeyStartCamera]SetSensorPort(pSensor->port, pSensor->width, pSensor->height, iDevID) = %d",res);
    res = CalculateGrabSize(grabSize, iDevID);
    qDebug("[DothinkeyStartCamera]CalculateGrabSize(grabSize, iDevID) = %d",res);
    //open video....
    res = OpenVideo(*grabSize, iDevID);
    qDebug("[DothinkeyStartCamera]OpenVideo(*grabSize, iDevID) = %d",res);
    //SetMonoMode(true, iDevID);
    //InitDisplay and InitIsp are required to call in order to capture frame
    res = InitDisplay(nullptr, pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, NULL, iDevID);
    qDebug("[DothinkeyStartCamera]InitDisplay(nullptr, pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, NULL, iDevID) = %d",res);
    res = InitIsp(pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, iDevID);
    qDebug("[DothinkeyStartCamera]InitIsp(pSensor->width, pSensor->height, pSensor->type, CHANNEL_A, iDevID) = %d",res);
    m_isGrabbing = true;
}

QImage DothinkeyGraber::grabImage()
{
    QMutexLocker tmplocker(&locker);

    SensorTab *pSensor = nullptr;
    ULONG retSize = 0;
    int iDevID = -1;
    UINT crcCount = 0;
    int grabSize;
    pSensor = &(m_CameraChannels[m_devID].current_sensor);
    iDevID = m_CameraChannels[m_devID].m_iDevID;
    grabSize = m_CameraChannels[m_devID].m_GrabSize;

    USHORT width = pSensor->width;
    USHORT height = pSensor->height;
    //BYTE type = pSensor->type;
    FrameInfo frameInfo;
    static LPBYTE bmpBuffer = (LPBYTE)malloc(width * height * 4);
    //allocate the bmp buffer.
    UINT nSize = width * height * 3 + 1024 * 1024;
    LPBYTE CameraBuffer = NULL;
    CameraBuffer = (LPBYTE)malloc(nSize);
    if ((CameraBuffer == NULL))
    {
        QImage image; return image;
    }
    memset(CameraBuffer, 0, nSize);
    int ret = GrabFrame(CameraBuffer, grabSize, &retSize, &frameInfo, iDevID);
    if (ret == DT_ERROR_OK)
    {
        GetMipiCrcErrorCount(&crcCount, CHANNEL_A, iDevID);
    }
    ImageProcess(CameraBuffer, bmpBuffer, width, height, &frameInfo, iDevID);
    QImage image = QImage(bmpBuffer, width, height, QImage::Format_RGB888);
    delete(CameraBuffer);
    CameraBuffer = NULL;
    return image;
}

cv::Mat DothinkeyGraber::grabImageCV()
{
    QMutexLocker tmplocker(&locker);

    //grabRet = true;
    SensorTab *pSensor = nullptr;
    ULONG retSize = 0;
    int iDevID = -1;
    UINT crcCount = 0;
    int grabSize;
    pSensor = &(m_CameraChannels[m_devID].current_sensor);
    iDevID = m_CameraChannels[m_devID].m_iDevID;
    grabSize = m_CameraChannels[m_devID].m_GrabSize;

    USHORT width = pSensor->width;
    USHORT height = pSensor->height;
    //BYTE type = pSensor->type;
    FrameInfo frameInfo;
    static LPBYTE bmpBuffer = (LPBYTE)malloc(width * height * 4);
    //allocate the bmp buffer.
    UINT nSize = width * height * 3 + 1024 * 1024;
    LPBYTE CameraBuffer = NULL;
    CameraBuffer = (LPBYTE)malloc(nSize);
    if ((CameraBuffer == NULL))
    {
        qInfo("CameraBuffer is Null");
        cv::Mat img;
        //grabRet = false;
        return img;
    }
    memset(CameraBuffer, 0, nSize);
    int ret = GrabFrame(CameraBuffer, grabSize, &retSize, &frameInfo, iDevID);
    if (ret == DT_ERROR_OK)
    {
        GetMipiCrcErrorCount(&crcCount, CHANNEL_A, iDevID);
    } else {
        qInfo("Camera Grab Frame Fail, GrabFrame() returned error code: %d", ret);
        cv::Mat img;
        //grabRet = false;
    }
    ImageProcess(CameraBuffer, bmpBuffer, width, height, &frameInfo, iDevID);
    cv::Mat img(height, width, CV_8UC3, bmpBuffer);
    delete(CameraBuffer);
    CameraBuffer = NULL;
    return img;
}

void DothinkeyGraber::setDeviceName(QString deviceName)
{
    for (int i = 0; i < m_devNum; i++)
    {
        QString tmp = reinterpret_cast<char*>(m_devName[i]);
        if (deviceName == tmp)
        {
            m_devID = i;
        }
    }
}

BOOL DothinkeyGraber::SetVoltageMclk(SensorTab CurrentSensor, int iDevID, float Mclk, float Avdd, float Dvdd, float Dovdd, float Afvcc, float vpp)
{
    SENSOR_POWER Power[10] = { POWER_AVDD, POWER_DOVDD, POWER_DVDD, POWER_AFVCC, POWER_VPP };
    int Volt[10] = { 0 };
    int Current[10] = { 300, 300, 300, 300, 100 };//300mA
    BOOL OnOff[10] = { TRUE,TRUE,TRUE,TRUE,TRUE };
    CURRENT_RANGE range[5] = { CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA, CURRENT_RANGE_MA };
    //set power to 0V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        qCritical("[SetVoltageMclk] Set Voltage Failed! \r\n");
        return DT_ERROR_FAILED;
    }
    //wait for the power is all to zero....
    Sleep(50);
    if (PmuSetOnOff(Power, OnOff, 5, iDevID) != DT_ERROR_OK)
    {
        CloseDevice(iDevID);
        qCritical("[SetVoltageMclk] Open PowerOnOff Failed! \r\n");
        return DT_ERROR_FAILED;
    }
    Sleep(50);
    // 1. set power the avdd
    Volt[POWER_DOVDD] = (int)(Dovdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);

    // 2, set power the dvdd
    Volt[POWER_DVDD] = (int)(Dvdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);

    // 3, set power the dovdd
    Volt[POWER_AVDD] = (int)(Avdd * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    Sleep(2);
    //4. set power the afvcc and vpp
    Volt[POWER_AFVCC] = (int)(Afvcc * 1000); // 2.8V
    Volt[POWER_VPP] = (int)(vpp * 1000); // 2.8V
    if (PmuSetVoltage(Power, Volt, 5, iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Open PowerOn Failed!");
        return DT_ERROR_FAILED;
    }
    //should wait for 50ms to be ready...
    Sleep(50);
    //first set pin definition...
    {
        BYTE  pinDef[40] = { 0 };
        if (CurrentSensor.port == PORT_MIPI || CurrentSensor.port == PORT_HISPI)
        {
            pinDef[0] = 20;
            pinDef[1] = 0;
            pinDef[2] = 2;
            pinDef[3] = 1;
            pinDef[4] = 3;
            pinDef[5] = 4;
            pinDef[6] = 5;
            pinDef[7] = 6;
            pinDef[8] = 7;
            pinDef[9] = 8;
            pinDef[10] = 9;
            pinDef[11] = 20;
            pinDef[12] = 10;
            pinDef[13] = 11;
            pinDef[14] = 12;
            pinDef[15] = 20;
            pinDef[16] = 20;
            pinDef[17] = 13;
            pinDef[18] = 15;
            pinDef[19] = 14;
            pinDef[20] = 19;
            pinDef[21] = 18;
            pinDef[22] = 20;
            pinDef[23] = 16;
            pinDef[24] = 20;
            pinDef[25] = 20;
        }
        else  //standard parallel..
        {

            pinDef[0] = 16;
            pinDef[1] = 0;
            pinDef[2] = 2;
            pinDef[3] = 1;
            pinDef[4] = 3;
            pinDef[5] = 4;
            pinDef[6] = 5;
            pinDef[7] = 6;
            pinDef[8] = 7;
            pinDef[9] = 8;
            pinDef[10] = 9;
            pinDef[11] = 20;
            pinDef[12] = 10;
            pinDef[13] = 11;
            pinDef[14] = 12;
            pinDef[15] = 20;
            pinDef[16] = 20;
            pinDef[17] = 20;
            pinDef[18] = 20;
            pinDef[19] = 20;
            pinDef[20] = 13;
            pinDef[21] = 20;
            pinDef[22] = 14;
            pinDef[23] = 15;
            pinDef[24] = 18;
            pinDef[25] = 19;
        }
        SetSoftPin(pinDef, iDevID);
    }
    EnableSoftPin(TRUE, iDevID);
    EnableGpio(TRUE, iDevID);

    int SampleSpeed[5] = { 100,100,100,100,100 };
    PmuSetSampleSpeed(Power, SampleSpeed, 5, iDevID);
    Sleep(10);
    {
        PmuSetCurrentRange(Power, range, 5, iDevID);
        PmuSetOcpCurrentLimit(Power, Current, 5, iDevID);
    }
    if (SetSensorClock(TRUE, (USHORT)(Mclk * 10), iDevID) != DT_ERROR_OK)
    {
        qCritical("[SetVoltageMclk] Set Mclk Failed!");
        return DT_ERROR_FAILED;
    }
    SetSoftPinPullUp(IO_PULLUP, iDevID);
    return TRUE;
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

iniParser::iniParser(){}

iniParser::~iniParser(){}

void iniParser::SetIniFilename(std::string sFilename)
{
    this->sFilename = sFilename;
}

int iniParser::ReadIniData(std::string sSection, std::string sSectionKey, int nDefault)
{
    std::wstring stemp = s2ws(sSection);
    LPCWSTR lpcwstrSection = stemp.c_str();
    std::wstring stemp1 = s2ws(sSectionKey);
    LPCWSTR lpcwstrSectionKey = stemp1.c_str();
    std::wstring stemp2 = s2ws(sFilename);
    LPCWSTR lpcwstrFilename = stemp2.c_str();
    return GetPrivateProfileInt(lpcwstrSection, lpcwstrSectionKey, nDefault, lpcwstrFilename);
}

BOOL iniParser::GetI2CData(pSensorTab pSensor, QStringList &cmd_list, QStringList &otp_list)
{
    std::string sReg, sVal;
    std::string strTmp[10];
    int tmp = 0;
    int state = -1;
    int reg = 0;
    int val = 0;
    int val_char_len = 4;
    if((pSensor->mode%2)==0)
        val_char_len = 6;
    USHORT *paraList;
    USHORT paraListSize = 0;
    USHORT sleepParaListSize = 0;
    USHORT af_InitParaListSize = 0;
    USHORT afAutoParaListSize = 0;
    USHORT af_FarParaListSize = 0;
    USHORT af_NearParaListSize = 0;
    USHORT exposure_ParaListSize = 0;
    USHORT gain_ParaListSize = 0;
    paraList = pSensor->ParaList;
    paraListSize = pSensor->ParaListSize;
    strTmp[0] = "[ParaList]";
    strTmp[1] = "[SleepParaList]";
    strTmp[2] = "[AF_InitParaList]";
    strTmp[3] = "[AFAutoParaList]";
    strTmp[4] = "[AF_FarParaList]";
    strTmp[5] = "[AF_NearParaList]";
    strTmp[6] = "[Exposure_ParaList]";
    strTmp[7] = "[Gain_ParaList]";
    strTmp[8] = "[SensorID_ParaList]";
    strTmp[9] = "[OTP_ParaList]";

    for (int i = 0; i < 10; i++)
    {
        std::transform(strTmp[i].begin(), strTmp[i].end(), strTmp[i].begin(), ::tolower);
    }
    std::string line;
    std::ifstream fs(sFilename);
    if (fs.is_open())
    {
        while (getline(fs, line))
        {
            tmp = (int)line.find("//");
            if (tmp == 0)
            {
                continue;
            }
            else if (tmp > 0)
            {
                line = line.substr(0, tmp);
            }
            tmp = (int)line.find("]");
            if (tmp == 0)
            {
                continue;
            }
            else if (tmp > 0)
            {
                line = line.substr(0, tmp + 1);
            }
            //printf("%s\r\n", line.c_str());
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            std::remove(line.begin(), line.end(), ' ');
            if (line.length() == 0) continue;
            if (line == strTmp[0])
            {
                state = 0;
                paraListSize = 0;
                continue;
            }
            else if (line == strTmp[1])
            {
                state = 1;
                sleepParaListSize = 0;
                continue;
            }
            else if (line == strTmp[2])
            {
                state = 2;
                af_InitParaListSize = 0;
                continue;
            }
            else if (line == strTmp[3])
            {
                state = 3;
                afAutoParaListSize = 0;
                continue;
            }
            else if (line == strTmp[4])
            {
                state = 4;
                af_FarParaListSize = 0;
                continue;
            }
            else if (line == strTmp[5])
            {
                state = 5;
                af_NearParaListSize = 0;
                continue;
            }
            else if (line == strTmp[6])
            {
                state = 6;
                exposure_ParaListSize = 0;
                continue;
            }
            else if (line == strTmp[7])
            {
                state = 7;
                gain_ParaListSize = 0;
                continue;
            }

            else if (line == strTmp[8])
            {
                state = 8;
                gain_ParaListSize = 0;
                continue;
            }
            else if (line == strTmp[9])
            {
                state = 9;
                gain_ParaListSize = 0;
                continue;
            }

            if (state == 0)
            {
                int tmp_1 = (int)line.find(",", 0);
                sReg = line.substr(0, tmp_1);
                sVal = line.substr(tmp_1 + 1, val_char_len);
                if (!sscanf_s(sReg.c_str(), "0x%x", &reg))
                    sscanf_s(sReg.c_str(), "%d", &reg);
                if (!sscanf_s(sVal.c_str(), "0x%x", &val))
                    sscanf_s(sVal.c_str(), "%d", &val);
                *(paraList + paraListSize) = reg;
                *(paraList + paraListSize + 1) = val;
                paraListSize += 2;
                //printf("Paralist: %s Reg: %s Val: %s\r\n", line.c_str(), sReg.c_str(), sVal.c_str());
            }

            if (state == 8)
            {
                cmd_list.append(line.c_str());
            }

            if (state == 9)
            {
                otp_list.append(line.c_str());
            }
        }
        fs.close();
    }
    pSensor->ParaListSize = paraListSize;
    return true;
}
