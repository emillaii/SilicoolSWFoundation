#ifndef MOTIONMANAGERCONFIG_H
#define MOTIONMANAGERCONFIG_H

#include "configManager/configobject.h"

class MotionManagerConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(
        QString localMotionManagerAddress READ localMotionManagerAddress WRITE setLocalMotionManagerAddress NOTIFY localMotionManagerAddressChanged)

public:
    MotionManagerConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    QString localMotionManagerAddress() const
    {
        return m_localMotionManagerAddress;
    }

public slots:

    void setLocalMotionManagerAddress(QString localMotionManagerAddress)
    {
        if (m_localMotionManagerAddress == localMotionManagerAddress)
            return;

        m_localMotionManagerAddress = localMotionManagerAddress;
        emit localMotionManagerAddressChanged(m_localMotionManagerAddress);
    }

signals:

    void localMotionManagerAddressChanged(QString localMotionManagerAddress);

private:
    QString m_localMotionManagerAddress;
};

#endif    // MOTIONMANAGERCONFIG_H
