#ifndef UPHHELPER_H
#define UPHHELPER_H

#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "utilities_global.h"
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

class UPHHelperConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int timeWindowLen READ timeWindowLen WRITE setTimeWindowLen NOTIFY timeWindowLenChanged)
    Q_PROPERTY(int calculateInterval READ calculateInterval WRITE setCalculateInterval NOTIFY calculateIntervalChanged)

public:
    UPHHelperConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    int timeWindowLen() const
    {
        return m_timeWindowLen;
    }

    int calculateInterval() const
    {
        return m_calculateInterval;
    }

public slots:
    void setTimeWindowLen(int timeWindowLen)
    {
        if (m_timeWindowLen == timeWindowLen)
            return;

        m_timeWindowLen = timeWindowLen;
        emit timeWindowLenChanged(m_timeWindowLen);
    }

    void setCalculateInterval(int calculateInterval)
    {
        if (m_calculateInterval == calculateInterval)
            return;

        m_calculateInterval = calculateInterval;
        emit calculateIntervalChanged(m_calculateInterval);
    }

signals:
    void timeWindowLenChanged(int timeWindowLen);

    void calculateIntervalChanged(int calculateInterval);

private:
    int m_timeWindowLen = 600;
    int m_calculateInterval = 1;
};

class DataStash : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged)

public:
    DataStash(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    int total() const
    {
        return m_total;
    }

public slots:
    void setTotal(int total)
    {
        if (m_total == total)
            return;

        m_total = total;
        emit totalChanged(m_total);
    }

signals:
    void totalChanged(int total);

private:
    int m_total = 0;
};

class UTILITIESSHARED_EXPORT UPHHelper : public QThread
{
    Q_OBJECT

    Q_PROPERTY(int uph READ uph WRITE setUph NOTIFY uphChanged)
    Q_PROPERTY(double lastCircelTime READ lastCircelTime WRITE setLastCircelTime NOTIFY lastCircelTimeChanged)
    Q_PROPERTY(double trayTestTime READ trayTestTime WRITE setTrayTestTime NOTIFY trayTestTimeChanged)
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged)

public:
    explicit UPHHelper(QObject *parent = nullptr);

    ~UPHHelper() override;

    void appendData();
    void stop();

    void trayTestStart();
    void trayTestEnd();

    Q_INVOKABLE void reset();

    int uph() const
    {
        return m_uph;
    }

    double lastCircelTime() const
    {
        return m_lastCircelTime;
    }

    int total() const
    {
        return m_total;
    }

    double trayTestTime() const
    {
        return m_trayTestTime;
    }

public slots:
    void setUph(int uph)
    {
        if (m_uph == uph)
            return;

        m_uph = uph;
        emit uphChanged(m_uph);
    }

    void setLastCircelTime(double lastCircelTime)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_lastCircelTime, lastCircelTime))
            return;

        m_lastCircelTime = lastCircelTime;
        emit lastCircelTimeChanged(m_lastCircelTime);
    }

    void setTotal(int total)
    {
        if (m_total == total)
            return;

        m_total = total;
        emit totalChanged(m_total);
    }

    void setTrayTestTime(double trayTestTime)
    {
        if (qFuzzyCompare(m_trayTestTime, trayTestTime))
            return;

        m_trayTestTime = trayTestTime;
        emit trayTestTimeChanged(m_trayTestTime);
    }

signals:
    void uphChanged(int uph);

    void lastCircelTimeChanged(double lastCircelTime);

    void totalChanged(int total);

    void trayTestTimeChanged(double trayTestTime);

protected:
    virtual void run() override;

private:
    const long long sleepStep = 50;

    bool isRun = true;
    QMutex locker;
    QList<QDateTime> timestamps;
    QDateTime trayStartTimeStamp;
    UPHHelperConfig *uphHelperConfig;
    ConfigFile *uphHelperConfigFile;
    DataStash *dataStash;
    ConfigFile *dataStashFile;
    int m_uph = 0;
    double m_lastCircelTime = -1;
    int m_total = 0;
    double m_trayTestTime = -1;
};

#endif    // UPHHELPER_H
