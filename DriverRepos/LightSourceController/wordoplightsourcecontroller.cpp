#include "wordoplightsourcecontroller.h"

WordopLightSourceController::WordopLightSourceController(SerialPortConfig *config, QObject *parent) : QObject(parent)
{
    m_config = qobject_cast<WordopLSCConfig *>(config);

    serialPort = new SCSerialPort("WordopLSC");
    serialPort->setConfig(config);
    serialPort->moveToThread(&SingletonThd::serialPortThd());
}

WordopLightSourceController::~WordopLightSourceController()
{
    dispose();
}

void WordopLightSourceController::init()
{
    serialPort->initSerialPort();
}

void WordopLightSourceController::setBrightness(int channel, int brightness)
{
    CommandStruct cmd;
    cmd.head = 0x40;
    cmd.len = 5;
    cmd.devCode = DevCode;
    cmd.devId = static_cast<uint8_t>(devId);
    cmd.cmd = SetBrightnessCmd;
    cmd.channel = static_cast<uint8_t>(channel);
    cmd.data = static_cast<uint8_t>(brightness);
    cmd.calcCheckSum();
    QByteArray data(reinterpret_cast<char *>(&cmd), sizeof(cmd));
    serialPort->writeData(data);
}

void WordopLightSourceController::open(int channel)
{
    setOnOffImpl(channel, true);
}

void WordopLightSourceController::close(int channel)
{
    setOnOffImpl(channel, false);
}

bool WordopLightSourceController::getBrightness(int channel, int &brightness)
{
    //    uint8_t cmd[7];
    //    cmd[0] = 0x40;
    //    cmd[1] = 0x04;
    //    cmd[2] = DevCode;
    //    cmd[3] = static_cast<uint8_t>(devId);
    //    cmd[4] = 0x31;
    //    cmd[5] = static_cast<uint8_t>(channel);
    //    int checkSum = 0;
    //    for (int i = 0; i < 6; i++)
    //    {
    //        checkSum += cmd[i];
    //    }
    //    cmd[6] = static_cast<uint8_t>(checkSum);
    //    QByteArray data(reinterpret_cast<char *>(cmd), 7);
    //    serialPort->clearReadBuffer();
    //    try
    //    {
    //        serialPort->writeData(data);
    //        QByteArray rsp = serialPort->readData(9, 100);
    //        brightness = static_cast<uint8_t>(rsp[5]);
    //        return true;
    //    }
    //    catch (SilicoolException &se)
    //    {
    //        qCritical() << se.what();
    //        brightness = -1;
    //        return false;
    //    }

    Q_UNUSED(channel);
    Q_UNUSED(brightness);
    return false;
}

void WordopLightSourceController::dispose()
{
    if (serialPort->isInit())
    {
        if (m_config != nullptr && m_config->closeLightBeforeCloseSW())
        {
            for (int i = 0; i < 4; i++)
            {
                setBrightness(i, 0);
            }
        }
    }
    serialPort->dispose();
}

void WordopLightSourceController::setOnOffImpl(int channel, bool isOn)
{
    CommandStruct cmd;
    cmd.head = 0x40;
    cmd.len = 5;
    cmd.devCode = DevCode;
    cmd.devId = static_cast<uint8_t>(devId);
    cmd.cmd = SetOnOffCmd;
    cmd.channel = static_cast<uint8_t>(channel);
    cmd.data = isOn ? 1 : 0;
    cmd.calcCheckSum();
    QByteArray data(reinterpret_cast<char *>(&cmd), sizeof(cmd));
    serialPort->writeData(data);
}
