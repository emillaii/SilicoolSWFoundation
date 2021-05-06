#ifndef COMPARISONOPERATION_H
#define COMPARISONOPERATION_H

#include "commonmethod.h"
#include "enumhelper.h"
#include "scmotion_global.h"
#include <QObject>
#include <configManager/configobject.h>

class SCMOTIONSHARED_EXPORT ComparisonOperation : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString cmpOperationType READ cmpOperationType WRITE setCmpOperationType NOTIFY cmpOperationTypeChanged)
public:
    enum Type
    {
        LessThanPos,
        GreaterThanPos,
        InPosRange,
        NotInPosRange,
        NotInState
    };
    Q_ENUM(Type)

    static EnumHelper<Type> &TypeEnumInfo()
    {
        static EnumHelper<Type> instance(staticMetaObject, "Type");
        return instance;
    }

    static ComparisonOperation *createInstance(Type type, QObject *parent = nullptr);

    ComparisonOperation(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("cmpOperationType");
    }

    virtual bool compare(double value) = 0;
    virtual bool compare2(double value1, double value2)
    {
        return compare(value1) || compare(value2);
    }
    virtual QString getCmpDescription() = 0;
    virtual void setLimitState(double limitState, QVariant parameter) = 0;

    QString cmpOperationType() const
    {
        return m_cmpOperationType;
    }

public slots:
    void setCmpOperationType(QString cmpOperationType)
    {
        if (m_cmpOperationType == cmpOperationType)
            return;

        m_cmpOperationType = cmpOperationType;
        emit cmpOperationTypeChanged(m_cmpOperationType);
    }

signals:
    void cmpOperationTypeChanged(QString cmpOperationType);

private:
    QString m_cmpOperationType;
};

class SCMOTIONSHARED_EXPORT LessThanPosCO : public ComparisonOperation
{
    Q_OBJECT

    Q_PROPERTY(double limitPos READ limitPos WRITE setLimitPos NOTIFY limitPosChanged)

public:
    Q_INVOKABLE LessThanPosCO(QObject *parent = nullptr) : ComparisonOperation(parent)
    {
        setEngineerAuthority("limitPos");
        init();
    }

    double limitPos() const
    {
        return m_limitPos;
    }

    // ComparisonOperation interface
public:
    virtual bool compare(double value) override
    {
        return value <= limitPos();
    }

    virtual QString getCmpDescription() override
    {
        return QString("less than pos %1").arg(limitPos());
    }

    virtual void setLimitState(double limitState, QVariant parameter) override
    {
        Q_UNUSED(parameter)
        setLimitPos(limitState);
    }

public slots:
    void setLimitPos(double limitPos)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_limitPos, limitPos))
            return;

        m_limitPos = limitPos;
        emit limitPosChanged(m_limitPos);
    }

signals:
    void limitPosChanged(double limitPos);

private:
    double m_limitPos = 0;
};

class SCMOTIONSHARED_EXPORT GreaterThanPosCO : public LessThanPosCO
{
    Q_OBJECT

public:
    Q_INVOKABLE GreaterThanPosCO(QObject *parent = nullptr) : LessThanPosCO(parent)
    {
        init();
    }

    // ComparisonOperation interface
public:
    virtual bool compare(double value) override
    {
        return !LessThanPosCO::compare(value);
    }

    virtual QString getCmpDescription() override
    {
        return QString("greater than pos %1").arg(limitPos());
    }
};

class SCMOTIONSHARED_EXPORT InPosRangeCO : public ComparisonOperation
{
    Q_OBJECT

    Q_PROPERTY(double limitPos1 READ limitPos1 WRITE setLimitPos1 NOTIFY limitPos1Changed)
    Q_PROPERTY(double limitPos2 READ limitPos2 WRITE setLimitPos2 NOTIFY limitPos2Changed)

public:
    Q_INVOKABLE InPosRangeCO(QObject *parent = nullptr) : ComparisonOperation(parent)
    {
        setEngineerAuthority("limitPos1");
        setEngineerAuthority("limitPos2");
        init();
    }

