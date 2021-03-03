#ifndef XYZMODULE_H
#define XYZMODULE_H

#include "configManager/configarray.h"
#include "scmotion_global.h"
#include "xymodule.h"

class SCMOTIONSHARED_EXPORT MoveSequence : public QObject
{
    Q_OBJECT

public:
    enum Sequence
    {
        XYZ,
        Z_XY,
        XY_Z,
        Z_X_Y,
        Z_Y_X,
        ZS_XY_Z,
        ZS_X_YZ,
        ZS_Y_XZ,
        ZS_XS_Y_XZ,
        ZS_YS_X_YZ
    };
    Q_ENUM(Sequence)

    static EnumHelper<Sequence> &SequenceEnumInfo()
    {
        static EnumHelper<Sequence> instance(staticMetaObject, "Sequence");
        return instance;
    }
};

class SCMOTIONSHARED_EXPORT MoveSequenceTransitionPosPrecision : public ConfigObject
{
    Q_OBJECT

public:
    Q_PROPERTY(MoveSequence::Sequence moveSequence READ moveSequence WRITE setMoveSequence NOTIFY moveSequenceChanged)
    Q_PROPERTY(ConfigArray *transitionPosPrecision READ transitionPosPrecision)

public:
    Q_INVOKABLE MoveSequenceTransitionPosPrecision(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_transitionPosPrecision = new ConfigArray(ConfigElementInfo::Double, this);
        init();
    }

    double transitionPosPrecision(int index)
    {
        if (index < m_transitionPosPrecision->count())
        {
            return m_transitionPosPrecision->at(index).toDouble();
        }
        return -1;
    }
    MoveSequence::Sequence moveSequence() const
    {
        return m_moveSequence;
    }
    ConfigArray *transitionPosPrecision() const
    {
        return m_transitionPosPrecision;
    }

public slots:
    void setMoveSequence(MoveSequence::Sequence moveSequence)
    {
        if (m_moveSequence == moveSequence)
            return;

        m_moveSequence = moveSequence;
        emit moveSequenceChanged(m_moveSequence);
    }
signals:
    void moveSequenceChanged(MoveSequence::Sequence moveSequence);

private:
    MoveSequence::Sequence m_moveSequence{ MoveSequence::ZS_XY_Z };
    ConfigArray *m_transitionPosPrecision;
};

class SCMOTIONSHARED_EXPORT XYZModulePos : public XYModulePos
{
    Q_OBJECT

public:
    Q_PROPERTY(double zPos READ zPos WRITE setZPos NOTIFY zPosChanged)
    Q_PROPERTY(double zPrecision READ zPrecision WRITE setZPrecision NOTIFY zPrecisionChanged)
    Q_PROPERTY(double zVelRatio READ zVelRatio WRITE setZVelRatio NOTIFY zVelRatioChanged)
    Q_PROPERTY(double zAccRatio READ zAccRatio WRITE setZAccRatio NOTIFY zAccRatioChanged)
    Q_PROPERTY(MoveSequence::Sequence safetyMoveSequence READ safetyMoveSequence WRITE setSafetyMoveSequence NOTIFY safetyMoveSequenceChanged)
    Q_PROPERTY(ConfigObjectArray *transitionPosPrecisions READ transitionPosPrecisions)

public:
    Q_INVOKABLE XYZModulePos();

    MoveSequenceTransitionPosPrecision *transitionPosPrecision(MoveSequence::Sequence moveSequence) const
    {
        if (transitionPosPrecisionMap.contains(moveSequence))
        {
            return transitionPosPrecisionMap[moveSequence];
        }
        return nullptr;
    }

    double zPos() const
    {
        return m_zPos;
    }

    double zPrecision() const
    {
        return m_zPrecision;
    }

    double zVelRatio() const
    {
        return m_zVelRatio;
    }

    double zAccRatio() const
    {
        return m_zAccRatio;
    }

    MoveSequence::Sequence safetyMoveSequence() const
    {
        return m_safetyMoveSequence;
    }

    ConfigObjectArray *transitionPosPrecisions() const
    {
        return m_transitionPosPrecisions;
    }

public slots:
    void setZPos(double zPos)
    {
        if (qFuzzyCompare(m_zPos, zPos))
            return;

        m_zPos = zPos;
        emit zPosChanged(m_zPos);
    }

    void setZPrecision(double zPrecision)
    {
        if (qFuzzyCompare(m_zPrecision, zPrecision))
            return;

        m_zPrecision = zPrecision;
        emit zPrecisionChanged(m_zPrecision);
    }

    void setZVelRatio(double zVelRatio)
    {
        if (qFuzzyCompare(m_zVelRatio, zVelRatio))
            return;

        m_zVelRatio = zVelRatio;
        emit zVelRatioChanged(m_zVelRatio);
    }

