#ifndef DISCHARGECONFIG_H
#define DISCHARGECONFIG_H

#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

class DischargeConfig : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(int JudgeIndex READ JudgeIndex WRITE setJudgeIndex NOTIFY JudgeIndexChanged)


public:

    DischargeConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

public:
    int JudgeIndex() const
    {
        return m_JudgeIndex;
    }

public slots:
    void setJudgeIndex(int JudgeIndex)
    {
        if (m_JudgeIndex == JudgeIndex)
            return;

        m_JudgeIndex = JudgeIndex;
        emit JudgeIndexChanged(m_JudgeIndex);
    }

signals:
    void JudgeIndexChanged(int JudgeIndex);

private:
    int m_JudgeIndex = 2;
};


#endif // DISCHARGECONFIG_H
