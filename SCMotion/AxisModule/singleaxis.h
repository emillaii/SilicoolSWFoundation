#ifndef SINGLEAXIS_H
#define SINGLEAXIS_H

#include "BasicElement/scaxis.h"
#include "MotionManager/motionelementcontainer.h"
#include "axismodule.h"
#include "configManager/configobjectarray.h"
#include "scmotion_global.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT SingleAxisPos : public ModulePos
{
    Q_OBJECT

    Q_PROPERTY(double pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(double precision READ precision WRITE setPrecision NOTIFY precisionChanged)
    Q_PROPERTY(double velRatio READ velRatio WRITE setVelRatio NOTIFY velRatioChanged)
    Q_PROPERTY(double accRatio READ accRatio WRITE setAccRatio NOTIFY accRatioChanged)

public:
    Q_INVOKABLE SingleAxisPos();

    double pos() const
    {
        return m_pos;
    }

    double precision() const
    {
        return m_precision;
    }

    double velRatio() const
    {
        return m_velRatio;
    }

    double accRatio() const
    {
        return m_accRatio;
    }

public slots:
    void setPos(double pos)
    {
        if (qFuzzyCompare(m_pos, pos))
            return;

        m_pos = pos;
        emit posChanged(m_pos);
    }

    void setPrecision(double precision)
    {
        if (qFuzzyCompare(m_precision, precision))
            return;

        m_precision = precision;
        emit precisionChanged(m_precision);
    }

    void setVelRatio(double velRatio)
    {
        if (qFuzzyCompare(m_velRatio, velRatio))
            return;

        m_velRatio = velRatio;
        emit velRatioChanged(m_velRatio);
    }

    void setAccRatio(double accRatio)
    {
        if (qFuzzyCompare(m_accRatio, accRatio))
            return;

        m_accRatio = accRatio;
        emit accRatioChanged(m_accRatio);
    }

signals:
    void posChanged(double pos);

    void precisionChanged(double precision);

    void velRatioChanged(double velRatio);

    void accRatioChanged(double accRatio);

    // ModulePos interface
protected:
    virtual MotionElement::Type getModuleType() override
    {
        return MotionElement::SingleAxis;
    }

private:
    double m_pos = 0;
    double m_precision = 0.1;
    double m_velRatio = -1;
    double m_accRatio = -1;
};

class SCMOTIONSHARED_EXPORT SingleAxisConfig : public AxisModuleConfig
{
    Q_OBJECT

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)

public:
    Q_INVOKABLE SingleAxisConfig(QObject *parent = nullptr);

    QString axisName() const
    {
        return m_axisName;
    }

public:
    virtual void setOptionalAxis(QVariantList axisNames) override
    {
        setOptionalProperty("axisName", axisNames);
        init();
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
    QString m_axisName;
};

class SCMOTIONSHARED_EXPORT SingleAxis : public AxisModule
{
    Q_OBJECT
public:
    explicit SingleAxis(QString moduleName, SingleAxisConfig *sAxisConfig, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::SingleAxis;
    }

    QString name() const
    {
        return objectName();
    }

    SCAxis *axis() const
    {
        return m_axis;
    }

    void moveToWithOffset(QString posName, double offset, bool waitDone = true);
    void waitArrivedPosWithOffset(QString posName, double offset);
    void tryToMove(QString posName, uint checkInterval = 10, uint stepCount = 10);
    bool greaterThanPos(QString posName);
    bool lessThanPos(QString posName);
    void waitGreaterThanPos(QString posName);
    void waitLessThanPos(QString posName);

    // AxisModule interface
protected:
    virtual void initImpl() override;

public slots:
    virtual void moveTo(QString posName, bool waitDone = true) override;
    virtual QVariantMap getModuleCurrentPos() override;

public:
    virtual void waitArrivedPos(QString posName) override;

private:
    void setNextPosVelAcc(SingleAxisPos *pos)
    {
        double velRatio = useVelInPosCfg() ? pos->velRatio() : -1;
        double accRatio = useAccInPosCfg() ? pos->accRatio() : -1;
        m_axis->setNextMoveVelAccRatio(velRatio, accRatio);
    }

private:
    SingleAxisConfig *sAxisConfig = nullptr;
    SCAxis *m_axis = nullptr;
};

#endif    // SINGLEAXIS_H
