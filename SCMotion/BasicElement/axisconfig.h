#ifndef AXISCONFIG_H
#define AXISCONFIG_H

#include "commonmethod.h"
#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"
#include "enumhelper.h"
#include "scmotion_global.h"
#include "softlandingpos.h"

class SCMOTIONSHARED_EXPORT AdvancedAxisConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(bool jogMovable READ jogMovable WRITE setJogMovable NOTIFY jogMovableChanged)
    Q_PROPERTY(double jogMoveMaxVel READ jogMoveMaxVel WRITE setJogMoveMaxVel NOTIFY jogMoveMaxVelChanged)

    Q_PROPERTY(bool useSoftLimit READ useSoftLimit WRITE setUseSoftLimit NOTIFY useSoftLimitChanged)
    Q_PROPERTY(bool respondToGlobalVelRatioChange READ respondToGlobalVelRatioChange WRITE setRespondToGlobalVelRatioChange NOTIFY
                   respondToGlobalVelRatioChangeChanged)

    Q_PROPERTY(bool needHome READ needHome WRITE setNeedHome NOTIFY needHomeChanged)
    Q_PROPERTY(HomeExecutor homeExecutor READ homeExecutor WRITE setHomeExecutor NOTIFY homeExecutorChanged)
    Q_PROPERTY(int resetDriverTime READ resetDriverTime WRITE setResetDriverTime NOTIFY resetDriverTimeChanged)
    Q_PROPERTY(QString startHomeDoName READ startHomeDoName WRITE setStartHomeDoName NOTIFY startHomeDoNameChanged)
    Q_PROPERTY(QString homeDoneDiName READ homeDoneDiName WRITE setHomeDoneDiName NOTIFY homeDoneDiNameChanged)
    Q_PROPERTY(double homeOffsetBetweenDriverAndController READ homeOffsetBetweenDriverAndController WRITE setHomeOffsetBetweenDriverAndController
                   NOTIFY homeOffsetBetweenDriverAndControllerChanged)
    Q_PROPERTY(int homeSettlingWindow READ homeSettlingWindow WRITE setHomeSettlingWindow NOTIFY homeSettlingWindowChanged)
    Q_PROPERTY(int homeSettlingPricision READ homeSettlingPricision WRITE setHomeSettlingPricision NOTIFY homeSettlingPricisionChanged)
    Q_PROPERTY(int homeSettlingTimeout READ homeSettlingTimeout WRITE setHomeSettlingTimeout NOTIFY homeSettlingTimeoutChanged)

    Q_PROPERTY(double maxCompensation READ maxCompensation WRITE setMaxCompensation NOTIFY maxCompensationChanged)

    Q_PROPERTY(bool settlingByAppLayer READ settlingByAppLayer WRITE setSettlingByAppLayer NOTIFY settlingByAppLayerChanged)
    Q_PROPERTY(int settlingWindow READ settlingWindow WRITE setSettlingWindow NOTIFY settlingWindowChanged)

public:
    enum HomeExecutor
    {
        Controller,
        Driver,
        DriverThenController,
        ClearPos
    };
    Q_ENUM(HomeExecutor)

    AdvancedAxisConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    void setOptionalIOName(const QVariantList &diNames, const QVariantList &doNames)
    {
        setOptionalProperty("startHomeDoName", doNames);
        setOptionalProperty("homeDoneDiName", diNames);
    }

    bool jogMovable() const
    {
        return m_jogMovable;
    }

    HomeExecutor homeExecutor() const
    {
        return m_homeExecutor;
    }

    int resetDriverTime() const
    {
        return m_resetDriverTime;
    }

    QString startHomeDoName() const
    {
        return m_startHomeDoName;
    }

    QString homeDoneDiName() const
    {
        return m_homeDoneDiName;
    }

    double homeOffsetBetweenDriverAndController() const
    {
        return m_homeOffsetBetweenDriverAndController;
    }

    bool settlingByAppLayer() const
    {
        return m_settlingByAppLayer;
    }

    int settlingWindow() const
    {
        return m_settlingWindow;
    }

    bool needHome() const
    {
        return m_needHome;
    }

    bool useSoftLimit() const
    {
        return m_useSoftLimit;
    }

    int homeSettlingWindow() const
    {
        return m_homeSettlingWindow;
    }

    int homeSettlingPricision() const
    {
        return m_homeSettlingPricision;
    }

    int homeSettlingTimeout() const
    {
        return m_homeSettlingTimeout;
    }

    double maxCompensation() const
    {
        return m_maxCompensation;
    }

    double jogMoveMaxVel() const
    {
        return m_jogMoveMaxVel;
    }

    bool respondToGlobalVelRatioChange() const
    {
        return m_respondToGlobalVelRatioChange;
    }