    void setZAccRatio(double zAccRatio)
    {
        if (qFuzzyCompare(m_zAccRatio, zAccRatio))
            return;

        m_zAccRatio = zAccRatio;
        emit zAccRatioChanged(m_zAccRatio);
    }

    void setSafetyMoveSequence(MoveSequence::Sequence safetyMoveSequence)
    {
        if (m_safetyMoveSequence == safetyMoveSequence)
            return;

        m_safetyMoveSequence = safetyMoveSequence;
        emit safetyMoveSequenceChanged(m_safetyMoveSequence);
    }

signals:
    void zPosChanged(double zPos);

    void zPrecisionChanged(double zPrecision);

    void zVelRatioChanged(double zVelRatio);

    void zAccRatioChanged(double zAccRatio);

    void safetyMoveSequenceChanged(MoveSequence::Sequence safetyMoveSequence);

private slots:
    void onTransitionPosPrecisionConfigChanged();
    void onTimeout();

    // ModulePos interface
protected:
    virtual MotionElement::Type getModuleType() override
    {
        return MotionElement::XYZModule;
    }

private:
    double m_zPos = 0;
    double m_zPrecision = 0.1;
    double m_zVelRatio = -1;
    double m_zAccRatio = -1;
    MoveSequence::Sequence m_safetyMoveSequence = { MoveSequence::ZS_XY_Z };
    ConfigObjectArray *m_transitionPosPrecisions;
    QMap<MoveSequence::Sequence, MoveSequenceTransitionPosPrecision *> transitionPosPrecisionMap;
    QTimer timer;
};

class SCMOTIONSHARED_EXPORT XYZModuleConfig : public AxisModuleConfig
{
    Q_OBJECT

    Q_PROPERTY(QString xAxisName READ xAxisName WRITE setXAxisName NOTIFY xAxisNameChanged)
    Q_PROPERTY(QString yAxisName READ yAxisName WRITE setYAxisName NOTIFY yAxisNameChanged)
    Q_PROPERTY(QString zAxisName READ zAxisName WRITE setZAxisName NOTIFY zAxisNameChanged)
    Q_PROPERTY(double zSafetyPos READ zSafetyPos WRITE setZSafetyPos NOTIFY zSafetyPosChanged)
    Q_PROPERTY(double xSafetyPos READ xSafetyPos WRITE setXSafetyPos NOTIFY xSafetyPosChanged)
    Q_PROPERTY(double ySafetyPos READ ySafetyPos WRITE setYSafetyPos NOTIFY ySafetyPosChanged)
    Q_PROPERTY(bool useZSafetyPos READ useZSafetyPos WRITE setUseZSafetyPos NOTIFY useZSafetyPosChanged)
    Q_PROPERTY(bool useXSafetyPos READ useXSafetyPos WRITE setUseXSafetyPos NOTIFY useXSafetyPosChanged)
    Q_PROPERTY(bool useYSafetyPos READ useYSafetyPos WRITE setUseYSafetyPos NOTIFY useYSafetyPosChanged)

public:
    Q_INVOKABLE XYZModuleConfig(QObject *parent = nullptr);

    QString xAxisName() const
    {
        return m_xAxisName;
    }

    QString yAxisName() const
    {
        return m_yAxisName;
    }

    QString zAxisName() const
    {
        return m_zAxisName;
    }

public:
    virtual void setOptionalAxis(QVariantList axisNames) override
    {
        setOptionalProperty("xAxisName", axisNames);
        setOptionalProperty("yAxisName", axisNames);
        setOptionalProperty("zAxisName", axisNames);
        init();
    }

    double zSafetyPos() const
    {
        return m_zSafetyPos;
    }

    double xSafetyPos() const
    {
        return m_xSafetyPos;
    }

    double ySafetyPos() const
    {
        return m_ySafetyPos;
    }

    bool useZSafetyPos() const
    {
        return m_useZSafetyPos;
    }

    bool useXSafetyPos() const
    {
        return m_useXSafetyPos;
    }

