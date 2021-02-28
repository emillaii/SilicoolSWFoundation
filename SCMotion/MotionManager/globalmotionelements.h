#ifndef GLOBALMOTIONELEMENTS_H
#define GLOBALMOTIONELEMENTS_H

#include <QObject>

class GlobalMotionElements
{
private:
    GlobalMotionElements() {}

public:
    static GlobalMotionElements &getIns()
    {
        static GlobalMotionElements instance;
        return instance;
    }

    QVariantList axisNames() const
    {
        return m_axisNames;
    }
    void setAxisNames(const QVariantList &axisNames)
    {
        m_axisNames = axisNames;
    }

    QVariantList cylNames() const
    {
        return m_cylNames;
    }
    void setCylNames(const QVariantList &cylNames)
    {
        m_cylNames = cylNames;
    }

    QVariantList diNames() const
    {
        return m_diNames;
    }
    void setDiNames(const QVariantList &diNames)
    {
        m_diNames = diNames;
    }

private:
    QVariantList m_axisNames;
    QVariantList m_cylNames;
    QVariantList m_diNames;
};

#endif    // GLOBALMOTIONELEMENTS_H
