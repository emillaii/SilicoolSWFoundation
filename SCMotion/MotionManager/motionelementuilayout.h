#ifndef MOTIONELEMENTUILAYOUT_H
#define MOTIONELEMENTUILAYOUT_H

#include "configManager/configarray.h"
#include "configManager/configobject.h"

class MotionElementUILayout : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString elementType READ elementType WRITE setElementType NOTIFY elementTypeChanged)
    Q_PROPERTY(QString pageName READ pageName WRITE setPageName NOTIFY pageNameChanged)
    Q_PROPERTY(ConfigArray *pageElements READ pageElements WRITE setPageElements NOTIFY pageElementsChanged)

public:
    Q_INVOKABLE MotionElementUILayout(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_pageElements = new ConfigArray(ConfigElementInfo::Other, this);
        disableTranslate();
        init();
    }

    QString elementType() const
    {
        return m_elementType;
    }

    QString pageName() const
    {
        return m_pageName;
    }

    ConfigArray *pageElements() const
    {
        return m_pageElements;
    }

public slots:
    void setElementType(QString elementType)
    {
        if (m_elementType == elementType)
            return;

        m_elementType = elementType;
        emit elementTypeChanged(m_elementType);
    }

    void setPageName(QString pageName)
    {
        if (m_pageName == pageName)
            return;

        m_pageName = pageName;
        emit pageNameChanged(m_pageName);
    }

    void setPageElements(ConfigArray *pageElements)
    {
        if (m_pageElements == pageElements)
            return;

        m_pageElements = pageElements;
        emit pageElementsChanged(m_pageElements);
    }

signals:
    void elementTypeChanged(QString elementType);

    void pageNameChanged(QString pageName);

    void pageElementsChanged(ConfigArray *pageElements);

private:
    QString m_elementType;
    QString m_pageName;
    ConfigArray *m_pageElements;
};

#endif    // MOTIONELEMENTUILAYOUT_H
