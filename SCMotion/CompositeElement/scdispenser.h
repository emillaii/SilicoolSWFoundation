#ifndef SCDISPENSER_H
#define SCDISPENSER_H

#include "BasicElement/scaxis.h"
#include "configManager/configarray.h"
#include "configManager/configobject.h"
#include "dispensepath.h"
#include "errorHandling/silicolerror.h"
#include "scmotion_global.h"
#include <QVector>

class SCMOTIONSHARED_EXPORT GeneralDispenserConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(bool addAnExtraPath READ addAnExtraPath WRITE setAddAnExtraPath NOTIFY addAnExtraPathChanged)
    Q_PROPERTY(double openOffset READ openOffset WRITE setOpenOffset NOTIFY openOffsetChanged)
    Q_PROPERTY(double closeOffset READ closeOffset WRITE setCloseOffset NOTIFY closeOffsetChanged)
    Q_PROPERTY(double maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)
    Q_PROPERTY(double maxAcc READ maxAcc WRITE setMaxAcc NOTIFY maxAccChanged)
    Q_PROPERTY(bool useLineSpeeds READ useLineSpeeds WRITE setUseLineSpeeds NOTIFY useLineSpeedsChanged)
    Q_PROPERTY(ConfigArray *lineSpeeds READ lineSpeeds)
    Q_PROPERTY(bool useLineDisable READ useLineDisable WRITE setUseLineDisable NOTIFY useLineDisableChanged)
    Q_PROPERTY(ConfigArray *lineDisabled READ lineDisabled)

public:
    GeneralDispenserConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_lineSpeeds = new ConfigArray(ConfigElementInfo::Double, this);
        m_lineDisabled = new ConfigArray(ConfigElementInfo::Bool, this);
        init();
    }

    bool addAnExtraPath() const
    {
        return m_addAnExtraPath;
    }

    double openOffset() const
    {
        return m_openOffset;
    }

    double closeOffset() const
    {
        return m_closeOffset;
    }

    double maxSpeed() const
    {
        return m_maxSpeed;
    }

    bool useLineSpeeds() const
    {
        return m_useLineSpeeds;
    }

    ConfigArray *lineSpeeds() const
    {
        return m_lineSpeeds;
    }

    bool useLineDisable() const
    {
        return m_useLineDisable;
    }

    ConfigArray *lineDisabled() const
    {
        return m_lineDisabled;
    }

    double maxAcc() const
    {
        return m_maxAcc;
    }

public slots:
    void setAddAnExtraPath(bool addAnExtraPath)
    {
        if (m_addAnExtraPath == addAnExtraPath)
            return;

        m_addAnExtraPath = addAnExtraPath;
        emit addAnExtraPathChanged(m_addAnExtraPath);
    }

    void setOpenOffset(double openOffset)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_openOffset, openOffset))
            return;

        m_openOffset = openOffset;
        emit openOffsetChanged(m_openOffset);
    }

    void setCloseOffset(double closeOffset)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_closeOffset, closeOffset))
            return;

        m_closeOffset = closeOffset;
        emit closeOffsetChanged(m_closeOffset);
    }

    void setMaxSpeed(double maxSpeed)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxSpeed, maxSpeed))
            return;

        m_maxSpeed = maxSpeed;
        emit maxSpeedChanged(m_maxSpeed);
    }

    void setUseLineSpeeds(bool useLineSpeeds)
    {
        if (m_useLineSpeeds == useLineSpeeds)
            return;

        m_useLineSpeeds = useLineSpeeds;
        emit useLineSpeedsChanged(m_useLineSpeeds);
    }

    void setUseLineDisable(bool useLineDisable)
    {
        if (m_useLineDisable == useLineDisable)
            return;

        m_useLineDisable = useLineDisable;
        emit useLineDisableChanged(m_useLineDisable);
    }

    void setMaxAcc(double maxAcc)
    {
        if (qFuzzyCompare(m_maxAcc, maxAcc))
            return;

        m_maxAcc = maxAcc;
        emit maxAccChanged(m_maxAcc);
    }

