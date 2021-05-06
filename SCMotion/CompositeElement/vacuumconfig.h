#ifndef VACUUMCONFIG_H
#define VACUUMCONFIG_H

#include "configManager/configobject.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT VacuumConfig : public ConfigObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString feedbackInputName READ feedbackInputName WRITE setFeedbackInputName NOTIFY feedbackInputNameChanged)
    Q_PROPERTY(QString vacuumizeOutputName READ vacuumizeOutputName WRITE setVacuumizeOutputName NOTIFY vacuumizeOutputNameChanged)
    Q_PROPERTY(QString blowOutputName READ blowOutputName WRITE setBlowOutputName NOTIFY blowOutputNameChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(int blowDelayAfterCloseVacuum READ blowDelayAfterCloseVacuum WRITE setBlowDelayAfterCloseVacuum NOTIFY blowDelayAfterCloseVacuumChanged)
    Q_PROPERTY(int blowTime READ blowTime WRITE setBlowTime NOTIFY blowTimeChanged)
    Q_PROPERTY(int blowTime2 READ blowTime2 WRITE setBlowTime2 NOTIFY blowTime2Changed)

    Q_INVOKABLE VacuumConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("name");
        setIdentityProperty("name");
        QStringList args;
        args << "feedbackInputName"
             << "vacuumizeOutputName"
             << "blowOutputName";
        setEngineerAuthorities(args);
        setUnit("timeout", "ms");
        setUnit("blowDelayAfterCloseVacuum", "ms");
        setUnit("blowTime", "ms");
        setUnit("blowTime2", "ms");
        init();
    }

    void setOptionalIO(QVariantList optionalDIs, QVariantList optionalDOs)
    {
        setOptionalProperty("feedbackInputName", optionalDIs);
        setOptionalProperty("vacuumizeOutputName", optionalDOs);
        QVariantList emptyAppendedOptions(optionalDOs);
        emptyAppendedOptions.append(QString(""));
        setOptionalProperty("blowOutputName", emptyAppendedOptions);
        init();
    }

    QString name() const
    {
        return m_name;
    }

    QString feedbackInputName() const
    {
        return m_feedbackInputName;
    }

    QString vacuumizeOutputName() const
    {
        return m_vacuumizeOutputName;
    }

    QString blowOutputName() const
    {
        return m_blowOutputName;
    }

    int timeout() const
    {
        return m_timeout;
    }

    int blowDelayAfterCloseVacuum() const
    {
        return m_blowDelayAfterCloseVacuum;
    }

    int blowTime() const
    {
        return m_blowTime;
    }

    int blowTime2() const
    {
        return m_blowTime2;
    }

public slots:
    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

    void setFeedbackInputName(QString feedbackInputName)
    {
        if (m_feedbackInputName == feedbackInputName)
            return;

        m_feedbackInputName = feedbackInputName;
        emit feedbackInputNameChanged(m_feedbackInputName);
    }

    void setVacuumizeOutputName(QString vacuumizeOutputName)
    {
        if (m_vacuumizeOutputName == vacuumizeOutputName)
            return;

        m_vacuumizeOutputName = vacuumizeOutputName;
        emit vacuumizeOutputNameChanged(m_vacuumizeOutputName);
    }

    void setBlowOutputName(QString blowOutputName)
    {
        if (m_blowOutputName == blowOutputName)
            return;

        m_blowOutputName = blowOutputName;
        emit blowOutputNameChanged(m_blowOutputName);
    }

    void setTimeout(int timeout)
    {
        if (m_timeout == timeout)
            return;

        m_timeout = timeout;
        emit timeoutChanged(m_timeout);
    }

    void setBlowDelayAfterCloseVacuum(int blowDelayAfterCloseVacuum)
    {
        if (m_blowDelayAfterCloseVacuum == blowDelayAfterCloseVacuum)
            return;

        m_blowDelayAfterCloseVacuum = blowDelayAfterCloseVacuum;
        emit blowDelayAfterCloseVacuumChanged(m_blowDelayAfterCloseVacuum);
    }

    void setBlowTime(int blowTime)
    {
        if (m_blowTime == blowTime)
            return;

        m_blowTime = blowTime;
        emit blowTimeChanged(m_blowTime);
    }

    void setBlowTime2(int blowTime2)
    {
        if (m_blowTime2 == blowTime2)
            return;

        m_blowTime2 = blowTime2;
        emit blowTime2Changed(m_blowTime2);
    }

signals:
    void nameChanged(QString name);

    void feedbackInputNameChanged(QString feedbackInputName);

    void vacuumizeOutputNameChanged(QString vacuumizeOutputName);

    void blowOutputNameChanged(QString blowOutputName);

    void timeoutChanged(int timeout);

    void blowDelayAfterCloseVacuumChanged(int blowDelayAfterCloseVacuum);

    void blowTimeChanged(int blowTime);

    void blowTime2Changed(int blowTime2);

private:
    QString m_name = "";
    QString m_feedbackInputName = "";
    QString m_vacuumizeOutputName = "";
    QString m_blowOutputName = "";
    int m_timeout = 1000;
    int m_blowDelayAfterCloseVacuum = -1;
    int m_blowTime = -1;
    int m_blowTime2 = -1;
};

#endif    // VACUUMCONFIG_H
