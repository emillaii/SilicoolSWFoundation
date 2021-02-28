#include "xfgraber.h"


XFGraber::XFGraber(QObject *parent) : SCGraber(parent)
{
    for (int i = 0; i < MAX_DEV; i++)
    {
        m_devName[i] = new char[1024];
    }
}

XFGraber::~XFGraber()
{
    for (int i = 0; i < MAX_DEV; i++) {
        delete m_devName[i];
    }
}

void XFGraber::enumerateDevice()
{
    if(m_IsOpened)
    {
        //throw SilicolAbort(tr("Graber is in opened! in enumerateDevice function"));
    }
    if(EnumerateDevice(m_devName) < 0)
    {
        throw  SilicolAbort(tr("enumerateDevice command failed!"));
    }
}

void XFGraber::releaseDevice()
{
    XFApiFree();
}

void XFGraber::openDevice()
{
    if(m_IsOpened)
    {
        //throw SilicolAbort(tr("Graber is in opened! in openDevice function"));
    }
    if(m_devName[0] == nullptr)
    {
        throw SilicolAbort(tr("device name is null! please enumerate device first!"));
    }
    if(OpenDevice(m_devName[m_devID]))
    {
        m_IsOpened = true;
        qInfo("Open device success");
    }
    else
    {
        m_IsOpened = false;
        throw SilicolAbort(tr("Open device failed!"));
    }
}

void XFGraber::closeDevice()
{
    //CloseDevice(m_devID);
    XF::StopVideo(m_devID);
}

void XFGraber::startCamera()
{
    qInfo("Start camera");
    if(!m_IsOpened)
    {
        throw SilicolAbort(tr("Graber didn't open! in startCamera function"));
    }

    if(!CameraIsPlay(m_devID))
    {
        if(PlayVideo(m_devID) !=1)
        {
            throw SilicolAbort(tr("start Camera failed!"));
        }
        qInfo("Start camera success");
    }
}

QImage XFGraber::grabImage()
{
    QMutexLocker locker(&mutex);
    if(!CameraIsPlay(m_devID)) {
        qWarning("Camerais not playing");
        QImage image; return image;
    }
    unsigned char *m_pRawBuffer;
    if(!GrabFrameToRGB24(&m_pRawBuffer, m_devID))
    {
        qWarning("XF Grabber grabFrameToRGB24 fail");
        QImage image; return image;
    }
    QImage  image(m_pRawBuffer, GetCurrentSensor(m_devID)->width, GetCurrentSensor(m_devID)->height, QImage::Format_RGB888);
    return image;
}

cv::Mat XFGraber::grabImageCV()
{
    QMutexLocker locker(&mutex);
    if(!CameraIsPlay(m_devID)) {
        qInfo("Grabber is not grabbing");
        cv::Mat img;
        return img;
    }
    qInfo("grab image: %s width: %d height: %d", GetCurrentSensor(m_devID)->sensorName, GetCurrentSensor(m_devID)->width, GetCurrentSensor(m_devID)->height);
    if(!m_IsOpened)
    {
        //throw SilicolAbort(tr("Open Device first!"));
        qInfo("Device did not open!");
        cv::Mat img;
        return img;
    }
    unsigned char *m_pRawBuffer;
    if(!GrabFrameToRGB24(&m_pRawBuffer, m_devID))
    {
        //throw SilicolAbort(tr("GrabFrameToRGB24 function failed!"));
        qInfo("GrabFrameToRGB24 function failed!");
        cv::Mat img;
        return img;
    }
    cv::Mat img(GetCurrentSensor(m_devID)->height, GetCurrentSensor(m_devID)->width, CV_8UC3, m_pRawBuffer);
    return img;
}

void XFGraber::setBandwidth(int width, int height)
{
}

uint getUIntFromHexOrDecString(QString value){
    bool ok = false;
    uint result = 0;
    result = value.toInt(&ok, 16);
    if(ok) return result;
    else result = value.toInt(&ok, 10);
    return result;
}

//Beware the "," in parsing ini file, if "," is present, need to use QStringList to parse
void XFGraber::readIniFile(QString iniFilename)
{
    iniFilename.replace("file:///", "");
    QSettings settings(iniFilename, QSettings::IniFormat);
    settings.beginGroup("Sensor");
    QString sensorName = settings.value("SensorName").toStringList()[0].split("//")[0];
    int width = settings.value("width").toStringList()[0].split("//")[0].toInt(0);
    qDebug("width: %d", width);

    int height = settings.value("height").toStringList()[0].split("//")[0].toInt(0);
    qDebug("height: %d", height);

    int sensorFormat = settings.value("sensorFormat").toStringList()[0].split("//")[0].toInt(0);
    qDebug("sensorFormat: %d", sensorFormat);

    int sensorPort = settings.value("sensorPort").toStringList()[0].split("//")[0].toInt(0);
    qDebug("sensorPort: %d", sensorPort);

    int pwdn = settings.value("pwdn").toStringList()[0].split("//")[0].toInt(0);
    qDebug("pwdn: %d", pwdn);

    QString slaveIDString = settings.value("SlaveID").toStringList()[0].split("//")[0];
    int slaveId = getUIntFromHexOrDecString(slaveIDString);
    qDebug("SlaveID: 0x%04x", slaveId);

    int i2c_mode = settings.value("i2c_mode").toStringList()[0].split("//")[0].toInt(0);
    qDebug("i2cMode: %d", i2c_mode);

    QString sensorIDAddrString = settings.value("SensorIDAddr").toStringList()[0].split("//")[0];
    int sensorIDAddr = getUIntFromHexOrDecString(sensorIDAddrString);
    qDebug("sensorIDAddr: 0x%04x", sensorIDAddr);

    QString sensorIDString = settings.value("SensorID").toStringList()[0].split("//")[0];
    int sensorID = getUIntFromHexOrDecString(sensorIDString);
    qDebug("sensorID: 0x%04x", sensorID);

    int disply_mode = settings.value("disply_mode").toStringList()[0].split("//")[0].toInt(0);
    qDebug("disply_mode: %d", disply_mode);

    int avdd = settings.value("avdd").toStringList()[0].split("//")[0].toInt(0);
    qDebug("avdd: %d", avdd);

    int dovdd = settings.value("DOVDD").toStringList()[0].split("//")[0].toInt(0);
    qDebug("dovdd: %d", dovdd);

    int dvdd = settings.value("DVDD").toStringList()[0].split("//")[0].toInt(0);
    qDebug("dvdd: %d", dvdd);

    int mclk = settings.value("mclk").toStringList()[0].split("//")[0].toInt(0);
    qDebug("mclk: %d", mclk);


    int mipiclk = settings.value("mipiclk").toStringList()[0].split("//")[0].toInt(0);
    qDebug("mipiClk: %d", mipiclk);

}
