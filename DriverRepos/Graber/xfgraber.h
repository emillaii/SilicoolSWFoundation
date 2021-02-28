#ifndef XFGRABER_H
#define XFGRABER_H

#include <QObject>
#include "scgraber.h"
#include "xfccm2.h"
#include "xfdefine.h"
#include "errorHandling/silicolerror.h"
#include "qimage.h"

using namespace XF;

class XFGraber : public SCGraber
{
    Q_OBJECT
public:
    explicit XFGraber(QObject *parent = nullptr);
    ~XFGraber() override;
    void setBandwidth(int width, int height);
    void readIniFile(QString iniFilename);
    //SCGraber interface

    virtual void enumerateDevice() override;
    virtual void releaseDevice() override;
    virtual void openDevice() override;
    virtual void closeDevice() override;
    virtual void startCamera() override;
    virtual QImage grabImage() override;
    virtual cv::Mat grabImageCV() override;

    int getSensorOutputWidth() { return XF::GetCurrentSensor(m_devID)->width; }
    int getSensorOutputHeight() { return XF::GetCurrentSensor(m_devID)->height; }

signals:

public slots:

private:
    QMutex mutex;
    char *m_devName[MAX_DEV];
    int m_devID = 0;
    bool m_IsOpened = false;
};

#endif // XFGRABER_H
