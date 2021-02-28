#ifndef PACVCMCONFIG_H
#define PACVCMCONFIG_H

#include "BasicElement/axisconfig.h"

class PACVcmConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(int vcmAddress READ vcmAddress WRITE setVcmAddress NOTIFY vcmAddressChanged)
    Q_PROPERTY(int findHardLimitDir READ findHardLimitDir WRITE setFindHardLimitDir NOTIFY findHardLimitDirChanged)
    Q_PROPERTY(int findHardLimitCurrent READ findHardLimitCurrent WRITE setFindHardLimitCurrent NOTIFY findHardLimitCurrentChanged)
    Q_PROPERTY(int findIndexDir READ findIndexDir WRITE setFindIndexDir NOTIFY findIndexDirChanged)
    Q_PROPERTY(int homeVel READ homeVel WRITE setHomeVel NOTIFY homeVelChanged)

public:
    Q_INVOKABLE PACVcmConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
        QVariantList dirs;
        dirs << 0 << 1;
        setOptionalProperty("findHardLimitDir", dirs);
        setOptionalProperty("findIndexDir", dirs);
        init();
    }

    int vcmAddress() const
    {
        return m_vcmAddress;
    }

    int findHardLimitDir() const
    {
        return m_findHardLimitDir;
    }

    int findHardLimitCurrent() const
    {
        return m_findHardLimitCurrent;
    }

    int findIndexDir() const
    {
        return m_findIndexDir;
    }

    int homeVel() const
    {
        return m_homeVel;
    }

public slots:
    void setVcmAddress(int vcmAddress)
    {
        if (m_vcmAddress == vcmAddress)
            return;

        m_vcmAddress = vcmAddress;
        emit vcmAddressChanged(m_vcmAddress);
    }

    void setFindHardLimitDir(int findHardLimitDir)
    {
        if (m_findHardLimitDir == findHardLimitDir)
            return;

        m_findHardLimitDir = findHardLimitDir;
        emit findHardLimitDirChanged(m_findHardLimitDir);
    }

    void setFindHardLimitCurrent(int findHardLimitCurrent)
    {
        if (m_findHardLimitCurrent == findHardLimitCurrent)
            return;

        m_findHardLimitCurrent = findHardLimitCurrent;
        emit findHardLimitCurrentChanged(m_findHardLimitCurrent);
    }

    void setFindIndexDir(int findIndexDir)
    {
        if (m_findIndexDir == findIndexDir)
            return;

        m_findIndexDir = findIndexDir;
        emit findIndexDirChanged(m_findIndexDir);
    }

    void setHomeVel(int homeVel)
    {
        if (m_homeVel == homeVel)
            return;

        m_homeVel = homeVel;
        emit homeVelChanged(m_homeVel);
    }

signals:
    void vcmAddressChanged(int vcmAddress);

    void maxVelChanged(int maxVel);

    void maxAccChanged(int maxAcc);

    void findHardLimitDirChanged(int findHardLimitDir);

    void findHardLimitCurrentChanged(int findHardLimitCurrent);

    void findIndexDirChanged(int findIndexDir);

    void homeVelChanged(int homeVel);

private:
    int m_vcmAddress = 1;
    int m_findHardLimitDir = 0;
    int m_findHardLimitCurrent = 800;
    int m_findIndexDir = 1;
    int m_homeVel = 10;
};

#endif    // PACVCMCONFIG_H