public slots:
    void setJogMovable(bool jogMovable)
    {
        if (m_jogMovable == jogMovable)
            return;

        m_jogMovable = jogMovable;
        emit jogMovableChanged(m_jogMovable);
    }

    void setHomeExecutor(HomeExecutor homeExecutor)
    {
        if (m_homeExecutor == homeExecutor)
            return;

        m_homeExecutor = homeExecutor;
        emit homeExecutorChanged(m_homeExecutor);
    }

    void setResetDriverTime(int resetDriverTime)
    {
        if (m_resetDriverTime == resetDriverTime)
            return;

        m_resetDriverTime = resetDriverTime;
        emit resetDriverTimeChanged(m_resetDriverTime);
    }

    void setStartHomeDoName(QString startHomeDoName)
    {
        if (m_startHomeDoName == startHomeDoName)
            return;

        m_startHomeDoName = startHomeDoName;
        emit startHomeDoNameChanged(m_startHomeDoName);
    }

    void setHomeDoneDiName(QString homeDoneDiName)
    {
        if (m_homeDoneDiName == homeDoneDiName)
            return;

        m_homeDoneDiName = homeDoneDiName;
        emit homeDoneDiNameChanged(m_homeDoneDiName);
    }

    void setHomeOffsetBetweenDriverAndController(double homeOffsetBetweenDriverAndController)
    {
        if (qFuzzyCompare(m_homeOffsetBetweenDriverAndController, homeOffsetBetweenDriverAndController))
            return;

        m_homeOffsetBetweenDriverAndController = homeOffsetBetweenDriverAndController;
        emit homeOffsetBetweenDriverAndControllerChanged(m_homeOffsetBetweenDriverAndController);
    }

    void setSettlingByAppLayer(bool settlingByAppLayer)
    {
        if (m_settlingByAppLayer == settlingByAppLayer)
            return;

        m_settlingByAppLayer = settlingByAppLayer;
        emit settlingByAppLayerChanged(m_settlingByAppLayer);
    }

    void setSettlingWindow(int settlingWindow)
    {
        if (m_settlingWindow == settlingWindow)
            return;

        m_settlingWindow = settlingWindow;
        emit settlingWindowChanged(m_settlingWindow);
    }

    void setNeedHome(bool needHome)
    {
        if (m_needHome == needHome)
            return;

        m_needHome = needHome;
        emit needHomeChanged(m_needHome);
    }

    void setUseSoftLimit(bool useSoftLimit)
    {
        if (m_useSoftLimit == useSoftLimit)
            return;

        m_useSoftLimit = useSoftLimit;
        emit useSoftLimitChanged(m_useSoftLimit);
    }

    void setHomeSettlingWindow(int homeSettlingWindow)
    {
        if (m_homeSettlingWindow == homeSettlingWindow)
            return;

        m_homeSettlingWindow = homeSettlingWindow;
        emit homeSettlingWindowChanged(m_homeSettlingWindow);
    }

    void setHomeSettlingPricision(int homeSettlingPricision)
    {
        if (m_homeSettlingPricision == homeSettlingPricision)
            return;

        m_homeSettlingPricision = homeSettlingPricision;
        emit homeSettlingPricisionChanged(m_homeSettlingPricision);
    }

    void setHomeSettlingTimeout(int homeSettlingTimeout)
    {
        if (m_homeSettlingTimeout == homeSettlingTimeout)
            return;

        m_homeSettlingTimeout = homeSettlingTimeout;
        emit homeSettlingTimeoutChanged(m_homeSettlingTimeout);
    }

    void setMaxCompensation(double maxCompensation)
    {
        if (qFuzzyCompare(m_maxCompensation, maxCompensation))
            return;

        m_maxCompensation = maxCompensation;
        emit maxCompensationChanged(m_maxCompensation);
    }

    void setJogMoveMaxVel(double jogMoveMaxVel)
    {
        if (qFuzzyCompare(m_jogMoveMaxVel, jogMoveMaxVel))
            return;

        m_jogMoveMaxVel = jogMoveMaxVel;
        emit jogMoveMaxVelChanged(m_jogMoveMaxVel);
    }

    void setRespondToGlobalVelRatioChange(bool respondToGlobalVelRatioChange)
    {
        if (m_respondToGlobalVelRatioChange == respondToGlobalVelRatioChange)
            return;

        m_respondToGlobalVelRatioChange = respondToGlobalVelRatioChange;
        emit respondToGlobalVelRatioChangeChanged(m_respondToGlobalVelRatioChange);
    }

