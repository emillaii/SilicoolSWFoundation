#ifndef TRAYDATA_H
#define TRAYDATA_H

#include "configManager/configobject.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT MagazineMap;

class UTILITIESSHARED_EXPORT TrayData : public ConfigObject
{
    Q_OBJECT

public:
    TrayData(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
    }

    virtual void reset(int trayState) = 0;
    virtual QString getTrayStateStr() = 0;
    virtual void setTrayStateStr(QString targetState) = 0;

    int index() const
    {
        return m_index;
    }

    void setIndex(int index)
    {
        m_index = index;
    }

signals:
    void stateChanged(int index);

private:
    friend class MagazineMap;

    int m_index = -1;
    bool hasStateChangedSignal = false;
};

#endif    // TRAYDATA_H
