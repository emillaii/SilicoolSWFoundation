#ifndef ACSAXISCONFIG_H
#define ACSAXISCONFIG_H

#include "BasicElement/axisconfig.h"

class ACSAxisConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(int channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(int rate READ rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
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

    int index() const
    {
        return m_index;
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

    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

signals:
    void channelChanged(int channel);

    void rateChanged(int rate);

    void indexChanged(int index);

private:
    int m_channel = 0;
    int m_rate = 0;
    int m_index = 0;
};

#endif    // ACSAXISCONFIG_H
