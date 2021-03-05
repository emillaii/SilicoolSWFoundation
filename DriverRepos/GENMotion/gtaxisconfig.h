#ifndef GTAXISCONFIG_H
#define GTAXISCONFIG_H

#include "BasicElement/axisconfig.h"
#include "enumhelper.h"

class GTHomeConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int ecatHomeMethod READ ecatHomeMethod WRITE setEcatHomeMethod NOTIFY ecatHomeMethodChanged)
    Q_PROPERTY(double ecatSearchHomeSpeed READ ecatSearchHomeSpeed WRITE setEcatSearchHomeSpeed NOTIFY ecatSearchHomeSpeedChanged)
    Q_PROPERTY(double ecatSearchIndexSpeed READ ecatSearchIndexSpeed WRITE setEcatSearchIndexSpeed NOTIFY ecatSearchIndexSpeedChanged)
    Q_PROPERTY(double ecatHomeAcc READ ecatHomeAcc WRITE setEcatHomeAcc NOTIFY ecatHomeAccChanged)
    Q_PROPERTY(double ecatHomeOffset READ ecatHomeOffset WRITE setEcatHomeOffset NOTIFY ecatHomeOffsetChanged)

    Q_PROPERTY(GTHomeMode gtHomeMode READ gtHomeMode WRITE setGtHomeMode NOTIFY gtHomeModeChanged)
    Q_PROPERTY(int homeDir READ homeDir WRITE setHomeDir NOTIFY homeDirChanged)
    Q_PROPERTY(int indexDir READ indexDir WRITE setIndexDir NOTIFY indexDirChanged)
    Q_PROPERTY(int edge READ edge WRITE setEdge NOTIFY edgeChanged)
    Q_PROPERTY(double velHigh READ velHigh WRITE setVelHigh NOTIFY velHighChanged)
    Q_PROPERTY(double velLow READ velLow WRITE setVelLow NOTIFY velLowChanged)

public:
    enum GTHomeMode
    {
        MODE_LIMIT_HOME,
        MODE_LIMIT_INDEX,
        MODE_HOME,
        MODE_INDEX
    };
    Q_ENUM(GTHomeMode)

    static EnumHelper<GTHomeMode> &GTHomeModeEnumInfo()
    {
        static EnumHelper<GTHomeMode> instance(staticMetaObject, "GTHomeMode");
        return instance;
    }

    GTHomeConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        QVariantList zeroOne;
        zeroOne << 0 << 1;
        setOptionalProperty("homeDir", zeroOne);
        setOptionalProperty("indexDir", zeroOne);
        setOptionalProperty("edge", zeroOne);
        init();
    }

    GTHomeMode gtHomeMode() const
    {
        return m_gtHomeMode;
    }

    int homeDir() const
    {
        return m_homeDir;
    }

    int indexDir() const
    {
        return m_indexDir;
    }

    int edge() const
    {
        return m_edge;
    }

    double velHigh() const
    {
        return m_velHigh;
    }

    double velLow() const
    {
        return m_velLow;
    }

    int ecatHomeMethod() const
    {
        return m_ecatHomeMethod;
    }

    double ecatSearchHomeSpeed() const
    {
        return m_ecatSearchHomeSpeed;
    }

    double ecatSearchIndexSpeed() const
    {
        return m_ecatSearchIndexSpeed;
    }

    double ecatHomeAcc() const
    {
        return m_ecatHomeAcc;
    }

    double ecatHomeOffset() const
    {
        return m_ecatHomeOffset;
    }

