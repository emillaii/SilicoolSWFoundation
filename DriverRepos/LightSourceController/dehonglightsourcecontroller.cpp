#include "dehonglightsourcecontroller.h"

DehongLightSourceController::DehongLightSourceController(SerialPortConfig *config, QObject *parent) : QObject(parent)
{
    serialPort = new SCSerialPort("DehongLSC");
    serialPort->setConfig(config);
    serialPort->moveToThread(&SingletonThd::serialPortThd());
}

void DehongLightSourceController::init()
{
    serialPort->initSerialPort();
}

// Example：
// SA0200#： Channel 1 set 200; SA0200#SB0255#: Channel 1 set 200, channel 2 set 255
void DehongLightSourceController::setBrightness(int channel, int brightness)
{
    CommandStruct cmd;
    cmd.channel = static_cast<uint8_t>(channel == 0 ? 'A' : 'B');
    cmd.data = static_cast<uint8_t>(brightness);
    QByteArray data(reinterpret_cast<char *>(&cmd), sizeof(cmd));
    serialPort->writeData(data);
}
