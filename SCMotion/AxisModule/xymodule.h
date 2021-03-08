#ifndef XYMODULE_H
#define XYMODULE_H

#include "BasicElement/scaxis.h"
#include "MotionManager/motionelementcontainer.h"
#include "axismodule.h"
#include "configManager/configobjectarray.h"
#include "scmotion_global.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT XYModulePos : public ModulePos
{
    Q_OBJECT

    Q_PROPERTY(double xPos READ xPos WRITE setXPos NOTIFY xPosChanged)
    Q_PROPERTY(double yPos READ yPos WRITE setYPos NOTIFY yPosChanged)
    Q_PROPERTY(double xPrecision READ xPrecision WRITE setXPrecision NOTIFY xPrecisionChanged)
    Q_PROPERTY(double yPrecision READ yPrecision WRITE setYPrecision NOTIFY yPrecisionChanged)
    Q_PROPERTY(double xVelRatio READ xVelRatio WRITE setXVelRatio NOTIFY xVelRatioChanged)
    Q_PROPERTY(double yVelRatio READ yVelRatio WRITE setYVelRatio NOTIFY yVelRatioChanged)
    Q_PROPERTY(double xAccRatio READ xAccRatio WRITE setXAccRatio NOTIFY xAccRatioChanged)
    Q_PROPERTY(double yAccRatio READ yAccRatio WRITE setYAccRatio NOTIFY yAccRatioChanged)

public:
    Q_INVOKABLE XYModulePos();

    double xPos() const
    {
        return m_xPos;
    }

    double yPos() const
    {
        return m_yPos;
    }

    double xPrecision() const
    {
        return m_xPrecision;
    }

    double yPrecision() const
    {
        return m_yPrecision;
    }

    double xVelRatio() const
    {
        return m_xVelRatio;
    }

    double yVelRatio() const
    {
        return m_yVelRatio;
    }

    double xAccRatio() const
    {
        return m_xAccRatio;
    }

    double yAccRatio() const
    {
        return m_yAccRatio;
    }

public slots:
    void setXPos(double xPos)
    {
        if (qFuzzyCompare(m_xPos, xPos))
            return;

        m_xPos = xPos;
        emit xPosChanged(m_xPos);
    }

    void setYPos(double yPos)
    {
        if (qFuzzyCompare(m_yPos, yPos))
            return;

        m_yPos = yPos;
        emit yPosChanged(m_yPos);
    }

    void setXPrecision(double xPrecision)
    {
        if (qFuzzyCompare(m_xPrecision, xPrecision))
            return;

        m_xPrecision = xPrecision;
        emit xPrecisionChanged(m_xPrecision);
    }

    void setYPrecision(double yPrecision)
    {
        if (qFuzzyCompare(m_yPrecision, yPrecision))
            return;

        m_yPrecision = yPrecision;
        emit yPrecisionChanged(m_yPrecision);
    }

    void setXVelRatio(double xVelRatio)
    {
        if (qFuzzyCompare(m_xVelRatio, xVelRatio))
            return;

        m_xVelRatio = xVelRatio;
        emit xVelRatioChanged(m_xVelRatio);
    }

    void setYVelRatio(double yVelRatio)
    {
        if (qFuzzyCompare(m_yVelRatio, yVelRatio))
            return;

        m_yVelRatio = yVelRatio;
        emit yVelRatioChanged(m_yVelRatio);
    }

    void setXAccRatio(double xAccRatio)
    {
        if (qFuzzyCompare(m_xAccRatio, xAccRatio))
            return;

        m_xAccRatio = xAccRatio;
        emit xAccRatioChanged(m_xAccRatio);
    }

    void setYAccRatio(double yAccRatio)
    {
        if (qFuzzyCompare(m_yAccRatio, yAccRatio))
            return;

        m_yAccRatio = yAccRatio;
        emit yAccRatioChanged(m_yAccRatio);
    }

signals:
    void xPosChanged(double xPos);

    void yPosChanged(double yPos);

    void xPrecisionChanged(double xPrecision);

    void yPrecisionChanged(double yPrecision);

    void xVelRatioChanged(double xVelRatio);

    void yVelRatioChanged(double yVelRatio);

    void xAccRatioChanged(double xAccRatio);

    void yAccRatioChanged(double yAccRatio);

    // ModulePos interface
protected:
    virtual MotionElement::Type getModuleType() override
    {
        return MotionElement::XYModule;
    }

private:
    double m_xPos = 0;
    double m_yPos = 0;
    double m_xPrecision = 0.1;
    double m_yPrecision = 0.1;
    double m_xVelRatio = -1;
    double m_yVelRatio = -1;
    double m_xAccRatio = -1;
    double m_yAccRatio = -1;
};

