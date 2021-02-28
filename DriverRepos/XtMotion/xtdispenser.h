#ifndef XTDISPENSER_H
#define XTDISPENSER_H

#include "CompositeElement/scdispenser.h"
#include "xtaxis.h"
#include <QLoggingCategory>

class XtFlyDispensingConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(double calibrateEscape READ calibrateEscape WRITE setCalibrateEscape NOTIFY calibrateEscapeChanged)
    Q_PROPERTY(double calibrateLineLen READ calibrateLineLen WRITE setCalibrateLineLen NOTIFY calibrateLineLenChanged)
    Q_PROPERTY(double calibrateMaxVel READ calibrateMaxVel WRITE setCalibrateMaxVel NOTIFY calibrateMaxVelChanged)
    Q_PROPERTY(double shotGlueDotDelay READ shotGlueDotDelay WRITE setShotGlueDotDelay NOTIFY shotGlueDotDelayChanged)
    Q_PROPERTY(double shotGlueDotTime READ shotGlueDotTime WRITE setShotGlueDotTime NOTIFY shotGlueDotTimeChanged)

public:
    XtFlyDispensingConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    double calibrateEscape() const
    {
        return m_calibrateEscape;
    }

    double calibrateLineLen() const
    {
        return m_calibrateLineLen;
    }

    double shotGlueDotDelay() const
    {
        return m_shotGlueDotDelay;
    }

    double shotGlueDotTime() const
    {
        return m_shotGlueDotTime;
    }

    double calibrateMaxVel() const
    {
        return m_calibrateMaxVel;
    }

public slots:
    void setCalibrateEscape(double calibrateEscape)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_calibrateEscape, calibrateEscape))
            return;

        m_calibrateEscape = calibrateEscape;
        emit calibrateEscapeChanged(m_calibrateEscape);
    }

    void setCalibrateLineLen(double calibrateLineLen)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_calibrateLineLen, calibrateLineLen))
            return;

        m_calibrateLineLen = calibrateLineLen;
        emit calibrateLineLenChanged(m_calibrateLineLen);
    }

    void setShotGlueDotDelay(double shotGlueDotDelay)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_shotGlueDotDelay, shotGlueDotDelay))
            return;

        m_shotGlueDotDelay = shotGlueDotDelay;
        emit shotGlueDotDelayChanged(m_shotGlueDotDelay);
    }

    void setShotGlueDotTime(double shotGlueDotTime)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_shotGlueDotTime, shotGlueDotTime))
            return;

        m_shotGlueDotTime = shotGlueDotTime;
        emit shotGlueDotTimeChanged(m_shotGlueDotTime);
    }

    void setCalibrateMaxVel(double calibrateMaxVel)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_calibrateMaxVel, calibrateMaxVel))
            return;

        m_calibrateMaxVel = calibrateMaxVel;
        emit calibrateMaxVelChanged(m_calibrateMaxVel);
    }

signals:
    void calibrateEscapeChanged(double calibrateEscape);

    void calibrateLineLenChanged(double calibrateLineLen);

    void shotGlueDotDelayChanged(double shotGlueDotDelay);

    void shotGlueDotTimeChanged(double shotGlueDotTime);

    void calibrateMaxVelChanged(double calibrateMaxVel);

private:
    double m_calibrateEscape = 2;
    double m_calibrateLineLen = 30;
    double m_shotGlueDotDelay = 0;
    double m_shotGlueDotTime = 0.005;
    double m_calibrateMaxVel = 80;
};

class XtDispenser : public SCDispenser
{
    Q_OBJECT

public:
    XtDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent = nullptr);

    void setXtFlyDispensingConfig(XtFlyDispensingConfig *xtFlyDispConfig)
    {
        this->xtFlyDispensingConfig = xtFlyDispConfig;
    }

    void shotGlueDotInLineCenter(QPointF xyStartPos,
                                 QPointF xyEndPos,
                                 double shotGlueDotZHeight,
                                 double safetyZHeight,
                                 double shotGlueTime,
                                 double shotGlueDelay);

    // SCDispenser interface
public:
    virtual void init(SCAxis *xAxis, SCAxis *yAxis, SCAxis *zAxis, SCDO *shotGlueOut) override;

protected:
    virtual void enterInterpolationMode() override {}
    virtual void exitInterpolationMode() override {}
    virtual bool isFinished() override
    {
        return 1 == XT_Controler_Extend::Check_Finish(curveId);
    }
    virtual void executePath(QVector<PathEndPoint> &path) override;

private:
    void mallocResource();
    void setCurveParam(int axisCount, QVector<int> &axis);
    void generalDispenseImpl(QVector<PathEndPoint> &dispensePath);
    void flyDispenseImpl(QVector<PathEndPoint> &dispensePath);

private:
    bool isMallocResource = false;
    int curveId;
    int threadCurve;
    int threadTrig;
    XtAxis *xtXAxis;
    XtAxis *xtYAxis;
    XtAxis *xtZAxis;
    XtDO *xtShotGlueOut;
    XtFlyDispensingConfig *xtFlyDispensingConfig = nullptr;
};

#endif    // XTDISPENSER_H
