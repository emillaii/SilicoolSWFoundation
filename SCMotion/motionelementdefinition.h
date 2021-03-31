#ifndef MOTIONELEMENTDEFINITION_H
#define MOTIONELEMENTDEFINITION_H

#include "configManager/configarray.h"
#include "configManager/configobject.h"
#include "motionutility.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT AxisModuleDefinition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(QString moduleType READ moduleType WRITE setModuleType NOTIFY moduleTypeChanged)
    Q_PROPERTY(ConfigArray *modulePosNames READ modulePosNames)
    Q_PROPERTY(ConfigArray* dutRelatedModulePosNames READ dutRelatedModulePosNames)

public:
    Q_INVOKABLE AxisModuleDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_modulePosNames = new ConfigArray(ConfigElementInfo::Other, this);
        m_dutRelatedModulePosNames = new ConfigArray(ConfigElementInfo::Other, this);

        auto typesInfo = MotionElement::TypeEnumInfo();
        auto typesNames = typesInfo.names();
        typesNames.removeOne(typesInfo.enumToName(MotionElement::Axis));
        typesNames.removeOne(typesInfo.enumToName(MotionElement::DI));
        typesNames.removeOne(typesInfo.enumToName(MotionElement::DO));
        typesNames.removeOne(typesInfo.enumToName(MotionElement::Cylinder));
        typesNames.removeOne(typesInfo.enumToName(MotionElement::Vacuum));
        setOptionalProperty("moduleType", toVariantList<QString>(typesNames));
        disableTranslate();
        init();
    }

    QString moduleName() const
    {
        return m_moduleName;
    }

    QString moduleType() const
    {
        return m_moduleType;
    }

    ConfigArray *modulePosNames() const
    {
        return m_modulePosNames;
    }

    ConfigArray* dutRelatedModulePosNames() const
    {
        return m_dutRelatedModulePosNames;
    }

public slots:
    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

    void setModuleType(QString moduleType)
    {
        if (m_moduleType == moduleType)
            return;

        m_moduleType = moduleType;
        emit moduleTypeChanged(m_moduleType);
    }

signals:
    void moduleNameChanged(QString moduleName);

    void moduleTypeChanged(QString moduleType);

private:
    QString m_moduleName;
    QString m_moduleType;
    ConfigArray *m_modulePosNames;
    ConfigArray* m_dutRelatedModulePosNames;
};

class SCMOTIONSHARED_EXPORT AxisDefinition : public ConfigObject
{
    Q_OBJECT

public:
    enum AxisType
    {
        GeneralAxis,
        ExtendedAxis,
        ExtendedAxis2,
        ExtendedAxis3,
        ExtendedAxis4
    };
    Q_ENUM(AxisType)

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)
    Q_PROPERTY(AxisType axisType READ axisType WRITE setAxisType NOTIFY axisTypeChanged)
    Q_PROPERTY(bool isDummyAxis READ isDummyAxis WRITE setIsDummyAxis NOTIFY isDummyAxisChanged)
    Q_PROPERTY(QString masterAxisName READ masterAxisName WRITE setMasterAxisName NOTIFY masterAxisNameChanged)

public:
    Q_INVOKABLE AxisDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    QString axisName() const
    {
        return m_axisName;
    }

    AxisType axisType() const
    {
        return m_axisType;
    }

    QString masterAxisName() const
    {
        return m_masterAxisName;
    }

    bool isDummyAxis() const
    {
        return m_isDummyAxis;
    }

public slots:
    void setAxisName(QString axisName)
    {
        if (m_axisName == axisName)
            return;

        m_axisName = axisName;
        emit axisNameChanged(m_axisName);
    }

    void setAxisType(AxisType axisType)
    {
        if (m_axisType == axisType)
            return;

        m_axisType = axisType;
        emit axisTypeChanged(m_axisType);
    }

    void setMasterAxisName(QString masterAxisName)
    {
        if (m_masterAxisName == masterAxisName)
            return;

        m_masterAxisName = masterAxisName;
        emit masterAxisNameChanged(m_masterAxisName);
    }

    void setIsDummyAxis(bool isDummyAxis)
    {
        if (m_isDummyAxis == isDummyAxis)
            return;

        m_isDummyAxis = isDummyAxis;
        emit isDummyAxisChanged(m_isDummyAxis);
    }

