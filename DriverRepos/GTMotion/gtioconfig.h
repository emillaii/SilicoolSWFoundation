#ifndef GTIOCONFIG_H
#define GTIOCONFIG_H

#include "BasicElement/ioconfig.h"

class GTIOConfig : public IOConfig
{
    Q_OBJECT

public:
    Q_PROPERTY(int coreNo READ coreNo WRITE setCoreNo NOTIFY coreNoChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)

    Q_INVOKABLE GTIOConfig(QObject *parent = nullptr) : IOConfig(parent)
    {
        QStringList args;
        args << "coreNo"
             << "index";
        setEngineerAuthorities(args);
        init();
    }
    int coreNo() const
    {
        return m_coreNo;
    }
    int index() const
    {
        return m_index;
    }

public slots:
    void setCoreNo(int coreNo)
    {
        if (m_coreNo == coreNo)
            return;

        m_coreNo = coreNo;
        emit coreNoChanged(m_coreNo);
    }
    void setIndex(int index)
    {
        if (m_index == index)
            return;

        m_index = index;
        emit indexChanged(m_index);
    }

signals:
    void coreNoChanged(int coreNo);
    void indexChanged(int index);

private:
    int m_coreNo = 1;
    int m_index = 0;
};

#endif    // GTIOCONFIG_H
