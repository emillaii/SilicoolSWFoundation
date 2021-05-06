#ifndef IGNORECONDITION_H
#define IGNORECONDITION_H

#include "MotionManager/globalmotionelements.h"
#include "MotionManager/motionstategetter.h"
#include "configManager/configobject.h"
#include "enumhelper.h"
#include "rep_motionManager_source.h"
#include "scmotion_global.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT IgnoreCollisionCondition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString ignoreCollisionConditionType READ ignoreCollisionConditionType WRITE setIgnoreCollisionConditionType NOTIFY
                   ignoreCollisionConditionTypeChanged)

public:
    enum Type
    {
        Axis,
        Cylinder
    };
    Q_ENUM(Type)

    static EnumHelper<Type> &TypeEnumInfo()
    {
        static EnumHelper<Type> instance(staticMetaObject, "Type");
        return instance;
    }

    static IgnoreCollisionCondition *createInstance(Type type, QObject *parent = nullptr);

    IgnoreCollisionCondition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("ignoreCollisionConditionType");
    }

    QString ignoreCollisionConditionType() const
    {
        return m_ignoreCollisionConditionType;
    }

    // interface
public:
    virtual bool meetCondition(QString name, double targetState) = 0;

public slots:
    void setIgnoreCollisionConditionType(QString ignoreCollisionConditionType)
    {
        if (m_ignoreCollisionConditionType == ignoreCollisionConditionType)
            return;

        m_ignoreCollisionConditionType = ignoreCollisionConditionType;
        emit ignoreCollisionConditionTypeChanged(m_ignoreCollisionConditionType);
    }

signals:
    void ignoreCollisionConditionTypeChanged(QString ignoreCollisionConditionType);

private:
    QString m_ignoreCollisionConditionType;
};

class SCMOTIONSHARED_EXPORT AxisIgnoreCollisionCondition : public IgnoreCollisionCondition
{
    Q_OBJECT

public:
    enum MoveDirection
    {
        Positive,
        Negative
    };
    Q_ENUM(MoveDirection)

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)
    Q_PROPERTY(MoveDirection moveDirection READ moveDirection WRITE setMoveDirection NOTIFY moveDirectionChanged)

public:
    Q_INVOKABLE AxisIgnoreCollisionCondition(QObject *parent = nullptr) : IgnoreCollisionCondition(parent)
    {
        setOptionalProperty("axisName", GlobalMotionElements::getIns().axisNames());
        setEngineerAuthority("axisName");
        setEngineerAuthority("moveDirection");
        init();
    }

    QString axisName() const
    {
        return m_axisName;
    }

    MoveDirection moveDirection() const
    {
        return m_moveDirection;
    }

    // IgnoreCollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override
    {
        if (name != axisName())
        {
            return false;
        }
        double axisCurrentPos = MotionStateGetter::getIns().reqGetAxisFeedbackPos(name);
        if (qFuzzyCompare(axisCurrentPos, UNKNOWN_AXIS_POS))
        {
            return false;
        }
        if (targetState > axisCurrentPos && moveDirection() == Positive)
        {
            return true;
        }
        if (targetState < axisCurrentPos && moveDirection() == Negative)
        {
            return true;
        }
        return false;
    }

public slots:
    void setAxisName(QString axisName)
    {
        if (m_axisName == axisName)
            return;

        m_axisName = axisName;
        emit axisNameChanged(m_axisName);
    }

    void setMoveDirection(MoveDirection moveDirection)
    {
        if (m_moveDirection == moveDirection)
            return;

        m_moveDirection = moveDirection;
        emit moveDirectionChanged(m_moveDirection);
    }

signals:
    void axisNameChanged(QString axisName);

    void moveDirectionChanged(MoveDirection moveDirection);

private:
    QString m_axisName;
    MoveDirection m_moveDirection;
};

class SCMOTIONSHARED_EXPORT CylIgnoreCollisionCondition : public IgnoreCollisionCondition
{
    Q_OBJECT

    Q_PROPERTY(QString cylName READ cylName WRITE setCylName NOTIFY cylNameChanged)
    Q_PROPERTY(int targetState READ targetState WRITE setTargetState NOTIFY targetStateChanged)

public:
    Q_INVOKABLE CylIgnoreCollisionCondition(QObject *parent = nullptr) : IgnoreCollisionCondition(parent)
    {
        QVariantList qv;
        qv.append(0);
        qv.append(1);
        setOptionalProperty("targetState", qv);
        setOptionalProperty("cylName", GlobalMotionElements::getIns().cylNames());
        setEngineerAuthority("cylName");
        setEngineerAuthority("targetState");
        init();
    }

    QString cylName() const
    {
        return m_cylName;
    }

    int targetState() const
    {
        return m_targetState;
    }

    // IgnoreCollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override
    {
        if (name != cylName())
        {
            return false;
        }
        return static_cast<int>(targetState) == m_targetState;
    }

public slots:
    void setCylName(QString cylName)
    {
        if (m_cylName == cylName)
            return;

        m_cylName = cylName;
        emit cylNameChanged(m_cylName);
    }

    void setTargetState(int targetState)
    {
        if (m_targetState == targetState)
            return;

        m_targetState = targetState;
        emit targetStateChanged(m_targetState);
    }

signals:
    void cylNameChanged(QString cylName);

    void targetStateChanged(int targetState);

private:
    QString m_cylName;
    int m_targetState;
};

#endif    // IGNORECONDITION_H
