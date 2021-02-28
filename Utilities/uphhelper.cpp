#include "uphhelper.h"

UPHHelper::UPHHelper(QObject *parent) : QThread(parent)
{
    uphHelperConfig = new UPHHelperConfig(this);
    uphHelperConfigFile = new ConfigFile("uphHelperConfig", uphHelperConfig, "./config/uphHelperConfig.json", false);
    uphHelperConfigFile->populate(true);

    dataStash = new DataStash(this);
    dataStashFile = new ConfigFile("DataStash", dataStash, "./config/dataStash.json", false);
    dataStashFile->populate(true);
    setTotal(dataStash->total());
    connect(this, &UPHHelper::totalChanged, dataStash, &DataStash::setTotal);

    if (uphHelperConfig->calculateInterval() < 1 || uphHelperConfig->calculateInterval() > 10)
    {
        qWarning() << tr("UPH calculateInterval less than 1s or greater than 10s, now set it to 1s!");
        uphHelperConfig->setCalculateInterval(1);
    }
    if (uphHelperConfig->timeWindowLen() < 30 || uphHelperConfig->timeWindowLen() > 3600)
    {
        qWarning() << tr("UPH timeWindowLen less than 30s or greater than 3600s, now set it to 600s!");
        uphHelperConfig->setTimeWindowLen(600);
    }
    start();
}

UPHHelper::~UPHHelper()
{
    isRun = false;
    wait();
}

void UPHHelper::appendData()
{
    QDateTime now = QDateTime::currentDateTime();
    QMutexLocker tmpLocker(&locker);
    if (timestamps.count() > 0)
    {
        setLastCircelTime((now.toMSecsSinceEpoch() - timestamps.last().toMSecsSinceEpoch()) / 1000.0);
    }
    timestamps.append(now);
    setTotal(m_total + 1);
}

void UPHHelper::trayTestStart()
{
    trayStartTimeStamp = QDateTime::currentDateTime();
}

void UPHHelper::trayTestEnd()
{
    QDateTime now = QDateTime::currentDateTime();
    setTrayTestTime((now.toMSecsSinceEpoch() - trayStartTimeStamp.toMSecsSinceEpoch()) / 1000.0);
}

void UPHHelper::reset()
{
    {
        QMutexLocker tmpLocker(&locker);
        timestamps.clear();
    }
    setUph(0);
    setLastCircelTime(-1);
    setTrayTestTime(-1);
    setTotal(0);
}

void UPHHelper::run()
{
    while (isRun)
    {
        QDateTime now = QDateTime::currentDateTime();
        {
            QMutexLocker tmpLocker(&locker);
            while (timestamps.count() > 0
                   && now.toSecsSinceEpoch() - timestamps.first().toSecsSinceEpoch() > uphHelperConfig->timeWindowLen())
            {
                timestamps.removeFirst();
            }
            if (timestamps.isEmpty())
            {
                setUph(0);
            }
            else
            {
                double tUph
                    = timestamps.count() * 3600.0 / (now.toSecsSinceEpoch() - timestamps.first().toSecsSinceEpoch());
                setUph(static_cast<int>(tUph));
            }
        }
        int interval = uphHelperConfig->calculateInterval() * 1000;
        QElapsedTimer timer;
        timer.start();
        while (isRun && timer.elapsed() < interval)
        {
            QThread::msleep(qMin(sleepStep, interval - timer.elapsed()));
        }
    }
}