signals:
    void jogMovableChanged(bool jogMovable);

    void homeExecutorChanged(HomeExecutor homeExecutor);

    void resetDriverTimeChanged(int resetDriverTime);

    void startHomeDoNameChanged(QString startHomeDoName);

    void homeDoneDiNameChanged(QString homeDoneDiName);

    void homeOffsetBetweenDriverAndControllerChanged(double homeOffsetBetweenDriverAndController);

    void settlingByAppLayerChanged(bool settlingByAppLayer);

    void settlingWindowChanged(int settlingWindow);

    void needHomeChanged(bool needHome);

    void useSoftLimitChanged(bool useSoftLimit);

    void homeSettlingWindowChanged(int homeSettlingWindow);

    void homeSettlingPricisionChanged(int homeSettlingPricision);

    void homeSettlingTimeoutChanged(int homeSettlingTimeout);

    void maxCompensationChanged(double maxCompensation);

    void jogMoveMaxVelChanged(double jogMoveMaxVel);

    void respondToGlobalVelRatioChangeChanged(bool respondToGlobalVelRatioChange);

private:
    HomeExecutor m_homeExecutor{ Controller };
    QString m_startHomeDoName;
    QString m_homeDoneDiName;
    double m_homeOffsetBetweenDriverAndController = 0;
    bool m_jogMovable = true;
    bool m_settlingByAppLayer = false;
    int m_settlingWindow = 5;
    int m_resetDriverTime = -1;
    bool m_needHome = true;
    bool m_useSoftLimit = true;
    int m_homeSettlingWindow = 10;
    int m_homeSettlingPricision = -1;
    int m_homeSettlingTimeout = 3000;
    double m_maxCompensation = -1;
    double m_jogMoveMaxVel = 100;
    bool m_respondToGlobalVelRatioChange = true;
};

class SCMOTIONSHARED_EXPORT AxisConfig : public ConfigObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(AdvancedAxisConfig *advancedAxisConfig READ advancedAxisConfig)
    Q_PROPERTY(double scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(double velocityRatio READ velocityRatio WRITE setVelocityRatio NOTIFY velocityRatioChanged)
    Q_PROPERTY(int homeTimeout READ homeTimeout WRITE setHomeTimeout NOTIFY homeTimeoutChanged)
    Q_PROPERTY(double maxVel READ maxVel WRITE setMaxVel NOTIFY maxVelChanged)
    Q_PROPERTY(double maxAcc READ maxAcc WRITE setMaxAcc NOTIFY maxAccChanged)
    Q_PROPERTY(double negativeLimit READ negativeLimit WRITE setNegativeLimit NOTIFY negativeLimitChanged)
    Q_PROPERTY(double positiveLimit READ positiveLimit WRITE setPositiveLimit NOTIFY positiveLimitChanged)
    Q_PROPERTY(ConfigObjectArray *softLandingPoses READ softLandingPoses)

public:
    Q_INVOKABLE AxisConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_advancedAxisConfig = new AdvancedAxisConfig(this);
        m_softLandingPoses = new ConfigObjectArray(&SoftLandingPos::staticMetaObject, this);
        setReadOnlyProperty("name");
        hide("softLandingPoses");
        init();
    }

    void setOptionalIOName(QVariantList diNames, QVariantList doNames)
    {
        diNames.append("");
        doNames.append("");
        m_advancedAxisConfig->setOptionalIOName(diNames, doNames);
    }

    void createLostPos(SoftlandingDefinition *softlandingDefinition)
    {
        for (int i = 0; i < softlandingDefinition->softLandingPoses()->count(); i++)
        {
            QString posName = softlandingDefinition->softLandingPoses()->at(i).toString();
            if (!softLandingPosMap.contains(posName))
            {
                SoftLandingPos *pos = new SoftLandingPos(m_softLandingPoses);
                pos->setAxisName(softlandingDefinition->axisName());
                pos->setPosName(posName);
                softLandingPosMap[posName] = pos;
                m_softLandingPoses->executeAddConfigObject(0, pos);
            }
        }
    }

    void removeRedundantPos(SoftlandingDefinition *softlandingDefinition)
    {
        QStringList definedSoftlandingPoses = unpackVariantList<QString>(softlandingDefinition->softLandingPoses()->members());
        for (int i = m_softLandingPoses->count() - 1; i >= 0; i--)
        {
            auto softLandingPos = m_softLandingPoses->getConfig<SoftLandingPos>(i);
            if (!definedSoftlandingPoses.contains(softLandingPos->posName()))
            {
                m_softLandingPoses->remove(i);
                if (softLandingPosMap.contains(softLandingPos->posName()))
                {
                    softLandingPosMap.remove(softLandingPos->posName());
                }
            }
        }
    }

    void pos2Dic()
    {
        for (int i = 0; i < m_softLandingPoses->count(); i++)
        {
            auto softLandingPos = m_softLandingPoses->getConfig<SoftLandingPos>(i);
            softLandingPosMap[softLandingPos->posName()] = softLandingPos;
        }
    }

    SoftLandingPos *getPos(QString posName)
    {
        if (softLandingPosMap.contains(posName))
        {
            return softLandingPosMap[posName];
        }
        else
        {
            throw SilicolAbort(tr("Undefined soft landing pos: %1").arg(posName), EX_LOCATION);
        }
    }

    QStringList softLandingPosNames() const
    {
        return softLandingPosMap.keys();
    }

    double velocityRatio() const
    {
        return m_velocityRatio;
    }

    QString name() const
    {
        return m_name;
    }

    int homeTimeout() const
    {
        return m_homeTimeout;
    }

    double maxVel() const
    {
        return m_maxVel;
    }

    double maxAcc() const
    {
        return m_maxAcc;
    }

    double positiveLimit() const
    {
        return m_positiveLimit;
    }

    double negativeLimit() const
    {
        return m_negativeLimit;
    }

    AdvancedAxisConfig *advancedAxisConfig() const
    {
        return m_advancedAxisConfig;
    }

    double scale() const
    {
        return m_scale;
    }

    ConfigObjectArray *softLandingPoses() const
    {
        return m_softLandingPoses;
    }

