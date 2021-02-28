#ifndef XTVCMCONFIG_H
#define XTVCMCONFIG_H

#include "BasicElement/axisconfig.h"

class XtVcmConfig : public AxisConfig
{
    Q_OBJECT

public:
    enum HomeMode
    {
        FindZeroIO,
        TouchFindIndexIO,
        FindZeroIOThenIndexIO
    };
    Q_ENUM(HomeMode)

    Q_PROPERTY(int canID READ canID WRITE setCanID NOTIFY canIDChanged)
    Q_PROPERTY(int homeDirection READ homeDirection WRITE setHomeDirection NOTIFY homeDirectionChanged)
    Q_PROPERTY(int runDirection READ runDirection WRITE setRunDirection NOTIFY runDirectionChanged)
    Q_PROPERTY(double homeOffset READ homeOffset WRITE setHomeOffset NOTIFY homeOffsetChanged)
    Q_PROPERTY(HomeMode homeMode READ homeMode WRITE setHomeMode NOTIFY homeModeChanged)
    Q_PROPERTY(double touchLimitCurrent READ touchLimitCurrent WRITE setTouchLimitCurrent NOTIFY touchLimitCurrentChanged)
    Q_PROPERTY(double findIndexIOMaxDistance READ findIndexIOMaxDistance WRITE setFindIndexIOMaxDistance NOTIFY findIndexIOMaxDistanceChanged)
    Q_PROPERTY(double findIndexIOMaxVelocity READ findIndexIOMaxVelocity WRITE setFindIndexIOMaxVelocity NOTIFY findIndexIOMaxVelocityChanged)
    Q_PROPERTY(int findIndexIODir READ findIndexIODir WRITE setFindIndexIODir NOTIFY findIndexIODirChanged)
    Q_PROPERTY(double maxJerk READ maxJerk WRITE setMaxJerk NOTIFY maxJerkChanged)
    Q_PROPERTY(double minCurrentLimit READ minCurrentLimit WRITE setMinCurrentLimit NOTIFY minCurrentLimitChanged)
    Q_PROPERTY(double maxCurrentLimit READ maxCurrentLimit WRITE setMaxCurrentLimit NOTIFY maxCurrentLimitChanged)

    Q_PROPERTY(int fslWindowLen READ fslWindowLen WRITE setFslWindowLen NOTIFY fslWindowLenChanged)
    Q_PROPERTY(double fslInPosError READ fslInPosError WRITE setFslInPosError NOTIFY fslInPosErrorChanged)
    Q_PROPERTY(int fslDetectionInterval READ fslDetectionInterval WRITE setFslDetectionInterval NOTIFY fslDetectionIntervalChanged)

    Q_PROPERTY(bool needMapCurrent2Force READ needMapCurrent2Force WRITE setNeedMapCurrent2Force NOTIFY needMapCurrent2ForceChanged)
    Q_PROPERTY(QString currentArray READ currentArray WRITE setCurrentArray NOTIFY currentArrayChanged)
    Q_PROPERTY(QString forceArray READ forceArray WRITE setForceArray NOTIFY forceArrayChanged)

public:
    Q_INVOKABLE XtVcmConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
        QVariantList dirOptions;
        dirOptions.append(0);
        dirOptions.append(1);
        setOptionalProperty("homeDirection", dirOptions);
        setOptionalProperty("runDirection", dirOptions);
        setOptionalProperty("findIndexIODir", dirOptions);
        init();
    }

    int canID() const
    {
        return m_canID;
    }

    int homeDirection() const
    {
        return m_homeDirection;
    }

    int runDirection() const
    {
        return m_runDirection;
    }

    double homeOffset() const
    {
        return m_homeOffset;
    }

    double maxJerk() const
    {
        return m_maxJerk;
    }

    double minCurrentLimit() const
    {
        return m_minCurrentLimit;
    }

    double maxCurrentLimit() const
    {
        return m_maxCurrentLimit;
    }

    bool needMapCurrent2Force() const
    {
        return m_needMapCurrent2Force;
    }

    HomeMode homeMode() const
    {
        return m_homeMode;
    }

    QString currentArray() const
    {
        return m_currentArray;
    }

    QString forceArray() const
    {
        return m_forceArray;
    }

    double touchLimitCurrent() const
    {
        return m_touchLimitCurrent;
    }

    double findIndexIOMaxDistance() const
    {
        return m_findIndexIOMaxDistance;
    }

    double findIndexIOMaxVelocity() const
    {
        return m_findIndexIOMaxVelocity;
    }

    int findIndexIODir() const
    {
        return m_findIndexIODir;
    }

    int fslWindowLen() const
    {
        return m_fslWindowLen;
    }

    double fslInPosError() const
    {
        return m_fslInPosError;
    }

    int fslDetectionInterval() const
    {
        return m_fslDetectionInterval;
    }

