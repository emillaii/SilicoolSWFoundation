#ifndef HIKVISIONCONFIG_H
#define HIKVISIONCONFIG_H

#include "configManager/configobject.h"
#include "hikvision_global.h"
#include <QObject>

class HIKVISIONSHARED_EXPORT HikVisionConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString visionServerPath READ visionServerPath WRITE setVisionServerPath NOTIFY visionServerPathChanged)
    Q_PROPERTY(QString visionMasterPath READ visionMasterPath WRITE setVisionMasterPath NOTIFY visionMasterPathChanged)

public:
    explicit HikVisionConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setSelectFileProperty("visionServerPath", true);
        setSelectFileProperty("visionMasterPath", true);
        init();
    }

    QString visionServerPath() const
    {
        return m_visionServerPath;
    }

    QString visionMasterPath() const
    {
        return m_visionMasterPath;
    }

public slots:
    void setVisionServerPath(QString visionServerPath)
    {
        if (m_visionServerPath == visionServerPath)
            return;

        m_visionServerPath = visionServerPath;
        emit visionServerPathChanged(m_visionServerPath);
    }

    void setVisionMasterPath(QString visionMasterPath)
    {
        if (m_visionMasterPath == visionMasterPath)
            return;

        m_visionMasterPath = visionMasterPath;
        emit visionMasterPathChanged(m_visionMasterPath);
    }

signals:
    void visionServerPathChanged(QString visionServerPath);

    void visionMasterPathChanged(QString visionMasterPath);

private:
    QString m_visionServerPath;
    QString m_visionMasterPath;
};

#endif    // HIKVISIONCONFIG_H
