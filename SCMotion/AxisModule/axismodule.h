#ifndef AXISMODULE_H
#define AXISMODULE_H

#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"
#include "motionelementdefinition.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT AxisModuleConfig;

class SCMOTIONSHARED_EXPORT ModulePos : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString posName READ posName WRITE setPosName NOTIFY posNameChanged)

public:
    ModulePos();

    QString posName() const
    {
        return m_posName;
    }

    void setAxisModuleConfig(AxisModuleConfig *value);

    void updatePosWithCurrentPos(QVariantMap axesCurrentPos);

signals:
    void updatePosReq(QString moduleName, MotionElement::Type moduleType, QString posName);
    void moveToReq(QString moduleName, MotionElement::Type moduleType, QString posName);

    // ConfigObject interface
public:
    virtual void handleEvent(QString event) override;

public slots:
    void setPosName(QString posName)
    {
        if (m_posName == posName)
            return;

        m_posName = posName;
        emit posNameChanged(m_posName);
    }

signals:
    void posNameChanged(QString posName);

protected:
    virtual MotionElement::Type getModuleType() = 0;

private:
    QString m_posName = "";
    AxisModuleConfig *axisModuleConfig = nullptr;
};

class SCMOTIONSHARED_EXPORT AxisModuleConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
    Q_PROPERTY(ConfigObjectArray *modulePositions READ modulePositions)

public:
    AxisModuleConfig(const QMetaObject *modulePosMetaObj, QObject *parent = nullptr);

    QString moduleName() const
    {
        return m_moduleName;
    }

    ConfigObjectArray *modulePositions() const
    {
        return m_modulePositions;
    }

    void modulePos2Dic()
    {
        for (int i = 0; i < m_modulePositions->count(); i++)
        {
            ModulePos *modulePos = m_modulePositions->getConfig<ModulePos>(i);
            modulePositionMap[modulePos->posName()] = modulePos;
            modulePos->setAxisModuleConfig(this);
        }
    }

    void removeRedundantPos(AxisModuleDefinition *amd)
    {
        QStringList definedModulePoses = unpackVariantList<QString>(amd->modulePosNames()->members());
        for (int i = m_modulePositions->count() - 1; i >= 0; i--)
        {
            auto modulePos = m_modulePositions->getConfig<ModulePos>(i);
            if (!definedModulePoses.contains(modulePos->posName()))
            {
                m_modulePositions->remove(i);
                if (modulePositionMap.contains(modulePos->posName()))
                {
                    modulePositionMap.remove(modulePos->posName());
                }
            }
        }
    }

    void createLostPos(AxisModuleDefinition *amd)
    {
        for (int i = 0; i < amd->modulePosNames()->count(); i++)
        {
            QString posName = amd->modulePosNames()->at(i).toString();
            if (!modulePositionMap.contains(posName))
            {
                auto modulePos = qobject_cast<ModulePos *>(modulePosMetaObj->newInstance());
                modulePos->setPosName(posName);
                modulePos->setAxisModuleConfig(this);
                m_modulePositions->executeAddConfigObject(0, modulePos);
                modulePositionMap[posName] = modulePos;
            }
        }
    }

    bool containsPos(QString posName) const
    {
        return modulePositionMap.contains(posName);
    }

    template <typename T>
    T *getPos(QString posName)
    {
        if (modulePositionMap.contains(posName))
        {
            T *pos = qobject_cast<T *>(modulePositionMap[posName]);
            if (pos == nullptr)
            {
                throw SilicolAbort(tr("Can not cast pos '%1' to type '%2'.").arg(posName).arg(T::staticMetaObject.className()), EX_LOCATION);
            }
            return pos;
        }
        else
        {
            throw SilicolAbort(tr("Undefined module pos '%1' in module '%2'").arg(posName).arg(moduleName()), EX_LOCATION);
        }
    }

    QStringList modulePosNames() const
    {
        return modulePositionMap.keys();
    }
    // interface
public:
    virtual void setOptionalAxis(QVariantList axisNames) = 0;

public slots:
    void setModuleName(QString moduleName)
    {
        if (m_moduleName == moduleName)
            return;

        m_moduleName = moduleName;
        emit moduleNameChanged(m_moduleName);
    }

signals:
    void moduleNameChanged(QString moduleName);

private:
    QString m_moduleName = "";
    const QMetaObject *modulePosMetaObj;
    ConfigObjectArray *m_modulePositions;
    QMap<QString, ModulePos *> modulePositionMap;
};

class SCMOTIONSHARED_EXPORT AxisModule : public QObject
{
    Q_OBJECT

public:
    AxisModule(AxisModuleConfig *axisModuleConfig, QObject *parent = nullptr) : QObject(parent), m_axisModuleConfig(axisModuleConfig) {}

    void init();

    AxisModuleConfig *axisModuleConfig() const
    {
        return m_axisModuleConfig;
    }

    template <typename T>
    T *getPos(QString posName)
    {
        return m_axisModuleConfig->getPos<T>(posName);
    }

    bool useVelInPosCfg() const;
    void setUseVelInPosCfg(bool useVelInPosCfg);
    bool useAccInPosCfg() const;
    void setUseAccInPosCfg(bool useAccInPosCfg);

public slots:
    virtual void moveTo(QString posName, bool waitDone = true) = 0;
    virtual QVariantMap getModuleCurrentPos() = 0;

public:
    virtual void waitArrivedPos(QString posName) = 0;

protected:
    virtual void initImpl() = 0;

private:
    AxisModuleConfig *m_axisModuleConfig;
    bool m_useVelInPosCfg = true;
    bool m_useAccInPosCfg = true;
};

#endif    // AXISMODULE_H