signals:
    void addAnExtraPathChanged(bool addAnExtraPath);

    void openOffsetChanged(double openOffset);

    void closeOffsetChanged(double closeOffset);

    void maxSpeedChanged(double maxSpeed);

    void useLineSpeedsChanged(bool useLineSpeeds);

    void useLineDisableChanged(bool useLineDisable);

    void maxAccChanged(double maxAcc);

private:
    bool m_addAnExtraPath = true;
    double m_openOffset = 0;
    double m_closeOffset = 0;
    double m_maxSpeed = 20;
    bool m_useLineSpeeds = false;
    ConfigArray *m_lineSpeeds;
    bool m_useLineDisable = false;
    ConfigArray *m_lineDisabled;
    double m_maxAcc = 3000;
};

class SCMOTIONSHARED_EXPORT FlyDispenserConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(double maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)
    Q_PROPERTY(double maxAcc READ maxAcc WRITE setMaxAcc NOTIFY maxAccChanged)
    Q_PROPERTY(double shotGlueDotStep READ shotGlueDotStep WRITE setShotGlueDotStep NOTIFY shotGlueDotStepChanged)
    Q_PROPERTY(double shotGlueDotTime READ shotGlueDotTime WRITE setShotGlueDotTime NOTIFY shotGlueDotTimeChanged)

public:
    FlyDispenserConfig(ConfigObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    double maxSpeed() const
    {
        return m_maxSpeed;
    }

    double shotGlueDotStep() const
    {
        return m_shotGlueDotStep;
    }

    double shotGlueDotTime() const
    {
        return m_shotGlueDotTime;
    }

    double maxAcc() const
    {
        return m_maxAcc;
    }

public slots:
    void setMaxSpeed(double maxSpeed)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxSpeed, maxSpeed))
            return;

        m_maxSpeed = maxSpeed;
        emit maxSpeedChanged(m_maxSpeed);
    }

    void setShotGlueDotStep(double shotGlueDotStep)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_shotGlueDotStep, shotGlueDotStep))
            return;

        m_shotGlueDotStep = shotGlueDotStep;
        emit shotGlueDotStepChanged(m_shotGlueDotStep);
    }

    void setShotGlueDotTime(double shotGlueDotTime)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_shotGlueDotTime, shotGlueDotTime))
            return;

        m_shotGlueDotTime = shotGlueDotTime;
        emit shotGlueDotTimeChanged(m_shotGlueDotTime);
    }

    void setMaxAcc(double maxAcc)
    {
        if (qFuzzyCompare(m_maxAcc, maxAcc))
            return;

        m_maxAcc = maxAcc;
        emit maxAccChanged(m_maxAcc);
    }

signals:
    void maxSpeedChanged(double maxSpeed);

    void shotGlueDotStepChanged(double shotGlueDotStep);

    void shotGlueDotTimeChanged(double shotGlueDotTime);

    void maxAccChanged(double maxAcc);

private:
    double m_maxSpeed = 60;
    double m_shotGlueDotStep = 0.1;
    double m_shotGlueDotTime = 0.002;
    double m_maxAcc = 1000;
};

class SCMOTIONSHARED_EXPORT DispenserConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(bool useFlyDispensing READ useFlyDispensing WRITE setUseFlyDispensing NOTIFY useFlyDispensingChanged)
    Q_PROPERTY(bool addPathToStartPos READ addPathToStartPos WRITE setAddPathToStartPos NOTIFY addPathToStartPosChanged)
    Q_PROPERTY(bool enableDispensing READ enableDispensing WRITE setEnableDispensing NOTIFY enableDispensingChanged)
    Q_PROPERTY(GeneralDispenserConfig *generalDispenserConfig READ generalDispenserConfig)
    Q_PROPERTY(FlyDispenserConfig *flyDispenserConfig READ flyDispenserConfig)

