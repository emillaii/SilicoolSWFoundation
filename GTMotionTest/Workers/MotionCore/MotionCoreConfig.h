#ifndef TRAYLOADERCONFIG_H
#define TRAYLOADERCONFIG_H

#include "configManager/configobject.h"

class MotionCoreConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int nChangeTrayAfterContinuallyPrFailed READ nChangeTrayAfterContinuallyPrFailed WRITE
                   setNChangeTrayAfterContinuallyPrFailed NOTIFY nChangeTrayAfterContinuallyPrFailedChanged)

public:
    MotionCoreConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    int nChangeTrayAfterContinuallyPrFailed() const
    {
        return m_nChangeTrayAfterContinuallyPrFailed;
    }

public slots:
    void setNChangeTrayAfterContinuallyPrFailed(int nChangeTrayAfterContinuallyPrFailed)
    {
        if (m_nChangeTrayAfterContinuallyPrFailed == nChangeTrayAfterContinuallyPrFailed)
            return;

        m_nChangeTrayAfterContinuallyPrFailed = nChangeTrayAfterContinuallyPrFailed;
        emit nChangeTrayAfterContinuallyPrFailedChanged(m_nChangeTrayAfterContinuallyPrFailed);
    }

signals:
    void nChangeTrayAfterContinuallyPrFailedChanged(int nChangeTrayAfterContinuallyPrFailed);

private:
    int m_nChangeTrayAfterContinuallyPrFailed = 5;
};

#endif    // TRAYLOADERCONFIG_H
