#ifndef BUZZER_H
#define BUZZER_H

#include "../configManager/configobject.h"
#include "enumhelper.h"

class MachineStateIndicator;

class UTILITIESSHARED_EXPORT BlinkConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(double openedtime READ openedtime WRITE setOpenedtime NOTIFY openedtimeChanged)
    Q_PROPERTY(double closedtime READ closedtime WRITE setClosedtime NOTIFY closedtimeChanged)

public:
    BlinkConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    double openedtime() const
    {
        return m_openedtime;
    }

    double closedtime() const
    {
        return m_closedtime;
    }

public slots:
    void setOpenedtime(double openedtime)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_openedtime, openedtime))
            return;

        m_openedtime = openedtime;
        emit openedtimeChanged(m_openedtime);
    }

    void setClosedtime(double closedtime)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_closedtime, closedtime))
            return;

        m_closedtime = closedtime;
        emit closedtimeChanged(m_closedtime);
    }

signals:
    void openedtimeChanged(double openedtime);

    void closedtimeChanged(double closedtime);

private:
    double m_openedtime = 500;
    double m_closedtime = 500;
};

class UTILITIESSHARED_EXPORT Buzzer : public QObject
{
    Q_OBJECT
public:
    enum BuzzerState
    {
        Closed,
        Blinking,
        Opened
    };
    Q_ENUM(BuzzerState)

    Buzzer(QObject *parent = nullptr) : QObject(parent) {}

    virtual void set(BuzzerState state) = 0;

    void setBlinkConfig(BlinkConfig *blinkConfig)
    {
        this->blinkConfig = blinkConfig;
    }

protected:
    BlinkConfig *blinkConfig = nullptr;
};

#endif    // BUZZER_H
