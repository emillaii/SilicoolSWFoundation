#ifndef VISIONELEMENTDEFINITION_H
#define VISIONELEMENTDEFINITION_H

#include "configManager/configarray.h"
#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"
#include "renameManager/renamemanager.h"
#include "scvision_global.h"

class SCVISIONSHARED_EXPORT VisionLocationDefinition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QString calibrationName READ calibrationName WRITE setCalibrationName NOTIFY calibrationNameChanged)

public:
    Q_INVOKABLE VisionLocationDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
        RenameManager::getIns().subscribePrNameChanged(this, "locationName");
    }
    QString locationName() const
    {
        return m_locationName;
    }
    QString calibrationName() const
    {
        return m_calibrationName;
    }

public slots:
    void setLocationName(QString locationName)
    {
        if (m_locationName == locationName)
            return;

        m_locationName = locationName;
        emit locationNameChanged(m_locationName);
    }
    void setCalibrationName(QString calibrationName)
    {
        if (m_calibrationName == calibrationName)
            return;

        m_calibrationName = calibrationName;
        emit calibrationNameChanged(m_calibrationName);
    }

signals:
    void locationNameChanged(QString locationName);
    void calibrationNameChanged(QString calibrationName);

private:
    QString m_locationName;
    QString m_calibrationName;
};

class SCVISIONSHARED_EXPORT VisionElementDefinition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(ConfigObjectArray *visionLocationDefinitions READ visionLocationDefinitions)
    Q_PROPERTY(ConfigArray *cameraNames READ cameraNames)
    Q_PROPERTY(ConfigArray *lscNames READ lscNames)

public:
    VisionElementDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_visionLocationDefinitions = new ConfigObjectArray(&VisionLocationDefinition::staticMetaObject, this);
        m_cameraNames = new ConfigArray(ConfigElementInfo::Other, this);
        m_lscNames = new ConfigArray(ConfigElementInfo::Other, this);
        disableTranslate();
        init();
    }

    ConfigObjectArray *visionLocationDefinitions() const
    {
        return m_visionLocationDefinitions;
    }

    ConfigArray *cameraNames() const
    {
        return m_cameraNames;
    }

    ConfigArray *lscNames() const
    {
        return m_lscNames;
    }

private:
    ConfigObjectArray *m_visionLocationDefinitions;
    ConfigArray *m_cameraNames;
    ConfigArray *m_lscNames;
};

#endif    // VISIONELEMENTDEFINITION_H