public slots:
    void setVelocityRatio(double velocityRatio)
    {
        if (qFuzzyCompare(m_velocityRatio, velocityRatio))
            return;
        if (velocityRatio <= 0 || velocityRatio > 1)
        {
            qCritical("Velocity ratio should be in range (0, 1]! Value: %f", velocityRatio);
            return;
        }

        m_velocityRatio = velocityRatio;
        emit velocityRatioChanged(m_velocityRatio);
    }

    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

    void setHomeTimeout(int homeTimeout)
    {
        if (m_homeTimeout == homeTimeout)
            return;

        m_homeTimeout = homeTimeout;
        emit homeTimeoutChanged(m_homeTimeout);
    }

    void setMaxVel(double maxVel)
    {
        if (qFuzzyCompare(m_maxVel, maxVel))
            return;

        m_maxVel = maxVel;
        emit maxVelChanged(m_maxVel);
    }

    void setMaxAcc(double maxAcc)
    {
        if (qFuzzyCompare(m_maxAcc, maxAcc))
            return;

        m_maxAcc = maxAcc;
        emit maxAccChanged(m_maxAcc);
    }

    void setPositiveLimit(double positiveLimit)
    {
        if (qFuzzyCompare(m_positiveLimit, positiveLimit))
            return;

        m_positiveLimit = positiveLimit;
        emit positiveLimitChanged(m_positiveLimit);
    }

    void setNegativeLimit(double negativeLimit)
    {
        if (qFuzzyCompare(m_negativeLimit, negativeLimit))
            return;

        m_negativeLimit = negativeLimit;
        emit negativeLimitChanged(m_negativeLimit);
    }

    void setScale(double scale)
    {
        if (qFuzzyCompare(m_scale, scale))
            return;

        m_scale = scale;
        emit scaleChanged(m_scale);
    }

signals:
    void velocityRatioChanged(double velocityRatio);

    void nameChanged(QString name);

    void homeTimeoutChanged(int homeTimeout);

    void maxVelChanged(double maxVel);

    void maxAccChanged(double maxAcc);

    void positiveLimitChanged(double positiveLimit);

    void negativeLimitChanged(double negativeLimit);

    void scaleChanged(double scale);

private:
    double m_velocityRatio = 0.5;
    QString m_name;
    int m_homeTimeout = 30000;
    double m_maxVel = 1;
    double m_maxAcc = 10;
    double m_positiveLimit = 1;
    double m_negativeLimit = -1;
    AdvancedAxisConfig *m_advancedAxisConfig;
    double m_scale = 1000;
    ConfigObjectArray *m_softLandingPoses;
    QMap<QString, SoftLandingPos *> softLandingPosMap;
};

#endif    // AXISCONFIG_H