public slots:
    void setGtHomeMode(GTHomeMode gtHomeMode)
    {
        if (m_gtHomeMode == gtHomeMode)
            return;

        m_gtHomeMode = gtHomeMode;
        emit gtHomeModeChanged(m_gtHomeMode);
    }

    void setHomeDir(int homeDir)
    {
        if (m_homeDir == homeDir)
            return;

        m_homeDir = homeDir;
        emit homeDirChanged(m_homeDir);
    }

    void setIndexDir(int indexDir)
    {
        if (m_indexDir == indexDir)
            return;

        m_indexDir = indexDir;
        emit indexDirChanged(m_indexDir);
    }

    void setEdge(int edge)
    {
        if (m_edge == edge)
            return;

        m_edge = edge;
        emit edgeChanged(m_edge);
    }

    void setVelHigh(double velHigh)
    {
        if (qFuzzyCompare(m_velHigh, velHigh))
            return;

        m_velHigh = velHigh;
        emit velHighChanged(m_velHigh);
    }

    void setVelLow(double velLow)
    {
        if (qFuzzyCompare(m_velLow, velLow))
            return;

        m_velLow = velLow;
        emit velLowChanged(m_velLow);
    }

    void setEcatHomeMethod(int ecatHomeMethod)
    {
        if (m_ecatHomeMethod == ecatHomeMethod)
            return;

        m_ecatHomeMethod = ecatHomeMethod;
        emit ecatHomeMethodChanged(m_ecatHomeMethod);
    }

    void setEcatSearchHomeSpeed(double ecatSearchHomeSpeed)
    {
        if (qFuzzyCompare(m_ecatSearchHomeSpeed, ecatSearchHomeSpeed))
            return;

        m_ecatSearchHomeSpeed = ecatSearchHomeSpeed;
        emit ecatSearchHomeSpeedChanged(m_ecatSearchHomeSpeed);
    }

    void setEcatSearchIndexSpeed(double ecatSearchIndexSpeed)
    {
        if (qFuzzyCompare(m_ecatSearchIndexSpeed, ecatSearchIndexSpeed))
            return;

        m_ecatSearchIndexSpeed = ecatSearchIndexSpeed;
        emit ecatSearchIndexSpeedChanged(m_ecatSearchIndexSpeed);
    }

    void setEcatHomeAcc(double ecatHomeAcc)
    {
        if (qFuzzyCompare(m_ecatHomeAcc, ecatHomeAcc))
            return;

        m_ecatHomeAcc = ecatHomeAcc;
        emit ecatHomeAccChanged(m_ecatHomeAcc);
    }

    void setEcatHomeOffset(double ecatHomeOffset)
    {
        if (qFuzzyCompare(m_ecatHomeOffset, ecatHomeOffset))
            return;

        m_ecatHomeOffset = ecatHomeOffset;
        emit ecatHomeOffsetChanged(m_ecatHomeOffset);
    }

signals:
    void gtHomeModeChanged(GTHomeMode gtHomeMode);

    void homeDirChanged(int homeDir);

    void indexDirChanged(int indexDir);

    void edgeChanged(int edge);

    void velHighChanged(double velHigh);

    void velLowChanged(double velLow);

    void ecatHomeMethodChanged(int ecatHomeMethod);

    void ecatSearchHomeSpeedChanged(double ecatSearchHomeSpeed);

    void ecatSearchIndexSpeedChanged(double ecatSearchIndexSpeed);

    void ecatHomeAccChanged(double ecatHomeAcc);

    void ecatHomeOffsetChanged(double ecatHomeOffset);

private:
    GTHomeMode m_gtHomeMode{ MODE_LIMIT_HOME };
    int m_homeDir = 0;
    int m_indexDir = 1;
    int m_edge = 1;
    double m_velHigh = 1;
    double m_velLow = 0.1;
    int m_ecatHomeMethod = 1;
    double m_ecatSearchHomeSpeed = 1;
    double m_ecatSearchIndexSpeed = 1;
    double m_ecatHomeAcc = 10;
    double m_ecatHomeOffset = 0;
};

class GTAxisConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(int coreNo READ coreNo WRITE setCoreNo NOTIFY coreNoChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(bool isEthercatAxis READ isEthercatAxis WRITE setIsEthercatAxis NOTIFY isEthercatAxisChanged)
    Q_PROPERTY(int posModeIndex READ posModeIndex WRITE setPosModeIndex NOTIFY posModeIndexChanged)
    Q_PROPERTY(int jogModeIndex READ jogModeIndex WRITE setJogModeIndex NOTIFY jogModeIndexChanged)
    Q_PROPERTY(int torModeIndex READ torModeIndex WRITE setTorModeIndex NOTIFY torModeIndexChanged)
    Q_PROPERTY(int inPosBand READ inPosBand WRITE setInPosBand NOTIFY inPosBandChanged)
    Q_PROPERTY(int inPosHoldTime READ inPosHoldTime WRITE setInPosHoldTime NOTIFY inPosHoldTimeChanged)
    Q_PROPERTY(int softLandingWindowSize READ softLandingWindowSize WRITE setSoftLandingWindowSize NOTIFY softLandingWindowSizeChanged)
    Q_PROPERTY(int softLandingWindowLen READ softLandingWindowLen WRITE setSoftLandingWindowLen NOTIFY softLandingWindowLenChanged)
    Q_PROPERTY(GTHomeConfig *gtHomeConfig READ gtHomeConfig)

public:
    Q_INVOKABLE GTAxisConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
        m_gtHomeConfig = new GTHomeConfig(this);
        init();
    }

    int coreNo() const
    {
        return m_coreNo;
    }

    int index() const
    {
        return m_index;
    }

    int inPosBand() const
    {
        return m_inPosBand;
    }

    int inPosHoldTime() const
    {
        return m_inPosHoldTime;
    }

    GTHomeConfig *gtHomeConfig() const
    {
        return m_gtHomeConfig;
    }

    bool isEthercatAxis() const
    {
        return m_isEthercatAxis;
    }

    int softLandingWindowSize() const
    {
        return m_softLandingWindowSize;
    }

    int softLandingWindowLen() const
    {
        return m_softLandingWindowLen;
    }

    int posModeIndex() const
    {
        return m_posModeIndex;
    }

    int torModeIndex() const
    {
        return m_torModeIndex;
    }

    int jogModeIndex() const
    {
        return m_jogModeIndex;
    }

