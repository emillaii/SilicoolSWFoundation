#ifndef DUMMYDI_H
#define DUMMYDI_H

#include "ioconfig.h"
#include "scdi.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT DummyIOConfig : public IOConfig
{
    Q_OBJECT

    Q_PROPERTY(bool dummyValue READ dummyValue WRITE setDummyValue NOTIFY dummyValueChanged)

public:
    Q_INVOKABLE DummyIOConfig(QObject *parent = nullptr) : IOConfig(parent)
    {
        init();
    }

    bool dummyValue() const
    {
        return m_dummyValue;
    }

public slots:
    void setDummyValue(bool dummyValue)
    {
        if (m_dummyValue == dummyValue)
            return;

        m_dummyValue = dummyValue;
        emit dummyValueChanged(m_dummyValue);
    }

signals:
    void dummyValueChanged(bool dummyValue);

private:
    bool m_dummyValue = false;
};

class SCMOTIONSHARED_EXPORT DummyDI : public SCDI
{
    Q_OBJECT
public:
    explicit DummyDI(QString name, QObject *parent = nullptr);

    // SCDI interface
protected:
    virtual bool getImpl() noexcept override;
    virtual void initImpl() override;

private:
    DummyIOConfig *dummyIOConfig;
};

#endif    // DUMMYDI_H
