#include "CzSensorIniFile.h"
#include <algorithm>
#include <QObject>

CzSensorIniFile::CzSensorIniFile()
{
}


CzSensorIniFile::~CzSensorIniFile()
{
}

bool CzSensorIniFile::LoadSensorSettingFromFile(T_SensorSetting *sensorSetting, const char *fileName)
{
    qInfo("LoadSensorSettingFromFile..%s", fileName);
   // if (sensorSetting->Id > 0)
   //     FreeSensorSetting(sensorSetting);
    LoadFile(std::string(fileName));
    std::vector<T_LineConf> vecSection;
    ReadSection("default", vecSection);
    std::string version = "1.0";
    for (auto itLine = vecSection.begin(); itLine != vecSection.end(); ++itLine)
    {
        std::string::size_type pos = itLine->Comment.find("CZTEK", 0);
        if (pos != std::string::npos)
            version = itLine->Comment.substr(pos + std::string("CZTEK ").size(), 3);
    }

    ReadSection("Sensor", vecSection);
    for (auto itLine = vecSection.begin(); itLine != vecSection.end(); ++itLine)
    {
        if (itLine->Key == "SchemeName") {
            sensorSetting->SchemeName = new char[128];
            memcpy(sensorSetting->SchemeName, itLine->Value.c_str(), itLine->Value.size());
            sensorSetting->SchemeName[itLine->Value.size()] = '\0';
        }
        /*if (itLine->Key == "Description") {
        sensor->Description = new char[128];
        memcpy(sensorSetting->Description, itLine->Value.c_str(), itLine->Value.size());
        }
        if (itLine->Key == "VendorName") {
        sensorSetting->VendorName = itLine->Value;
        }*/
        if (itLine->Key == "ChipName") {
            sensorSetting->ChipName = new char[128];
            memcpy(sensorSetting->ChipName, itLine->Value.c_str(), itLine->Value.size());
            sensorSetting->ChipName[itLine->Value.size()] = '\0';
        }
        if (itLine->Key == "InterfaceType") {
            sensorSetting->InterfaceType = (E_InterfaceType)stoi(itLine->Value);
        }
        if ((itLine->Key == "MipiLanes") || (itLine->Key == "Lanes")) {
            sensorSetting->Lanes = stoi(itLine->Value);
        }
        if (itLine->Key == "MipiFreq") {
            sensorSetting->MipiFreq = stoi(itLine->Value);
        }
        if (itLine->Key == "Mclk") {
            sensorSetting->Mclk_kHz = stoi(itLine->Value) * 1000;
        }
        if (itLine->Key == "DataWidth") {
            sensorSetting->DataWidth = stoi(itLine->Value);
        }
        if (itLine->Key == "ImageFormat") {
            sensorSetting->ImageFormat = (E_ImageFormat)stoi(itLine->Value);
        }
        if (itLine->Key == "ImageMode") {
            sensorSetting->ImageMode = (E_ImageMode)stoi(itLine->Value);
        }
        if (itLine->Key == "PixelWidth") {
            sensorSetting->PixelWidth = stoi(itLine->Value);
        }
        if (itLine->Key == "PixelHeight") {
            sensorSetting->PixelHeight = stoi(itLine->Value);
        }
        if (itLine->Key == "QuickWidth") {
            //sensorSetting->QuickWidth = stoi(itLine->Value);
        }
        if (itLine->Key == "QuickHeight") {
           // sensorSetting->QuickHeight = stoi(itLine->Value);
        }
        if (itLine->Key == "CropParam") {
            sensorSetting->CropParam.X = sensorSetting->CropParam.Y = 0;
            sensorSetting->CropParam.Width = sensorSetting->CropParam.Height = 0;
            std::vector<std::string> paramList = split(itLine->Value, ",");
            if (paramList.size() > 3) {
                try
                {
                    sensorSetting->CropParam.X = stoi(paramList[0]);
                    sensorSetting->CropParam.Y = stoi(paramList[1]);
                    sensorSetting->CropParam.Width = stoi(paramList[2]);
                    sensorSetting->CropParam.Height = stoi(paramList[3]);
                }
                catch (...)
                {
                }
            }
        }
        if (itLine->Key == "LvdsParam") {
            //sensorSetting->LvdsParam = itLine->Value;
        }
        if ((itLine->Key == "I2CMode") || (itLine->Key == "RegBitsMode")) {
            sensorSetting->I2cParam.RegBitsMode = (E_RegBitsMode)stoi(itLine->Value);
        }
        //if (itLine->Key == "CommIntfType") {
        //    sensorSetting->I2cParam->CommIntfType = (T_CommIntfConf::E_IntfType)stoi(itLine->Value);
        //    if (!ok) {
        //        qCritical() << QString("%1[%2] itLine->Value is invalid!").arg(itLine->Key).arg(itLine->Value);
        //        return false;
        //    }
        //}
        if ((itLine->Key == "I2CSpeed") || (itLine->Key == "CommSpeed")) {
            sensorSetting->I2cParam.Speed = stoi(itLine->Value);
            if (version == "1.0") {
                if (sensorSetting->I2cParam.Speed == 0)
                    sensorSetting->I2cParam.Speed = 1; // 100kHz
                else
                    sensorSetting->I2cParam.Speed = 4; // 400kHz
            }
        }
        if ((itLine->Key == "I2CAddress") || (itLine->Key == "CommAddr")) {
            sensorSetting->I2cParam.Addr = stoi(itLine->Value, 0, 16);
        }
        if (itLine->Key == "CommExtraParam") {
            //sensorSetting->CommExtraParam = itLine->Value;
        }
        if (itLine->Key == "PclkPol") {
            sensorSetting->PclkPol = stoi(itLine->Value);
        }
        if (itLine->Key == "DataPol") {
            sensorSetting->DataPol = stoi(itLine->Value);
        }
        if (itLine->Key == "HsyncPol") {
            sensorSetting->HsyncPol = stoi(itLine->Value);
        }
        if (itLine->Key == "VsyncPol") {
            sensorSetting->VsyncPol = stoi(itLine->Value);
        }
        if (itLine->Key == "Pwdn") {
            sensorSetting->Pwdn = stoi(itLine->Value);
        }
        if (itLine->Key == "PwdnParam") {
            //sensorSetting->PwdnParam = itLine->Value;
        }
        if (itLine->Key == "Reset") {
            sensorSetting->Reset = stoi(itLine->Value);
        }
        if (itLine->Key == "ResetParam") {
            //sensorSetting->ResetParam = itLine->Value;
        }
        if (itLine->Key == "FocusParam") {
            //sensorSetting->FocusParam = itLine->Value;
        }
        if (itLine->Key == "AppType") {
        }
    }
    std::vector<T_RegConf> vecRegConf;
    sensorSetting->FullModeParamCount = 0;
    std::string strTemp = ReadSection("FullModeParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);

        if (vecRegConf.size() > 0) {
            sensorSetting->FullModeParamCount = (int)vecRegConf.size();
            sensorSetting->FullModeParams = new T_RegConf[sensorSetting->FullModeParamCount];
            for (int i = 0; i < sensorSetting->FullModeParamCount; i++)
            {
                sensorSetting->FullModeParams[i] = vecRegConf[i];
            }
        }
    }

    sensorSetting->OtpInitParamCount = 0;
    strTemp = ReadSection("OtpInitParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->OtpInitParamCount = (int)vecRegConf.size();
            sensorSetting->OtpInitParams = new T_RegConf[sensorSetting->OtpInitParamCount];
            for (int i = 0; i < sensorSetting->OtpInitParamCount; i++)
                sensorSetting->OtpInitParams[i] = vecRegConf[i];
        }
    }

    strTemp = ReadSection("QuickModeParams");
    if (!strTemp.empty()) {
        /*parseRegisterList(strTemp, vecRegConf);
        sensorSetting->QuickModeParamCount = vecRegConf.size();
        sensorSetting->QuickModeParams = new T_RegConf[sensorSetting->QuickModeParamCount];
        for (int i = 0; i < sensorSetting->QuickModeParamCount; i++)
        sensorSetting->QuickModeParams[i] = vecRegConf[i];*/
    }

    sensorSetting->SleepParamCount = 0;
    strTemp = ReadSection("SleepParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->SleepParamCount = (int)vecRegConf.size();
            sensorSetting->SleepParams = new T_RegConf[sensorSetting->SleepParamCount];
            for (int i = 0; i < sensorSetting->SleepParamCount; i++)
                sensorSetting->SleepParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->AfInitParamCount = 0;
    strTemp = ReadSection("AfInitParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->AfInitParamCount = (int)vecRegConf.size();
            sensorSetting->AfInitParams = new T_RegConf[sensorSetting->AfInitParamCount];
            for (int i = 0; i < sensorSetting->AfInitParamCount; i++)
                sensorSetting->AfInitParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->AfAutoParamCount = 0;
    strTemp = ReadSection("AfAutoParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->AfAutoParamCount = (int)vecRegConf.size();
            sensorSetting->AfAutoParams = new T_RegConf[sensorSetting->AfAutoParamCount];
            for (int i = 0; i < sensorSetting->AfAutoParamCount; i++)
                sensorSetting->AfAutoParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->AfFarParamCount = 0;
    strTemp = ReadSection("AfFarParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->AfFarParamCount = (int)vecRegConf.size();
            sensorSetting->AfFarParams = new T_RegConf[sensorSetting->AfFarParamCount];
            for (int i = 0; i < sensorSetting->AfFarParamCount; i++)
                sensorSetting->AfFarParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->AfNearParamCount = 0;
    strTemp = ReadSection("AfNearParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->AfNearParamCount = (int)vecRegConf.size();
            sensorSetting->AfNearParams = new T_RegConf[sensorSetting->AfNearParamCount];
            for (int i = 0; i < sensorSetting->AfNearParamCount; i++)
                sensorSetting->AfNearParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->ExposureParamCount = 0;
    strTemp = ReadSection("ExposureParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->ExposureParamCount = (int)vecRegConf.size();
            sensorSetting->ExposureParams = new T_RegConf[sensorSetting->ExposureParamCount];
            for (int i = 0; i < sensorSetting->ExposureParamCount; i++)
                sensorSetting->ExposureParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->GainParamCount = 0;
    strTemp = ReadSection("GainParams");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf);
        if (vecRegConf.size() > 0) {
            sensorSetting->GainParamCount = (int)vecRegConf.size();
            sensorSetting->GainParams = new T_RegConf[sensorSetting->GainParamCount];
            for (int i = 0; i < sensorSetting->GainParamCount; i++)
                sensorSetting->GainParams[i] = vecRegConf[i];
        }
    }

    sensorSetting->PowerCount = 0;
    std::vector<T_Power> vecPowerConf;
    strTemp = ReadSection("Voltages");
    if (!strTemp.empty()) {
        parseVoltageList(strTemp, vecPowerConf);
        if (vecPowerConf.size() > 0) {
            sensorSetting->PowerCount = (int)vecPowerConf.size();
            sensorSetting->Powers = new T_Power[sensorSetting->PowerCount];
            for (int i = 0; i < sensorSetting->PowerCount; i++)
                sensorSetting->Powers[i] = vecPowerConf[i];
        }
    }

    sensorSetting->FlagRegisterCount = 0;
    strTemp = ReadSection("FlagRegisters");
    if (!strTemp.empty()) {
        parseRegisterList(strTemp, vecRegConf, true);
        if (vecRegConf.size() > 0) {
            sensorSetting->FlagRegisterCount = (int)vecRegConf.size();
            sensorSetting->FlagRegisters = new T_RegConf[sensorSetting->FlagRegisterCount];
            for (int i = 0; i < sensorSetting->FlagRegisterCount; i++)
                sensorSetting->FlagRegisters[i] = vecRegConf[i];
        }
    }
    sensorSetting->Id = 1; // greater than 0
    return true;
}

void CzSensorIniFile::FreeSensorSetting(T_SensorSetting *sensorSetting)
{
    if (sensorSetting->Id > 0) {
        delete[] sensorSetting->SchemeName;
        delete[] sensorSetting->ChipName;
        if (sensorSetting->FullModeParamCount > 0)
            delete[] sensorSetting->FullModeParams;
        if (sensorSetting->OtpInitParamCount > 0)
            delete[] sensorSetting->OtpInitParams;
        if (sensorSetting->SleepParamCount > 0)
            delete[] sensorSetting->SleepParams;
        if (sensorSetting->AfInitParamCount > 0)
            delete[] sensorSetting->AfInitParams;
        if (sensorSetting->AfAutoParamCount > 0)
            delete[] sensorSetting->AfAutoParams;
        if (sensorSetting->AfFarParamCount > 0)
            delete[] sensorSetting->AfFarParams;
        if (sensorSetting->AfNearParamCount > 0)
            delete[] sensorSetting->AfNearParams;
        if (sensorSetting->ExposureParamCount > 0)
            delete[] sensorSetting->ExposureParams;
        if (sensorSetting->GainParamCount > 0)
            delete[] sensorSetting->GainParams;
        if (sensorSetting->FlagRegisterCount > 0)
            delete[] sensorSetting->FlagRegisters;
        if (sensorSetting->PowerCount > 0)
            delete[] sensorSetting->Powers;
    }
}

bool CzSensorIniFile::parseRegisterList(const std::string &paramString, std::vector<T_RegConf> &regList, bool isFlagRegister)
{
    regList.clear();
    std::vector<std::string> lines = split(paramString, "\n");
    std::string strTemp;
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        strTemp = *it;
        int idxSemicolon = (int)strTemp.find_last_of(';');
        if (idxSemicolon != std::string::npos)
            strTemp = trim(strTemp.erase(idxSemicolon));
        std::vector<std::string> paramList = split(strTemp, ",");
        if (paramList.size() < 2)
            continue;
        T_RegConf regConf;
        regConf.Addr = stoul(paramList[0], 0, 16);
        regConf.Value = stoul(paramList[1], 0, 16);
        regConf.Delay_ms = 0;
        if (isFlagRegister)
        {
            regConf.Mask = 0xff;
            if (paramList.size() > 2)
                regConf.Mask = stoul(paramList[2], 0, 16);
        }
        else
        {
            if (paramList.size() > 2)
                regConf.Delay_ms = stoi(paramList[2]);
            regConf.Mask = 0xff;
            if (paramList.size() > 3)
                regConf.Mask = stoi(paramList[3], 0, 16);
        }
        regList.push_back(regConf);
    }
    return true;
}

bool CzSensorIniFile::parseVoltageList(const std::string &strVoltage, std::vector<T_Power> &powerList)
{
    powerList.clear();
    std::vector<std::string> lines = split(strVoltage, "\n");
    std::string strTemp;
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        strTemp = trim(it->erase(it->find_last_not_of(';')));
        std::vector<std::string> paramList = split(strTemp, ",");
        if (paramList.size() < 3)
            continue;
        T_Power power;
        std::string strVoltName = paramList[0];
        std::transform(strVoltName.begin(), strVoltName.end(), strVoltName.begin(), toupper);
        if (strVoltName == "DVDD")
            power.Id = (int)Power_DVDD;
        else if (strVoltName == "AVDD")
            power.Id = (int)Power_AVDD;
        else if (strVoltName == "DOVDD")
            power.Id = (int)Power_DOVDD;
        else if (strVoltName == "AFVCC")
            power.Id = (int)Power_AFVCC;
        else if (strVoltName == "VPP")
            power.Id = (int)Power_VPP;
        else if (strVoltName == "AVDD2")
            power.Id = (int)Power_AVDD2;
        else if (strVoltName == "VOIS")
            power.Id = (int)Power_VOIS;
        else
        {
            power.Id = 0;
            return false;
        }
        power.Value = stoi(paramList[1]); // mV
        power.Delay_ms = stoi(paramList[2]);
        powerList.push_back(power);
    }
    return true;
}
