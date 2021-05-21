#ifndef RENAMEMANAGERCONFIG_H
#define RENAMEMANAGERCONFIG_H

#include "configManager/configobject.h"

class RenameMgrConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)

public:
    RenameMgrConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    bool enable() const
    {
        return m_enable;
    }

public slots:
    void setEnable(bool enable)
    {
        if (m_enable == enable)
            return;

        m_enable = enable;
        emit enableChanged(m_enable);
    }

signals:
    void enableChanged(bool enable);

private:
    bool m_enable = false;
};

#endif    // RENAMEMANAGERCONFIG_H
