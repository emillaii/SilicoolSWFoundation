#ifndef ACSDO_H
#define ACSDO_H

#include <QObject>
#include "BasicElement/scdo.h"
#include "ACSC.h"
#include "acscardconfig.h"
#include "acserrorhandle.h"

class ACSDOConfig: public IOConfig
{
    Q_OBJECT

    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int bit READ bit WRITE setBit NOTIFY bitChanged)

public:

    int port() const
    {
        return m_port;
    }

    int bit() const
    {
        return m_bit;
    }

public slots:
    void setPort(int port)
    {
        if (m_port == port)
            return;

        m_port = port;
        emit portChanged(m_port);
    }

    void setBit(int bit)
    {
        if (m_bit == bit)
            return;

        m_bit = bit;
        emit bitChanged(m_bit);
    }

signals:
    void portChanged(int port);

    void bitChanged(int bit);

private:
    int m_port = 0;
    int m_bit = 0;
};

class ACSDO : public SCDO
{
    Q_OBJECT
public:
    explicit ACSDO(ACSDOConfig* cfg, QString name, QObject *parent = nullptr):SCDO(name, parent), cfg(cfg){}

    // SCDO interface
protected:
    bool getImpl() noexcept;
    void setImpl(bool state) noexcept;
    void initImpl(){}

private:
    ACSDOConfig* cfg;
};

#endif // ACSDO_H
