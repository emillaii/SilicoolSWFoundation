#ifndef HIKVISIONLOCATIONCONFIG_H
#define HIKVISIONLOCATIONCONFIG_H

#include "VisionLocation/visionlocationconfig.h"
#include "hikvision_global.h"
#include "visionconfigdir.h"

class HIKVISIONSHARED_EXPORT HikVisionLocationConfig : public VisionLocationConfig
{
    Q_OBJECT

    Q_PROPERTY(int processId READ processId WRITE setProcessId NOTIFY processIdChanged)
    Q_PROPERTY(int imageSourceModuleId READ imageSourceModuleId WRITE setImageSourceModuleId NOTIFY imageSourceModuleIdChanged)
    Q_PROPERTY(int resultModuleId READ resultModuleId WRITE setResultModuleId NOTIFY resultModuleIdChanged)

public:
    Q_INVOKABLE HikVisionLocationConfig(QObject *parent = nullptr) : VisionLocationConfig(parent)
    {
        connect(this, &VisionLocationConfig::locationNameChanged, this, &HikVisionLocationConfig::onLocationNameChanged, Qt::DirectConnection);
        init();
    }

    int processId() const
    {
        return m_processId;
    }

    int imageSourceModuleId() const
    {
        return m_imageSourceModuleId;
    }

    int resultModuleId() const
    {
        return m_resultModuleId;
    }

public slots:
    void setProcessId(int processId)
    {
        if (m_processId == processId)
            return;

        m_processId = processId;
        emit processIdChanged(m_processId);
    }

    void setImageSourceModuleId(int imageSourceModuleId)
    {
        if (m_imageSourceModuleId == imageSourceModuleId)
            return;

        m_imageSourceModuleId = imageSourceModuleId;
        emit imageSourceModuleIdChanged(m_imageSourceModuleId);
    }

    void setResultModuleId(int resultModuleId)
    {
        if (m_resultModuleId == resultModuleId)
            return;

        m_resultModuleId = resultModuleId;
        emit resultModuleIdChanged(m_resultModuleId);
    }

    void resetPrResultImage() override
    {
        QString prResultImageName = VisionConfigDir::getIns().prConfigDir() + locationName() + "_resultImage.jpg";
        if (QFile::exists(prResultImageName))
        {
            setPrResultImage(QString("file:///") + prResultImageName);
        }
        else
        {
            setPrResultImage("");
        }
    }

    void onLocationNameChanged(QString locationName)
    {
        if (!locationName.isEmpty())
        {
            resetPrResultImage();
        }
    }

signals:
    void processIdChanged(int processId);

    void imageSourceModuleIdChanged(int imageSourceModuleId);

    void resultModuleIdChanged(int resultModuleId);

private:
    int m_processId = 10000;
    int m_imageSourceModuleId = 0;
    int m_resultModuleId = -1;
};

#endif    // HIKVISIONLOCATIONCONFIG_H
