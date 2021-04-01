#ifndef FEEDINGCONFIG_H
#define FEEDINGCONFIG_H

#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

class FeedingConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(double FeedingStep READ FeedingStep WRITE setFeedingStep NOTIFY FeedingStepChanged)
    Q_PROPERTY(double FeedCarrierBoardStep READ FeedCarrierBoardStep WRITE setFeedCarrierBoardStep NOTIFY FeedCarrierBoardStepChanged)

    Q_PROPERTY(double FeedCarrierBoardSpeed READ FeedCarrierBoardSpeed WRITE setFeedCarrierBoardSpeed NOTIFY FeedCarrierBoardSpeedChanged)
    Q_PROPERTY(double DetaperSpeed READ DetaperSpeed WRITE setDetaperSpeed NOTIFY DetaperSpeedChanged)
    Q_PROPERTY(double OutCarrierBoardSpeed READ OutCarrierBoardSpeed WRITE setOutCarrierBoardSpeed NOTIFY OutCarrierBoardSpeedChanged)

public:
    FeedingConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

public:
    int mMagCloumn;
    int mNowColumnIndex;

    double SearchForMagOffset = 11.0;
    double FeedingStep() const
    {
        return m_FeedingStep;
    }

    double FeedCarrierBoardStep() const
    {
        return m_FeedCarrierBoardStep;
    }

    double FeedCarrierBoardSpeed() const
    {
        return m_FeedCarrierBoardSpeed;
    }

    double DetaperSpeed() const
    {
        return m_DetaperSpeed;
    }

    double OutCarrierBoardSpeed() const
    {
        return m_OutCarrierBoardSpeed;
    }

public slots:
    void setFeedingStep(double FeedingStep)
    {
        if (qFuzzyCompare(m_FeedingStep, FeedingStep))
            return;

        m_FeedingStep = FeedingStep;
        emit FeedingStepChanged(m_FeedingStep);
    }

    void setFeedCarrierBoardStep(double FeedCarrierBoardStep)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_FeedCarrierBoardStep, FeedCarrierBoardStep))
            return;

        m_FeedCarrierBoardStep = FeedCarrierBoardStep;
        emit FeedCarrierBoardStepChanged(m_FeedCarrierBoardStep);
    }

    void setFeedCarrierBoardSpeed(double FeedCarrierBoardSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_FeedCarrierBoardSpeed, FeedCarrierBoardSpeed))
            return;

        m_FeedCarrierBoardSpeed = FeedCarrierBoardSpeed;
        emit FeedCarrierBoardSpeedChanged(m_FeedCarrierBoardSpeed);
    }

    void setDetaperSpeed(double DetaperSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_DetaperSpeed, DetaperSpeed))
            return;

        m_DetaperSpeed = DetaperSpeed;
        emit DetaperSpeedChanged(m_DetaperSpeed);
    }

    void setOutCarrierBoardSpeed(double OutCarrierBoardSpeed)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_OutCarrierBoardSpeed, OutCarrierBoardSpeed))
            return;

        m_OutCarrierBoardSpeed = OutCarrierBoardSpeed;
        emit OutCarrierBoardSpeedChanged(m_OutCarrierBoardSpeed);
    }

signals:
    void FeedingStepChanged(double FeedingStep);

    void FeedCarrierBoardStepChanged(double FeedCarrierBoardStep);

    void FeedCarrierBoardSpeedChanged(double FeedCarrierBoardSpeed);

    void DetaperSpeedChanged(double DetaperSpeed);

    void OutCarrierBoardSpeedChanged(double OutCarrierBoardSpeed);

private:
    double m_FeedingStep = 0;
    double m_FeedCarrierBoardStep = 0;
    double m_FeedCarrierBoardSpeed = 0;
    double m_DetaperSpeed = 0;
    double m_OutCarrierBoardSpeed = 0;
};

#endif // FEEDINGCONFIG_H
