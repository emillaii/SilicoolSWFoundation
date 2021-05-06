#ifndef ELMODRIVERCONFIG_H
#define ELMODRIVERCONFIG_H

#include "BasicElement/axisconfig.h"
#include "BasicElement/softlandingpos.h"
#include "configManager/configobject.h"

class ElmoDriverConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(int posSensorSocketNum READ posSensorSocketNum WRITE setPosSensorSocketNum NOTIFY posSensorSocketNumChanged)
    Q_PROPERTY(QString targetIp READ targetIp WRITE setTargetIp NOTIFY targetIpChanged)
    Q_PROPERTY(QString hostIp READ hostIp WRITE setHostIp NOTIFY hostIpChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(double homeAcc READ homeAcc WRITE setHomeAcc NOTIFY homeAccChanged)
    Q_PROPERTY(double homeVel1 READ homeVel1 WRITE setHomeVel1 NOTIFY homeVel1Changed)
    Q_PROPERTY(double homeVel2 READ homeVel2 WRITE setHomeVel2 NOTIFY homeVel2Changed)
    Q_PROPERTY(double homeMaxCurRatio READ homeMaxCurRatio WRITE setHomeMaxCurRatio NOTIFY homeMaxCurRatioChanged)
    Q_PROPERTY(double softlandingAcc READ softlandingAcc WRITE setSoftlandingAcc NOTIFY softlandingAccChanged)
    Q_PROPERTY(double maxCurrent READ maxCurrent WRITE setMaxCurrent NOTIFY maxCurrentChanged)

public:
    Q_INVOKABLE ElmoDriverConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
        QVariantList qv;
        qv << 1 << 2 << 3 << 4;
        setOptionalProperty("posSensorSocketNum", qv);
        QStringList args;
        args << "posSensorSocketNum"
             << "targetIp"
             << "port"
             << "homeAcc"
             << "homeVel1"
             << "homeVel2"
             << "homeMaxCurRatio"
             << "softlandingAcc"
             << "maxCurrent";
        setEngineerAuthorities(args);
        init();
    }

    int posSensorSocketNum() const
    {
        return m_posSensorSocketNum;
    }

    QString targetIp() const
    {
        return m_targetIp;
    }

    QString hostIp() const
    {
        return m_hostIp;
    }

    int port() const
    {
        return m_port;
    }

    double homeAcc() const
    {
        return m_homeAcc;
    }

    double homeVel1() const
    {
        return m_homeVel1;
    }

    double homeVel2() const
    {
        return m_homeVel2;
    }

    double homeMaxCurRatio() const
    {
        return m_homeMaxCurRatio;
    }

    double softlandingAcc() const
    {
        return m_softlandingAcc;
    }

    double maxCurrent() const
    {
        return m_maxCurrent;
    }

public slots:

    void setPosSensorSocketNum(int posSensorSocketNum)
    {
        if (m_posSensorSocketNum == posSensorSocketNum)
            return;

        m_posSensorSocketNum = posSensorSocketNum;
        emit posSensorSocketNumChanged(m_posSensorSocketNum);
    }

    void setTargetIp(QString targetIp)
    {
        if (m_targetIp == targetIp)
            return;

        m_targetIp = targetIp;
        emit targetIpChanged(m_targetIp);
    }

    void setHostIp(QString hostIp)
    {
        if (m_hostIp == hostIp)
            return;

        m_hostIp = hostIp;
        emit hostIpChanged(m_hostIp);
    }

    void setPort(int port)
    {
        if (m_port == port)
            return;

        m_port = port;
        emit portChanged(m_port);
    }

    void setHomeAcc(double homeAcc)
    {
        if (qFuzzyCompare(m_homeAcc, homeAcc))
            return;

        m_homeAcc = homeAcc;
        emit homeAccChanged(m_homeAcc);
    }

    void setHomeVel1(double homeVel1)
    {
        if (qFuzzyCompare(m_homeVel1, homeVel1))
            return;

        m_homeVel1 = homeVel1;
        emit homeVel1Changed(m_homeVel1);
    }

    void setHomeVel2(double homeVel2)
    {
        if (qFuzzyCompare(m_homeVel2, homeVel2))
            return;

        m_homeVel2 = homeVel2;
        emit homeVel2Changed(m_homeVel2);
    }

    void setHomeMaxCurRatio(double homeMaxCurRatio)
    {
        if (qFuzzyCompare(m_homeMaxCurRatio, homeMaxCurRatio))
            return;

        m_homeMaxCurRatio = homeMaxCurRatio;
        emit homeMaxCurRatioChanged(m_homeMaxCurRatio);
    }

    void setSoftlandingAcc(double softlandingAcc)
    {
        if (qFuzzyCompare(m_softlandingAcc, softlandingAcc))
            return;

        m_softlandingAcc = softlandingAcc;
        emit softlandingAccChanged(m_softlandingAcc);
    }

    void setMaxCurrent(double maxCurrent)
    {
        if (qFuzzyCompare(m_maxCurrent, maxCurrent))
            return;

        m_maxCurrent = maxCurrent;
        emit maxCurrentChanged(m_maxCurrent);
    }

signals:
    void posSensorSocketNumChanged(int posSensorSocketNum);

    void targetIpChanged(QString targetIp);

    void hostIpChanged(QString hostIp);

    void portChanged(int port);

    void homeAccChanged(double homeAcc);

    void homeVel1Changed(double homeVel1);

    void homeVel2Changed(double homeVel2);

    void homeMaxCurRatioChanged(double homeMaxCurRatio);

    void softlandingAccChanged(double softlandingAcc);

    void maxCurrentChanged(double maxCurrent);

private:
    int m_posSensorSocketNum = 1;
    QString m_targetIp = "192.168.1.";
    QString m_hostIp = "192.168.1.";
    int m_port = 5001;
    double m_homeAcc = 20;
    double m_homeVel1 = 5;
    double m_homeVel2 = -1;
    double m_homeMaxCurRatio = 0.5;
    double m_softlandingAcc = 50;
    double m_maxCurrent = 1;
};

#endif    // ELMODRIVERCONFIG_H
