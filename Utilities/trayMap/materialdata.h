#ifndef MATERIALDATA_H
#define MATERIALDATA_H

#include "configManager/configobject.h"
#include "utilities_global.h"

class TrayMap;

class UTILITIESSHARED_EXPORT MaterialData : public ConfigObject
{
    Q_OBJECT

public:
    MaterialData(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
    }

    virtual void reset(int materialState) = 0;
    virtual QString getMaterialStateStr() = 0;
    virtual void setMaterialStateStr(QString targetState) = 0;

    int rowIndex() const
    {
        return m_rowIndex;
    }
    int columnIndex() const
    {
        return m_columnIndex;
    }

signals:
    void stateChanged(int rowIndex, int columnIndex);

private:
    void setIndex(int rowIndex, int columnIndex)
    {
        m_rowIndex = rowIndex;
        m_columnIndex = columnIndex;
    }

private:
    friend class TrayMap;

    int m_rowIndex = -1;
    int m_columnIndex = -1;
    bool hasStateChangedSignal = false;
};

#endif    // MATERIALDATA_H