public slots:
    void setCanID(int canID)
    {
        if (m_canID == canID)
            return;

        m_canID = canID;
        emit canIDChanged(m_canID);
    }

    void setHomeDirection(int homeDirection)
    {
        if (m_homeDirection == homeDirection)
            return;

        m_homeDirection = homeDirection;
        emit homeDirectionChanged(m_homeDirection);
    }

    void setRunDirection(int runDirection)
    {
        if (m_runDirection == runDirection)
            return;

        m_runDirection = runDirection;
        emit runDirectionChanged(m_runDirection);
    }

    void setHomeOffset(double homeOffset)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_homeOffset, homeOffset))
            return;

        m_homeOffset = homeOffset;
        emit homeOffsetChanged(m_homeOffset);
    }

    void setMaxJerk(double maxJerk)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxJerk, maxJerk))
            return;

        m_maxJerk = maxJerk;
        emit maxJerkChanged(m_maxJerk);
    }

    void setMinCurrentLimit(double minCurrentLimit)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_minCurrentLimit, minCurrentLimit))
            return;

        m_minCurrentLimit = minCurrentLimit;
        emit minCurrentLimitChanged(m_minCurrentLimit);
    }

    void setMaxCurrentLimit(double maxCurrentLimit)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxCurrentLimit, maxCurrentLimit))
            return;

        m_maxCurrentLimit = maxCurrentLimit;
        emit maxCurrentLimitChanged(m_maxCurrentLimit);
    }

    void setNeedMapCurrent2Force(bool needMapCurrent2Force)
    {
        if (m_needMapCurrent2Force == needMapCurrent2Force)
            return;

        m_needMapCurrent2Force = needMapCurrent2Force;
        emit needMapCurrent2ForceChanged(m_needMapCurrent2Force);
    }

    void setHomeMode(HomeMode homeMode)
    {
        if (m_homeMode == homeMode)
            return;

        m_homeMode = homeMode;
        emit homeModeChanged(m_homeMode);
    }

    void setCurrentArray(QString currentArray)
    {
        if (m_currentArray == currentArray)
            return;

        m_currentArray = currentArray;
        emit currentArrayChanged(m_currentArray);
    }

    void setForceArray(QString forceArray)
    {
        if (m_forceArray == forceArray)
            return;

        m_forceArray = forceArray;
        emit forceArrayChanged(m_forceArray);
    }

    void setTouchLimitCurrent(double touchLimitCurrent)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_touchLimitCurrent, touchLimitCurrent))
            return;

        m_touchLimitCurrent = touchLimitCurrent;
        emit touchLimitCurrentChanged(m_touchLimitCurrent);
    }

    void setFindIndexIOMaxDistance(double findIndexIOMaxDistance)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_findIndexIOMaxDistance, findIndexIOMaxDistance))
            return;

        m_findIndexIOMaxDistance = findIndexIOMaxDistance;
        emit findIndexIOMaxDistanceChanged(m_findIndexIOMaxDistance);
    }

    void setFindIndexIOMaxVelocity(double findIndexIOMaxVelocity)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_findIndexIOMaxVelocity, findIndexIOMaxVelocity))
            return;

        m_findIndexIOMaxVelocity = findIndexIOMaxVelocity;
        emit findIndexIOMaxVelocityChanged(m_findIndexIOMaxVelocity);
    }

    void setFindIndexIODir(int findIndexIODir)
    {
        if (m_findIndexIODir == findIndexIODir)
            return;

        m_findIndexIODir = findIndexIODir;
        emit findIndexIODirChanged(m_findIndexIODir);
    }

    void setFslWindowLen(int fslWindowLen)
    {
        if (m_fslWindowLen == fslWindowLen)
            return;

        m_fslWindowLen = fslWindowLen;
        emit fslWindowLenChanged(m_fslWindowLen);
    }

    void setFslInPosError(double fslInPosError)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_fslInPosError, fslInPosError))
            return;

        m_fslInPosError = fslInPosError;
        emit fslInPosErrorChanged(m_fslInPosError);
    }

    void setFslDetectionInterval(int fslDetectionInterval)
    {
        if (m_fslDetectionInterval == fslDetectionInterval)
            return;

        m_fslDetectionInterval = fslDetectionInterval;
        emit fslDetectionIntervalChanged(m_fslDetectionInterval);
    }

signals:
    void canIDChanged(int canID);

    void homeDirectionChanged(int homeDirection);

    void runDirectionChanged(int runDirection);

    void homeOffsetChanged(double homeOffset);

    void maxJerkChanged(double maxJerk);

    void minCurrentLimitChanged(double minCurrentLimit);

    void maxCurrentLimitChanged(double maxCurrentLimit);

    void needMapCurrent2ForceChanged(bool needMapCurrent2Force);

    void homeModeChanged(HomeMode homeMode);

    void currentArrayChanged(QString currentArray);

    void forceArrayChanged(QString forceArray);

    void touchLimitCurrentChanged(double touchLimitCurrent);

    void findIndexIOMaxDistanceChanged(double findIndexIOMaxDistance);

    void findIndexIOMaxVelocityChanged(double findIndexIOMaxVelocity);

    void findIndexIODirChanged(int findIndexIODir);

    void fslWindowLenChanged(int fslWindowLen);

    void fslInPosErrorChanged(double fslInPosError);

    void fslDetectionIntervalChanged(int fslDetectionInterval);

private:
    int m_canID = 0;
    double m_homeOffset = 0;
    double m_maxJerk = 10000;
    double m_minCurrentLimit = -10;
    double m_maxCurrentLimit = 10;
    int m_homeDirection = 0;
    int m_runDirection = 0;
    bool m_needMapCurrent2Force = true;
    HomeMode m_homeMode{ FindZeroIO };
    QString m_currentArray = "0.2,0.5,0.8,1.2";
    QString m_forceArray = "10,330,660,1000";
    double m_touchLimitCurrent = 0.5;
    double m_findIndexIOMaxDistance = 5;
    double m_findIndexIOMaxVelocity = 2;
    int m_findIndexIODir = 1;
    int m_fslWindowLen = 10;
    double m_fslInPosError = 0.002;
    int m_fslDetectionInterval = 1;
};

#endif    // XTVCMCONFIG_H
