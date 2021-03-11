#ifndef ACSAXISCONFIG_H
#define ACSAXISCONFIG_H

#include "BasicElement/axisconfig.h"

class ACSAxisConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int homeBfIndex READ homeBfIndex WRITE setHomeBfIndex NOTIFY homeBfIndexChanged)
    Q_PROPERTY(int initBfIndex READ initBfIndex WRITE setInitBfIndex NOTIFY initBfIndexChanged)
public:

    Q_INVOKABLE ACSAxisConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
       init();
    }

    int index() const
    {
        return m_index;
    }

    int homeBfIndex() const
    {
        return m_homeBfIndex;
    }

    int initBfIndex() const
    {
        return m_initBfIndex;
    }

public slots:

    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

    void setHomeBfIndex(int homeBfIndex)
    {
        if (m_homeBfIndex == homeBfIndex)
            return;

        m_homeBfIndex = homeBfIndex;
        emit homeBfIndexChanged(m_homeBfIndex);
    }

    void setInitBfIndex(int initBfIndex)
    {
        if (m_initBfIndex == initBfIndex)
            return;

        m_initBfIndex = initBfIndex;
        emit initBfIndexChanged(m_initBfIndex);
    }

signals:

    void indexChanged(int index);

    void homeBfIndexChanged(int homeBfIndex);

    void initBfIndexChanged(int initBfIndex);

private:
    int m_index = 0;
    int m_homeBfIndex = 2;
    int m_initBfIndex = 1;
};

#endif    // ACSAXISCONFIG_H
