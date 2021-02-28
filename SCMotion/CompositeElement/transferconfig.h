#ifndef TRANSFERCONFIG_H
#define TRANSFERCONFIG_H

#include "BasicElement/scaxis.h"
#include "configManager/configobject.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT LoadConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(SCAxis::Direction beltRunDirection READ beltRunDirection WRITE setBeltRunDirection NOTIFY beltRunDirectionChanged)
    Q_PROPERTY(int loadTimeout READ loadTimeout WRITE setLoadTimeout NOTIFY loadTimeoutChanged)
    Q_PROPERTY(int delayAfterSNRGotSignal READ delayAfterSNRGotSignal WRITE setDelayAfterSNRGotSignal NOTIFY delayAfterSNRGotSignalChanged)

public:
    LoadConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    SCAxis::Direction beltRunDirection() const
    {
        return m_beltRunDirection;
    }

    int loadTimeout() const
    {
        return m_loadTimeout;
    }

    int delayAfterSNRGotSignal() const
    {
        return m_delayAfterSNRGotSignal;
    }

public slots:
    void setBeltRunDirection(SCAxis::Direction beltRunDirection)
    {
        if (m_beltRunDirection == beltRunDirection)
            return;

        m_beltRunDirection = beltRunDirection;
        emit beltRunDirectionChanged(m_beltRunDirection);
    }

    void setLoadTimeout(int loadTimeout)
    {
        if (m_loadTimeout == loadTimeout)
            return;

        m_loadTimeout = loadTimeout;
        emit loadTimeoutChanged(m_loadTimeout);
    }

    void setDelayAfterSNRGotSignal(int delayAfterSNRGotSignal)
    {
        if (m_delayAfterSNRGotSignal == delayAfterSNRGotSignal)
            return;

        m_delayAfterSNRGotSignal = delayAfterSNRGotSignal;
        emit delayAfterSNRGotSignalChanged(m_delayAfterSNRGotSignal);
    }

signals:
    void beltRunDirectionChanged(SCAxis::Direction beltRunDirection);

    void loadTimeoutChanged(int loadTimeout);

    void delayAfterSNRGotSignalChanged(int delayAfterSNRGotSignal);

private:
    SCAxis::Direction m_beltRunDirection{ SCAxis::Positive };
    int m_loadTimeout = 10000;
    int m_delayAfterSNRGotSignal = -1;
};

class SCMOTIONSHARED_EXPORT UnloadConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(SCAxis::Direction beltRunDirection READ beltRunDirection WRITE setBeltRunDirection NOTIFY beltRunDirectionChanged)
    Q_PROPERTY(int unloadTimeout READ unloadTimeout WRITE setUnloadTimeout NOTIFY unloadTimeoutChanged)
    Q_PROPERTY(int delayAfterSNRLostSignal READ delayAfterSNRLostSignal WRITE setDelayAfterSNRLostSignal NOTIFY delayAfterSNRLostSignalChanged)

public:
    UnloadConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    SCAxis::Direction beltRunDirection() const
    {
        return m_beltRunDirection;
    }

    int unloadTimeout() const
    {
        return m_unloadTimeout;
    }

    int delayAfterSNRLostSignal() const
    {
        return m_delayAfterSNRLostSignal;
    }

public slots:
    void setBeltRunDirection(SCAxis::Direction beltRunDirection)
    {
        if (m_beltRunDirection == beltRunDirection)
            return;

        m_beltRunDirection = beltRunDirection;
        emit beltRunDirectionChanged(m_beltRunDirection);
    }

    void setUnloadTimeout(int unloadTimeout)
    {
        if (m_unloadTimeout == unloadTimeout)
            return;

        m_unloadTimeout = unloadTimeout;
        emit unloadTimeoutChanged(m_unloadTimeout);
    }

    void setDelayAfterSNRLostSignal(int delayAfterSNRLostSignal)
    {
        if (m_delayAfterSNRLostSignal == delayAfterSNRLostSignal)
            return;

        m_delayAfterSNRLostSignal = delayAfterSNRLostSignal;
        emit delayAfterSNRLostSignalChanged(m_delayAfterSNRLostSignal);
    }

signals:
    void beltRunDirectionChanged(SCAxis::Direction beltRunDirection);

    void unloadTimeoutChanged(int unloadTimeout);

    void delayAfterSNRLostSignalChanged(int delayAfterSNRLostSignal);

private:
    SCAxis::Direction m_beltRunDirection{ SCAxis::Positive };
    int m_unloadTimeout = 10000;
    int m_delayAfterSNRLostSignal = -1;
};

#endif    // TRANSFERCONFIG_H