class SCMOTIONSHARED_EXPORT XYModuleConfig : public AxisModuleConfig
{
    Q_OBJECT

    Q_PROPERTY(QString xAxisName READ xAxisName WRITE setXAxisName NOTIFY xAxisNameChanged)
    Q_PROPERTY(QString yAxisName READ yAxisName WRITE setYAxisName NOTIFY yAxisNameChanged)

public:
    Q_INVOKABLE XYModuleConfig(QObject *parent = nullptr);

    QString xAxisName() const
    {
        return m_xAxisName;
    }

    QString yAxisName() const
    {
        return m_yAxisName;
    }

public:
    virtual void setOptionalAxis(QVariantList axisNames) override
    {
        setOptionalProperty("xAxisName", axisNames);
        setOptionalProperty("yAxisName", axisNames);
        init();
    }

public slots:
    void setXAxisName(QString xAxisName)
    {
        if (m_xAxisName == xAxisName)
            return;

        m_xAxisName = xAxisName;
        emit xAxisNameChanged(m_xAxisName);
    }

    void setYAxisName(QString yAxisName)
    {
        if (m_yAxisName == yAxisName)
            return;

        m_yAxisName = yAxisName;
        emit yAxisNameChanged(m_yAxisName);
    }

signals:
    void xAxisNameChanged(QString xAxisName);

    void yAxisNameChanged(QString yAxisName);

private:
    QString m_xAxisName;
    QString m_yAxisName;
};

class SCMOTIONSHARED_EXPORT XYModule : public AxisModule
{
    Q_OBJECT

public:
    XYModule(QString name, XYModuleConfig *xyModuleConfig, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::XYModule;
    }

    QString name() const
    {
        return objectName();
    }

    SCAxis *xAxis() const
    {
        return m_xAxis;
    }

    SCAxis *yAxis() const
    {
        return m_yAxis;
    }

    void moveToWithOffset(QString posName, QPointF offset, bool waitDone = true);
    void waitArrivedPosWithOffset(QString posName, QPointF offset);
    void setNextPosVelAcc(QString posName);
    void moveTo(QPointF pos, bool waitDone = true, double precision = 0.1);
    void waitArrivedPos(QPointF pos, double precision = 0.1);
    void relMove(QPointF step, double precision = 0.1);

    // AxisModule interface
protected:
    virtual void initImpl() override;

public slots:
    virtual void moveTo(QString posName, bool waitDone = true) override;
    virtual QVariantMap getModuleCurrentPos() override;

public:
    virtual void waitArrivedPos(QString posName) override;

private:
    void setNextPosVelAcc(XYModulePos *pos)
    {
        double xVelRatio = useVelInPosCfg() ? pos->xVelRatio() : -1;
        double xAccRatio = useAccInPosCfg() ? pos->xAccRatio() : -1;
        double yVelRatio = useVelInPosCfg() ? pos->yVelRatio() : -1;
        double yAccRatio = useAccInPosCfg() ? pos->yAccRatio() : -1;

        m_xAxis->setNextMoveVelAccRatio(xVelRatio, xAccRatio);
        m_yAxis->setNextMoveVelAccRatio(yVelRatio, yAccRatio);
    }

private:
    XYModuleConfig *xyModuleConfig;
    SCAxis *m_xAxis = nullptr;
    SCAxis *m_yAxis = nullptr;
};

#endif    // XYMODULE_H
