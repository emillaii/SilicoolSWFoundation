#ifndef GTIOCONFIG_H
#define GTIOCONFIG_H

#include "BasicElement/ioconfig.h"

class GTIOConfig : public IOConfig
{
    Q_OBJECT

public:
    enum IOType
    {
        AxisIO,
        EthercatIO
    };
    Q_ENUM(IOType)

    Q_PROPERTY(int coreNo READ coreNo WRITE setCoreNo NOTIFY coreNoChanged)
    Q_PROPERTY(IOType ioType READ ioType WRITE setIoType NOTIFY ioTypeChanged)
    Q_PROPERTY(int axis READ axis WRITE setAxis NOTIFY axisChanged)
    Q_PROPERTY(int slave READ slave WRITE setSlave NOTIFY slaveChanged)
    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)

public:
    Q_INVOKABLE GTIOConfig(QObject *parent = nullptr) : IOConfig(parent)
    {
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

    IOType ioType() const
    {
        return m_ioType;
    }

    int axis() const
    {
        return m_axis;
    }

    int slave() const
    {
        return m_slave;
    }

    int offset() const
    {
        return m_offset;
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

    void setIoType(IOType ioType)
    {
        if (m_ioType == ioType)
            return;

        m_ioType = ioType;
        emit ioTypeChanged(m_ioType);
    }

    void setAxis(int axis)
    {
        if (m_axis == axis)
            return;

        m_axis = axis;
        emit axisChanged(m_axis);
    }

    void setSlave(int slave)
    {
        if (m_slave == slave)
            return;

        m_slave = slave;
        emit slaveChanged(m_slave);
    }

    void setOffset(int offset)
    {
        if (m_offset == offset)
            return;

        m_offset = offset;
        emit offsetChanged(m_offset);
    }

signals:
    void coreNoChanged(int coreNo);
    void indexChanged(int index);
    void ioTypeChanged(IOType ioType);
    void axisChanged(int axis);
    void slaveChanged(int slave);
    void offsetChanged(int offset);

private:
    int m_coreNo = 1;
    int m_index = 0;
    IOType m_ioType{ AxisIO };
    int m_axis = 0;
    int m_slave = 0;
    int m_offset = 0;
};

#endif    // GTIOCONFIG_H
