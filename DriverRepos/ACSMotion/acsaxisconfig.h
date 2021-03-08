#ifndef ACSAXISCONFIG_H
#define ACSAXISCONFIG_H

#include "BasicElement/axisconfig.h"

class ACSAxisConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(int channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(int rate READ rate WRITE setRate NOTIFY rateChanged)

public:

    Q_INVOKABLE ACSAxisConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
    }

    int channel() const
    {
        return m_channel;
    }

    int rate() const
    {
        return m_rate;
    }

public slots:
    void setChannel(int channel)
    {
        if (m_channel == channel)
            return;

        m_channel = channel;
        emit channelChanged(m_channel);
    }

    void setRate(int rate)
    {
        if (m_rate == rate)
            return;

        m_rate = rate;
        emit rateChanged(m_rate);
    }

signals:
    void channelChanged(int channel);

    void rateChanged(int rate);

private:
    int m_channel;
    int m_rate;
};

#endif    // ACSAXISCONFIG_H
