#ifndef COLLISIONCONDITION_H
#define COLLISIONCONDITION_H

#include "MotionManager/motionstategetter.h"
#include "comparisonoperation.h"
#include "configManager/configobject.h"
#include "errorHandling/actionerror.h"
#include "globalmotionelements.h"
#include "rep_motionManager_source.h"
#include "scmotion_global.h"
#include <QDebug>

class SCMOTIONSHARED_EXPORT CollisionCondition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString collisionConditionType READ collisionConditionType WRITE setcollisionConditionType NOTIFY
                   collisionConditionTypeChanged)
    Q_PROPERTY(ComparisonOperation *cmpOperation READ cmpOperation WRITE setCmpOperation)

public:
    enum Type
    {
        SingleAxisCollisionCondition,
        TwoAxisCollisionCondition,
        CylinderCollisionCondition,
        DICollisionCondition
    };
    Q_ENUM(Type)

    static EnumHelper<Type> &TypeEnumInfo()
    {
        static EnumHelper<Type> instance(staticMetaObject, "Type");
        return instance;
    }

    static CollisionCondition *createInstance(Type type, QObject *parent = nullptr);

    CollisionCondition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("collisionConditionType");
    }

    QString collisionConditionType() const
    {
        return m_collisionConditionType;
    }

    ComparisonOperation *cmpOperation() const
    {
        return m_cmpOperation;
    }

    Q_INVOKABLE ComparisonOperation *getCmpOperation() const
    {
        return m_cmpOperation;
    }

    Q_INVOKABLE void setLimitStateWithCurrentState(QVariant parameter = QVariant())
    {
        cmpOperation()->setLimitState(getCurrentState(), parameter);
    }

    // interface
public:
    virtual bool meetCondition(QString name, double targetState) = 0;
    virtual QString getCollisionDescription() = 0;
    virtual QStringList getAffectedMotionElementNames() = 0;
    virtual double getCurrentState() = 0;

public slots:
    void setcollisionConditionType(QString collisionConditionType)
    {
        if (m_collisionConditionType == collisionConditionType)
            return;

        m_collisionConditionType = collisionConditionType;
        emit collisionConditionTypeChanged(m_collisionConditionType);
    }

    void setCmpOperation(ComparisonOperation *cmpOperation)
    {
        m_cmpOperation = cmpOperation;
    }

signals:
    void collisionConditionTypeChanged(QString collisionConditionType);

private:
    QString m_collisionConditionType;
    ComparisonOperation *m_cmpOperation;
};

class SCMOTIONSHARED_EXPORT SingleAxisCollision : public CollisionCondition
{
    Q_OBJECT

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)

public:
    Q_INVOKABLE SingleAxisCollision(QObject *parent = nullptr) : CollisionCondition(parent)
    {
        setOptionalProperty("axisName", GlobalMotionElements::getIns().axisNames());
    }

    QString axisName() const
    {
        return m_axisName;
    }

    // CollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override;

    virtual QString getCollisionDescription() override;

    virtual QStringList getAffectedMotionElementNames() override;

    virtual double getCurrentState() override;

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
    double currentPos;
    double targetPos;
    QString m_axisName;
};

class SCMOTIONSHARED_EXPORT TwoAxisCollision : public CollisionCondition
{
    Q_OBJECT

public:
    enum PosComposition
    {
        Add,
        Minus
    };
    Q_ENUM(PosComposition)

    Q_PROPERTY(QString axis1Name READ axis1Name WRITE setAxis1Name NOTIFY axis1NameChanged)
    Q_PROPERTY(PosComposition posComposition READ posComposition WRITE setPosComposition NOTIFY posCompositionChanged)
    Q_PROPERTY(QString axis2Name READ axis2Name WRITE setAxis2Name NOTIFY axis2NameChanged)

public:
    Q_INVOKABLE TwoAxisCollision(QObject *parent = nullptr);

    QString axis1Name() const
    {
        return m_axis1Name;
    }

    PosComposition posComposition() const
    {
        return m_posComposition;
    }

    QString axis2Name() const
    {
        return m_axis2Name;
    }

    // CollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override;

    virtual QString getCollisionDescription() override;

    virtual QStringList getAffectedMotionElementNames() override;

    virtual double getCurrentState() override;

public slots:
    void setAxis1Name(QString axis1Name)
    {
        if (m_axis1Name == axis1Name)
            return;

        m_axis1Name = axis1Name;
        emit axis1NameChanged(m_axis1Name);
    }

    void setPosComposition(PosComposition posComposition)
    {
        if (m_posComposition == posComposition)
            return;

        m_posComposition = posComposition;
        emit posCompositionChanged(m_posComposition);
    }

    void setAxis2Name(QString axis2Name)
    {
        if (m_axis2Name == axis2Name)
            return;

        m_axis2Name = axis2Name;
        emit axis2NameChanged(m_axis2Name);
    }

signals:
    void axis1NameChanged(QString axis1Name);

    void posCompositionChanged(PosComposition posComposition);

    void axis2NameChanged(QString axis2Name);

private:
    QString getPosComposition();

private:
    double axis1CurrentPos;
    double axis2CurrentPos;
    double axis1TargetPos;
    double axis2TargetPos;
    QString m_axis1Name;
    PosComposition m_posComposition;
    QString m_axis2Name;
};

class SCMOTIONSHARED_EXPORT CylinderCollision : public CollisionCondition
{
    Q_OBJECT

    Q_PROPERTY(QString cylName READ cylName WRITE setCylName NOTIFY cylNameChanged)

public:
    Q_INVOKABLE CylinderCollision(QObject *parent = nullptr) : CollisionCondition(parent)
    {
        setOptionalProperty("cylName", GlobalMotionElements::getIns().cylNames());
    }

    QString cylName() const
    {
        return m_cylName;
    }

    // CollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override;

    virtual QString getCollisionDescription() override;

    virtual QStringList getAffectedMotionElementNames() override;

    virtual double getCurrentState() override;

public slots:
    void setCylName(QString cylName)
    {
        if (m_cylName == cylName)
            return;

        m_cylName = cylName;
        emit cylNameChanged(m_cylName);
    }

signals:
    void cylNameChanged(QString cylName);

private:
    double cylState;
    QString m_cylName;
};

class SCMOTIONSHARED_EXPORT DICollision : public CollisionCondition
{
    Q_OBJECT

    Q_PROPERTY(QString diName READ diName WRITE setDiName NOTIFY diNameChanged)

public:
    DICollision(QObject *parent = nullptr) : CollisionCondition(parent)
    {
        setOptionalProperty("diName", GlobalMotionElements::getIns().diNames());
    }

    QString diName() const
    {
        return m_diName;
    }

    // CollisionCondition interface
public:
    virtual bool meetCondition(QString name, double targetState) override;
    virtual QString getCollisionDescription() override;
    virtual QStringList getAffectedMotionElementNames() override;
    virtual double getCurrentState() override;

public slots:
    void setDiName(QString diName)
    {
        if (m_diName == diName)
            return;

        m_diName = diName;
        emit diNameChanged(m_diName);
    }

signals:
    void diNameChanged(QString diName);

private:
    bool diState;
    QString m_diName;
};

#endif    // COLLISIONCONDITION_H