public slots:
    void setCoreNo(int coreNo)
    {
        if (m_coreNo == coreNo)
            return;

        m_coreNo = coreNo;
        emit coreNoChanged(m_coreNo);
    }

    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

    void setInPosBand(int inPosBand)
    {
        if (m_inPosBand == inPosBand)
            return;

        m_inPosBand = inPosBand;
        emit inPosBandChanged(m_inPosBand);
    }

    void setInPosHoldTime(int inPosHoldTime)
    {
        if (m_inPosHoldTime == inPosHoldTime)
            return;

        m_inPosHoldTime = inPosHoldTime;
        emit inPosHoldTimeChanged(m_inPosHoldTime);
    }

    void setIsEthercatAxis(bool isEthercatAxis)
    {
        if (m_isEthercatAxis == isEthercatAxis)
            return;

        m_isEthercatAxis = isEthercatAxis;
        emit isEthercatAxisChanged(m_isEthercatAxis);
    }

    void setSoftLandingWindowSize(int softLandingWindowSize)
    {
        if (m_softLandingWindowSize == softLandingWindowSize)
            return;

        m_softLandingWindowSize = softLandingWindowSize;
        emit softLandingWindowSizeChanged(m_softLandingWindowSize);
    }

    void setSoftLandingWindowLen(int softLandingWindowLen)
    {
        if (m_softLandingWindowLen == softLandingWindowLen)
            return;

        m_softLandingWindowLen = softLandingWindowLen;
        emit softLandingWindowLenChanged(m_softLandingWindowLen);
    }

    void setPosModeIndex(int posModeIndex)
    {
        if (m_posModeIndex == posModeIndex)
            return;

        m_posModeIndex = posModeIndex;
        emit posModeIndexChanged(m_posModeIndex);
    }

    void setTorModeIndex(int torModeIndex)
    {
        if (m_torModeIndex == torModeIndex)
            return;

        m_torModeIndex = torModeIndex;
        emit torModeIndexChanged(m_torModeIndex);
    }

    void setJogModeIndex(int jogModeIndex)
    {
        if (m_jogModeIndex == jogModeIndex)
            return;

        m_jogModeIndex = jogModeIndex;
        emit jogModeIndexChanged(m_jogModeIndex);
    }

signals:
    void coreNoChanged(int coreNo);

    void indexChanged(int index);

    void inPosBandChanged(int inPosBand);

    void inPosHoldTimeChanged(int inPosHoldTime);

    void isEthercatAxisChanged(bool isEthercatAxis);

    void softLandingWindowSizeChanged(int softLandingWindowSize);

    void softLandingWindowLenChanged(int softLandingWindowLen);

    void posModeIndexChanged(int posModeIndex);

    void torModeIndexChanged(int torModeIndex);

    void jogModeIndexChanged(int jogModeIndex);

private:
    int m_coreNo = 1;
    int m_index = 1;
    int m_inPosBand = 1;
    int m_inPosHoldTime = 3;
    GTHomeConfig *m_gtHomeConfig;
    bool m_isEthercatAxis = true;
    int m_softLandingWindowSize = 10;
    int m_softLandingWindowLen = 10;
    int m_posModeIndex = 8;
    int m_torModeIndex = 4;
    int m_jogModeIndex = 3;
};

#endif    // GTAXISCONFIG_H