    bool useYSafetyPos() const
    {
        return m_useYSafetyPos;
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

    void setZAxisName(QString zAxisName)
    {
        if (m_zAxisName == zAxisName)
            return;

        m_zAxisName = zAxisName;
        emit zAxisNameChanged(m_zAxisName);
    }

    void setZSafetyPos(double zSafetyPos)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_zSafetyPos, zSafetyPos))
            return;

        m_zSafetyPos = zSafetyPos;
        emit zSafetyPosChanged(m_zSafetyPos);
    }

    void setXSafetyPos(double xSafetyPos)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_xSafetyPos, xSafetyPos))
            return;

        m_xSafetyPos = xSafetyPos;
        emit xSafetyPosChanged(m_xSafetyPos);
    }

    void setYSafetyPos(double ySafetyPos)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_ySafetyPos, ySafetyPos))
            return;

        m_ySafetyPos = ySafetyPos;
        emit ySafetyPosChanged(m_ySafetyPos);
    }

    void setUseZSafetyPos(bool useZSafetyPos)
    {
        if (m_useZSafetyPos == useZSafetyPos)
            return;

        m_useZSafetyPos = useZSafetyPos;
        emit useZSafetyPosChanged(m_useZSafetyPos);
    }

    void setUseXSafetyPos(bool useXSafetyPos)
    {
        if (m_useXSafetyPos == useXSafetyPos)
            return;

        m_useXSafetyPos = useXSafetyPos;
        emit useXSafetyPosChanged(m_useXSafetyPos);
    }

    void setUseYSafetyPos(bool useYSafetyPos)
    {
        if (m_useYSafetyPos == useYSafetyPos)
            return;

        m_useYSafetyPos = useYSafetyPos;
        emit useYSafetyPosChanged(m_useYSafetyPos);
    }

signals:
    void xAxisNameChanged(QString xAxisName);

    void yAxisNameChanged(QString yAxisName);

    void zAxisNameChanged(QString zAxisName);

    void zSafetyPosChanged(double zSafetyPos);

    void xSafetyPosChanged(double xSafetyPos);

    void ySafetyPosChanged(double ySafetyPos);

    void useZSafetyPosChanged(bool useZSafetyPos);

    void useXSafetyPosChanged(bool useXSafetyPos);

    void useYSafetyPosChanged(bool useYSafetyPos);

private:
    QString m_xAxisName;
    QString m_yAxisName;
    QString m_zAxisName;
    double m_zSafetyPos = 0;
    double m_xSafetyPos = 0;
    double m_ySafetyPos = 0;
    bool m_useZSafetyPos = false;
    bool m_useXSafetyPos = false;
    bool m_useYSafetyPos = false;
};

class SCMOTIONSHARED_EXPORT XYZModule : public AxisModule
{
    Q_OBJECT

    struct AxisTargetInfo
    {
    public:
        AxisTargetInfo(SCAxis *axis, double targetPos, double precision, double velRatio, double accRatio)
            : axis(axis), targetPos(targetPos), precision(precision), velRatio(velRatio), accRatio(accRatio)
        {
        }

        SCAxis *axis;
        double targetPos;
        double precision;
        double velRatio;
        double accRatio;
    };

public:
    XYZModule(QString name, XYZModuleConfig *xyzModuleConfig, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::XYZModule;
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

    SCAxis *zAxis() const
    {
        return m_zAxis;
    }

    void moveToWithOffset(QString posName,
                          MoveSequence::Sequence safetyMoveSequence,
                          double xOffset = 0,
                          double yOffset = 0,
                          double zOffset = 0,
                          bool waitDone = true,
                          bool logIt = true);
    void waitArrivedPosWithOffset(QString posName, double xOffset = 0, double yOffset = 0, double zOffset = 0);
    void relMove(double xStep = 0, double yStep = 0, double zStep = 0);
    void absMove(double x, double y, double z);
    void moveXYToPos(QString posName, bool waitDone = true);
    void moveZToPos(QString posName, bool waitDone = true);
    void waitXYArrivedPos(QString posName);
    void waitZArrivedPos(QString posName);
    void moveXyToPosWithOffset(QString posName, double xOffset, double yOffset, bool waitDone = true);
    void waitXyArrivedPosWithOffset(QString posName, double xOffset, double yOffset);
    void setNextPosXYVelAcc(QString posName);
    void moveXY(double xTarget, double yTarget, double precision = 0.1);
    void moveXyWithoutWait(const QPointF &xyTarget);
    void waitXyApproachPos(const QPointF &xyTarget, double approachWindow);
    void waitXyArrivedPos(const QPointF &xyTarget, double precision);
    void waitXyzProfilerStopped();
    void moveToPos(QString posName, MoveSequence::Sequence safetyMoveSequence, bool waitDone = true);

    void recordCurrentPos();
    void moveToRecordPos(MoveSequence::Sequence safetyMoveSequence);

    // AxisModule interface
protected:
    virtual void initImpl() override;

public slots:
    virtual void moveTo(QString posName, bool waitDone = true) override;
    virtual QVariantMap getModuleCurrentPos() override;

public:
    virtual void waitArrivedPos(QString posName) override;

private:
    const QString RecordPosName = "__RecordPos";
    XYZModuleConfig *xyzModuleConfig;
    XYZModulePos *recordPos = nullptr;
    SCAxis *m_xAxis = nullptr;
    SCAxis *m_yAxis = nullptr;
    SCAxis *m_zAxis = nullptr;
};

#endif    // XYZMODULE_H
