#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QObject>

class TestClass : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int i READ i WRITE setI NOTIFY iChanged)
    Q_PROPERTY(double d READ d WRITE setD NOTIFY dChanged)
    Q_PROPERTY(QString s READ s WRITE setS NOTIFY sChanged)
    Q_PROPERTY(bool b READ b WRITE setB NOTIFY bChanged)

public:
    Q_INVOKABLE TestClass() {}
    virtual ~TestClass() {}

    int i() const
    {
        return m_i;
    }

    double d() const
    {
        return m_d;
    }

    QString s() const
    {
        return m_s;
    }

    bool b() const
    {
        return m_b;
    }

public slots:
    void setI(int i)
    {
        if (m_i == i)
            return;

        m_i = i;
        emit iChanged(m_i);
    }

    void setD(double d)
    {
        if (qFuzzyCompare(m_d, d))
            return;

        m_d = d;
        emit dChanged(m_d);
    }

    void setS(QString s)
    {
        if (m_s == s)
            return;

        m_s = s;
        emit sChanged(m_s);
    }

    void setB(bool b)
    {
        if (m_b == b)
            return;

        m_b = b;
        emit bChanged(m_b);
    }

signals:
    void iChanged(int i);

    void dChanged(double d);

    void sChanged(QString s);

    void bChanged(bool b);

private:
    int m_i = 0;
    double m_d = 0;
    QString m_s;
    bool m_b = false;
};

#endif    // TESTCLASS_H
