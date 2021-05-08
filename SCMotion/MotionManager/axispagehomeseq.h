#ifndef AXISPAGEHOMESEQ_H
#define AXISPAGEHOMESEQ_H

#include "configManager/configarray.h"
#include "configManager/configobject.h"

class AxisPageHomeSeq : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString pageName READ pageName WRITE setPageName NOTIFY pageNameChanged)
    Q_PROPERTY(ConfigArray *pageElements READ pageElements)
    Q_PROPERTY(QString homeSeqExample READ homeSeqExample WRITE setHomeSeqExample NOTIFY homeSeqExampleChanged)
    Q_PROPERTY(ConfigArray *homeSeq READ homeSeq)
    Q_PROPERTY(bool isValid READ isValid WRITE setIsValid NOTIFY isValidChanged)

public:
    Q_INVOKABLE AxisPageHomeSeq(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_pageElements = new ConfigArray(ConfigElementInfo::Other, this);
        m_homeSeq = new ConfigArray(ConfigElementInfo::Other, this);
        init();
    }

    QString pageName() const
    {
        return m_pageName;
    }

    ConfigArray *pageElements() const
    {
        return m_pageElements;
    }

    QString homeSeqExample() const
    {
        return m_homeSeqExample;
    }

    ConfigArray *homeSeq() const
    {
        return m_homeSeq;
    }

    bool isValid() const
    {
        return m_isValid;
    }

public slots:
    void setPageName(QString pageName)
    {
        if (m_pageName == pageName)
            return;

        m_pageName = pageName;
        emit pageNameChanged(m_pageName);
    }

    void setHomeSeqExample(QString homeSeqExample)
    {
        if (m_homeSeqExample == homeSeqExample)
            return;

        m_homeSeqExample = homeSeqExample;
        emit homeSeqExampleChanged(m_homeSeqExample);
    }

    void setIsValid(bool isValid)
    {
        if (m_isValid == isValid)
            return;

        m_isValid = isValid;
        emit isValidChanged(m_isValid);
    }

signals:
    void pageNameChanged(QString pageName);

    void homeSeqExampleChanged(QString homeSeqExample);

    void isValidChanged(bool isValid);

private:
    QString m_pageName;
    ConfigArray *m_pageElements;
    QString m_homeSeqExample = "[\"axis1,axis2,axis3\",\"axis4,axis5,axis6\",\"axis7,axis8,axis9\"]";
    ConfigArray *m_homeSeq;
    bool m_isValid = false;
};

#endif    // AXISPAGEHOMESEQ_H