signals:
    void axisNameChanged(QString axisName);

    void axisTypeChanged(AxisType axisType);

    void masterAxisNameChanged(QString masterAxisName);

    void isDummyAxisChanged(bool isDummyAxis);

private:
    QString m_axisName;
    AxisType m_axisType{ GeneralAxis };
    QString m_masterAxisName;
    bool m_isDummyAxis = false;
};

class SoftlandingDefinition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)
    Q_PROPERTY(ConfigArray *softLandingPoses READ softLandingPoses)

public:
    Q_INVOKABLE SoftlandingDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_softLandingPoses = new ConfigArray(ConfigElementInfo::Other, this);
        disableTranslate();
        init();
    }

    ConfigArray *softLandingPoses() const
    {
        return m_softLandingPoses;
    }

    QString axisName() const
    {
        return m_axisName;
    }

public slots:
    void setAxisName(QString axisName)
    {
        if (m_axisName == axisName)
            return;

        m_axisName = axisName;
        emit axisNameChanged(m_axisName);
    }

signals:
    void axisNameChanged(QString axisName);

private:
    ConfigArray *m_softLandingPoses;
    QString m_axisName;
};

///
/// \brief The MotionElementDefinition class
/// Must be configed carefully by developer!
///
class SCMOTIONSHARED_EXPORT MotionElementDefinition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString processName READ processName WRITE setProcessName NOTIFY processNameChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(ConfigObjectArray *axisDefinitions READ axisDefinitions)
    Q_PROPERTY(ConfigArray *diNames READ diNames)
    Q_PROPERTY(ConfigArray *doNames READ doNames)
    Q_PROPERTY(ConfigArray *vacuumNames READ vacuumNames)
    Q_PROPERTY(ConfigArray *cylNames READ cylNames)
    Q_PROPERTY(ConfigObjectArray *axisModules READ axisModules)
    Q_PROPERTY(ConfigObjectArray *softlandingDefinition READ softlandingDefinition)

public:
    Q_INVOKABLE MotionElementDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_axisDefinitions = new ConfigObjectArray(&AxisDefinition::staticMetaObject, this);
        m_diNames = new ConfigArray(ConfigElementInfo::Other);
        m_doNames = new ConfigArray(ConfigElementInfo::Other);
        m_vacuumNames = new ConfigArray(ConfigElementInfo::Other);
        m_cylNames = new ConfigArray(ConfigElementInfo::Other);
        m_axisModules = new ConfigObjectArray(&AxisModuleDefinition::staticMetaObject, this);
        m_softlandingDefinitions = new ConfigObjectArray(&SoftlandingDefinition::staticMetaObject, this);
        disableTranslate();
        init();
    }

    QString address() const
    {
        return m_address;
    }

    ConfigArray *diNames() const
    {
        return m_diNames;
    }

    ConfigArray *doNames() const
    {
        return m_doNames;
    }

    ConfigArray *vacuumNames() const
    {
        return m_vacuumNames;
    }

    ConfigArray *cylNames() const
    {
        return m_cylNames;
    }

    ConfigObjectArray *axisModules() const
    {
        return m_axisModules;
    }

    QString processName() const
    {
        return m_processName;
    }

    ConfigObjectArray *axisDefinitions() const
    {
        return m_axisDefinitions;
    }

    ConfigObjectArray *softlandingDefinition() const
    {
        return m_softlandingDefinitions;
    }

public slots:
    void setAddress(QString address)
    {
        if (m_address == address)
            return;

        m_address = address;
        emit addressChanged(m_address);
    }

    void setProcessName(QString processName)
    {
        if (m_processName == processName)
            return;

        m_processName = processName;
        emit processNameChanged(m_processName);
    }

signals:
    void addressChanged(QString address);

    void processNameChanged(QString processName);

private:
    QString m_address;
    ConfigArray *m_diNames;
    ConfigArray *m_doNames;
    ConfigArray *m_vacuumNames;
    ConfigArray *m_cylNames;
    ConfigObjectArray *m_axisModules;
    QString m_processName;
    ConfigObjectArray *m_axisDefinitions;
    ConfigObjectArray *m_softlandingDefinitions;
};

#endif    // MOTIONELEMENTDEFINITION_H