public:
    DispenserConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_generalDispenserConfig = new GeneralDispenserConfig(this);
        m_flyDispenserConfig = new FlyDispenserConfig(this);
        init();
    }

    bool useFlyDispensing() const
    {
        return m_useFlyDispensing;
    }

    GeneralDispenserConfig *generalDispenserConfig() const
    {
        return m_generalDispenserConfig;
    }

    FlyDispenserConfig *flyDispenserConfig() const
    {
        return m_flyDispenserConfig;
    }

    bool addPathToStartPos() const
    {
        return m_addPathToStartPos;
    }

    bool enableDispensing() const
    {
        return m_enableDispensing;
    }

public slots:
    void setUseFlyDispensing(bool useFlyDispensing)
    {
        if (m_useFlyDispensing == useFlyDispensing)
            return;

        m_useFlyDispensing = useFlyDispensing;
        emit useFlyDispensingChanged(m_useFlyDispensing);
    }

    void setAddPathToStartPos(bool addPathToStartPos)
    {
        if (m_addPathToStartPos == addPathToStartPos)
            return;

        m_addPathToStartPos = addPathToStartPos;
        emit addPathToStartPosChanged(m_addPathToStartPos);
    }

    void setEnableDispensing(bool enableDispensing)
    {
        if (m_enableDispensing == enableDispensing)
            return;

        m_enableDispensing = enableDispensing;
        emit enableDispensingChanged(m_enableDispensing);
    }

signals:
    void useFlyDispensingChanged(bool useFlyDispensing);

    void addPathToStartPosChanged(bool addPathToStartPos);

    void enableDispensingChanged(bool enableDispensing);

private:
    bool m_useFlyDispensing = false;
    GeneralDispenserConfig *m_generalDispenserConfig;
    FlyDispenserConfig *m_flyDispenserConfig;
    bool m_addPathToStartPos = true;
    bool m_enableDispensing = true;
};

class SCMOTIONSHARED_EXPORT SCDispenser : public QObject
{
    Q_OBJECT

public:
    SCDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent = nullptr);

    virtual void init(SCAxis *xAxis, SCAxis *yAxis, SCAxis *zAxis, SCDO *shotGlueOut);

    DispenserConfig *dispenserConfig() const
    {
        return m_dispConfig;
    }

    void generateLineSpeeds(int pointCount)
    {
        generateConfig(pointCount, m_dispConfig->generalDispenserConfig()->lineSpeeds(), m_dispConfig->generalDispenserConfig()->maxSpeed());
    }
    void generateLineDisabled(int pointCount)
    {
        generateConfig(pointCount, m_dispConfig->generalDispenserConfig()->lineDisabled(), false);
    }

    void dispensePath(QVector<PathEndPoint> &path);
    void dispenseLine(QPointF startPos, QPointF endPos, double dispenseHeight, double zSafetyHeight);
    void dispenseRect(QPointF leftTopPos, double width, double height, double dispenseHeight, double zSafetyHeight);

protected:
    virtual void enterInterpolationMode() = 0;
    virtual void exitInterpolationMode() = 0;
    virtual bool isFinished() = 0;
    virtual void executePath(QVector<PathEndPoint> &path) = 0;

    void moveToStartPos(double x, double y, double z, double zSafetyHeight);
    void __dispensePath(QVector<PathEndPoint> &path);
    void waitDone(int timeout = 60000);
    double getGeneraDispVel(int index);
    bool getLineDisabled(int index);
    void addExtraPath(QVector<PathEndPoint> &path);
    void setDispenseVel(QVector<PathEndPoint> &path);
    void setLineDisable(QVector<PathEndPoint> &path);
    void checkIsInit();

private:
    void generateConfig(int pointCount, ConfigArray *config, QVariant defaultValue);

private:
    const QLoggingCategory &logCate;
    DispenserConfig *m_dispConfig;
    SCAxis *xAxis;
    SCAxis *yAxis;
    SCAxis *zAxis;
    SCDO *shotGlueOut;
    bool isInit = false;
};

#endif    // SCDISPENSER_H