    // ComparisonOperation interface
public:
    virtual bool compare(double value) override
    {
        return (value >= limitPos1() && value <= limitPos2()) || (value >= limitPos2() && value <= limitPos1());
    }

    virtual bool compare2(double value1, double value2) override
    {
        double minValue = qMin(value1, value2);
        double maxValue = qMax(value1, value2);
        double minLimit = qMin(limitPos1(), limitPos2());
        double maxLimit = qMax(limitPos1(), limitPos2());
        return maxValue >= minLimit && minValue <= maxLimit;
    }

    virtual QString getCmpDescription() override
    {
        return QString("in pos range [%1, %2]").arg(limitPos1()).arg(limitPos2());
    }

    virtual void setLimitState(double limitState, QVariant parameter) override
    {
        if (parameter == 1)
        {
            setLimitPos1(limitState);
        }
        else if (parameter == 2)
        {
            setLimitPos2(limitState);
        }
        else
        {
            qCritical() << "Parameter error:" << parameter;
        }
    }

public:
    double limitPos1() const
    {
        return m_limitPos1;
    }
    double limitPos2() const
    {
        return m_limitPos2;
    }

public slots:
    void setLimitPos1(double limitPos1)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_limitPos1, limitPos1))
            return;

        m_limitPos1 = limitPos1;
        emit limitPos1Changed(m_limitPos1);
    }
    void setLimitPos2(double limitPos2)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_limitPos2, limitPos2))
            return;

        m_limitPos2 = limitPos2;
        emit limitPos2Changed(m_limitPos2);
    }

signals:
    void limitPos1Changed(double limitPos1);
    void limitPos2Changed(double limitPos2);

private:
    double m_limitPos1 = 0;

    double m_limitPos2 = 0;
};

class SCMOTIONSHARED_EXPORT NotInPosRangeCO : public InPosRangeCO
{
    Q_OBJECT

public:
    Q_INVOKABLE NotInPosRangeCO(QObject *parent = nullptr) : InPosRangeCO(parent)
    {
        init();
    }
    // ComparisonOperation interface
public:
    virtual bool compare(double value) override
    {
        return !InPosRangeCO::compare(value);
    }

    virtual bool compare2(double value1, double value2) override
    {
        double minValue = qMin(value1, value2);
        double maxValue = qMax(value1, value2);
        double minLimit = qMin(limitPos1(), limitPos2());
        double maxLimit = qMax(limitPos1(), limitPos2());
        return minValue >= maxLimit || maxValue <= minLimit;
    }

    virtual QString getCmpDescription() override
    {
        return QString("not in pos range [%1, %2]").arg(limitPos1()).arg(limitPos2());
    }
};

class SCMOTIONSHARED_EXPORT NotInStateCO : public ComparisonOperation
{
    Q_OBJECT

    Q_PROPERTY(int limitState READ limitState WRITE setLimitState NOTIFY limitStateChanged)

public:
    Q_INVOKABLE NotInStateCO(QObject *parent = nullptr) : ComparisonOperation(parent)
    {
        QVariantList qv;
        qv.append(0);
        qv.append(1);
        setOptionalProperty("limitState", qv);
        setEngineerAuthority("limitState");
        init();
    }

    int limitState() const
    {
        return m_limitState;
    }

    // ComparisonOperation interface
public:
    virtual bool compare(double value) override
    {
        return static_cast<int>(value) != limitState();
    }

    virtual QString getCmpDescription() override
    {
        return QString("not in state %1").arg(limitState());
    }

    virtual void setLimitState(double limitState, QVariant parameter) override
    {
        Q_UNUSED(parameter);
        setLimitState(static_cast<int>(limitState));
    }

public slots:
    void setLimitState(int limitState)
    {
        if (m_limitState == limitState)
            return;

        m_limitState = limitState;
        emit limitStateChanged(m_limitState);
    }

signals:
    void limitStateChanged(int limitState);

private:
    int m_limitState = 0;
};

#endif    // COMPARISONOPERATION_H
