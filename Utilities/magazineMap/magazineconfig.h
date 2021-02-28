#ifndef MAGAZINECONFIG_H
#define MAGAZINECONFIG_H

#include "configManager/configobject.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT MagazineConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int trayCount READ trayCount WRITE setTrayCount NOTIFY trayCountChanged)

public:
    MagazineConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    int trayCount() const
    {
        return m_trayCount;
    }

public slots:
    void setTrayCount(int trayCount)
    {
        if (m_trayCount == trayCount)
            return;

        if (trayCount < 1)
        {
            qCritical() << tr("trayCount must greater than 0!");
            emit trayCountChanged(m_trayCount);
            return;
        }

        m_trayCount = trayCount;
        emit trayCountChanged(m_trayCount);
    }

signals:
    void trayCountChanged(int trayCount);

private:
    int m_trayCount = 1;
};

#endif    // MAGAZINECONFIG_H
