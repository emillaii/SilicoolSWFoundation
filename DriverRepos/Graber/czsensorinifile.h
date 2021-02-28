#ifndef CZSENSORINIFILE_H
#define CZSENSORINIFILE_H

#include "czinifile.h"
#include "CmtiSdk.h"

class CzSensorIniFile : public IniFile
{
public:
    CzSensorIniFile();
    ~CzSensorIniFile();

    bool LoadSensorSettingFromFile(T_SensorSetting *sensorSetting, const char *fileName);
    void FreeSensorSetting(T_SensorSetting *sensorSetting);

private:
    bool parseRegisterList(const std::string &paramString, std::vector<T_RegConf> &regList, bool isFlagRegister = false);
    bool parseVoltageList(const std::string &strVoltage, std::vector<T_Power> &powerList);
};

#endif // CZSENSORINIFILE_H
