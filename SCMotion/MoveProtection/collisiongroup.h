#ifndef COLLISIONGROUP_H
#define COLLISIONGROUP_H

#include "collisioncondition.h"
#include "configManager/configobjectarray.h"
#include "ignorecondition.h"
#include "scmotion_global.h"
#include <QObject>

class DummyCmpOperation : public ComparisonOperation
{
    Q_OBJECT

public:
    DummyCmpOperation(QObject *parent = nullptr) : ComparisonOperation(parent) {}

    // ComparisonOperation interface
public:
    virtual bool compare(double value) override
    {
        Q_UNUSED(value);
        return true;
    }
    virtual QString getCmpDescription() override
    {
        return "DummyCmpOperation";
    }
    virtual void setLimitState(double limitState, QVariant parameter) override
    {
        Q_UNUSED(limitState)
        Q_UNUSED(parameter)
    }
};

class DummyCollisionCondition : public CollisionCondition
{
    Q_OBJECT

public:
    Q_INVOKABLE DummyCollisionCondition(QObject *parent = nullptr) : CollisionCondition(parent)
    {
        setCmpOperation(new DummyCmpOperation(this));
        init();
    }

    // CollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override
    {
        Q_UNUSED(name)
        Q_UNUSED(targetState)
        return true;
    }

    virtual QString getCollisionDescription() override
    {
        return "DummyCollisionCondition";
    }

    virtual QStringList getAffectedMotionElementNames() override
    {
        return QStringList();
    }

    virtual double getCurrentState() override
    {
        return -9999;
    }
};

class SCMOTIONSHARED_EXPORT CollisionConditionsConfig : public ConfigObjectArray
{
    Q_OBJECT

public:
    CollisionConditionsConfig() : ConfigObjectArray(&DummyCollisionCondition::staticMetaObject) {}

    Q_INVOKABLE bool useTableToRepresent() const
    {
        return false;
    }

    Q_INVOKABLE bool add(int index, QString collisionConditionType, QString cmpOperationType);

    QStringList getAffectedMotionElementNames();

    // ConfigBase interface
public:
    virtual bool read(const QJsonValue &jsonValue) override;
};

class DummyIgnoreCollisionCondition : public IgnoreCollisionCondition
{
    Q_OBJECT

public:
    Q_INVOKABLE DummyIgnoreCollisionCondition(QObject *parent = nullptr) : IgnoreCollisionCondition(parent)
    {
        init();
    }

    // IgnoreCollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override
    {
        Q_UNUSED(name);
        Q_UNUSED(targetState);
        return false;
    }
};

class SCMOTIONSHARED_EXPORT IgnoreCollisionConditionsConfig : public ConfigObjectArray
{
    Q_OBJECT

public:
    IgnoreCollisionConditionsConfig() : ConfigObjectArray(&DummyIgnoreCollisionCondition::staticMetaObject) {}

    Q_INVOKABLE bool add(int index, QString ignoreCollisionConditionType);

    // ConfigBase interface
public:
    virtual bool read(const QJsonValue &jsonValue) override;
};

class SCMOTIONSHARED_EXPORT CollisionGroup : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString groupDescription READ groupDescription WRITE setGroupDescription NOTIFY groupDescriptionChanged)
    Q_PROPERTY(CollisionConditionsConfig *collisionConditions READ collisionConditions)
    Q_PROPERTY(IgnoreCollisionConditionsConfig *ignoreCollisionConditions READ ignoreCollisionConditions)
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)

public:
    Q_INVOKABLE CollisionGroup(QObject *parent = nullptr);

    bool willCollide(QString name, double targetState);

    QString getCollisionDescription();

    QString groupDescription() const
    {
        return m_groupDescription;
    }

    CollisionConditionsConfig *collisionConditions() const
    {
        return m_collisionConditions;
    }

    IgnoreCollisionConditionsConfig *ignoreCollisionConditions() const
    {
        return m_ignoreCollisionConditions;
    }

    Q_INVOKABLE QVariantList getCollisionConditionTypes() const
    {
        return toVariantList(CollisionCondition::TypeEnumInfo().names());
    }

    Q_INVOKABLE QVariantList getIgnoreCollisionConditionTypes() const
    {
        return toVariantList(IgnoreCollisionCondition::TypeEnumInfo().names());
    }

    Q_INVOKABLE QVariantList getComparisonOperationTypes() const
    {
        return toVariantList(ComparisonOperation::TypeEnumInfo().names());
    }

    bool enable() const
    {
        return m_enable;
    }

public slots:
    void setGroupDescription(QString groupDescription)
    {
        if (m_groupDescription == groupDescription)
            return;

        m_groupDescription = groupDescription;
        emit groupDescriptionChanged(m_groupDescription);
    }

    void setEnable(bool enable)
    {
        if (m_enable == enable)
            return;

        if (!enable)
        {
            qWarning() << "Disable collision group:" << groupDescription();
        }
        m_enable = enable;
        emit enableChanged(m_enable);
    }

signals:
    void groupDescriptionChanged(QString groupDescription);

    void enableChanged(bool enable);

private:
    QString m_groupDescription;
    CollisionConditionsConfig *m_collisionConditions;
    IgnoreCollisionConditionsConfig *m_ignoreCollisionConditions;
    bool m_enable = true;
};

#endif    // COLLISIONGROUP_H
