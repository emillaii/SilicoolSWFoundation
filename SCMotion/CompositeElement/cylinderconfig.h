#ifndef CYLINDERCONFIG_H
#define CYLINDERCONFIG_H

#include "configManager/configobject.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT CylinderConfig : public ConfigObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString falseInput READ falseInput WRITE setFalseInput NOTIFY falseInputChanged)
    Q_PROPERTY(QString trueInput READ trueInput WRITE setTrueInput NOTIFY trueInputChanged)
    Q_PROPERTY(QString falseOutput READ falseOutput WRITE setFalseOutput NOTIFY falseOutputChanged)
    Q_PROPERTY(QString trueOutput READ trueOutput WRITE setTrueOutput NOTIFY trueOutputChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(int delayAfterMoveToFalse READ delayAfterMoveToFalse WRITE setDelayAfterMoveToFalse NOTIFY delayAfterMoveToFalseChanged)
    Q_PROPERTY(int delayAfterMoveoToTrue READ delayAfterMoveoToTrue WRITE setDelayAfterMoveoToTrue NOTIFY delayAfterMoveoToTrueChanged)
    Q_PROPERTY(bool noFalseInputSNR READ noFalseInputSNR WRITE setNoFalseInputSNR NOTIFY noFalseInputSNRChanged)
    Q_PROPERTY(bool noTrueInputSNR READ noTrueInputSNR WRITE setNoTrueInputSNR NOTIFY noTrueInputSNRChanged)

    Q_INVOKABLE CylinderConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("name");
        setIdentityProperty("name");
        init();
    }

    void setOptionalIO(QVariantList optionalDIs, QVariantList optionalDOs)
    {
        QVariantList emptyAppendedDos(optionalDOs);
        emptyAppendedDos.append(QString(""));
        QVariantList emptyAppendedDis(optionalDIs);
        emptyAppendedDis.append(QString(""));
        setOptionalProperty("falseInput", emptyAppendedDis);
        setOptionalProperty("trueInput", emptyAppendedDis);
        setOptionalProperty("falseOutput", emptyAppendedDos);
        setOptionalProperty("trueOutput", optionalDOs);
        init();
    }

    QString falseInput() const
    {
        return m_falseInput;
    }
    QString trueInput() const
    {
        return m_trueInput;
    }

    QString falseOutput() const
    {
        return m_falseOutput;
    }

    QString trueOutput() const
    {
        return m_trueOutput;
    }

    int timeout() const
    {
        return m_timeout;
    }

    int delayAfterMoveToFalse() const
    {
        return m_delayAfterMoveToFalse;
    }

    int delayAfterMoveoToTrue() const
    {
        return m_delayAfterMoveoToTrue;
    }

    QString name() const
    {
        return m_name;
    }

    bool noFalseInputSNR() const
    {
        return m_noFalseInputSNR;
    }

    bool noTrueInputSNR() const
    {
        return m_noTrueInputSNR;
    }

public slots:
    void setFalseInput(QString falseInput)
    {
        if (m_falseInput == falseInput)
            return;

        m_falseInput = falseInput;
        emit falseInputChanged(m_falseInput);
    }
    void setTrueInput(QString trueInput)
    {
        if (m_trueInput == trueInput)
            return;

        m_trueInput = trueInput;
        emit trueInputChanged(m_trueInput);
    }

    void setFalseOutput(QString falseOutput)
    {
        if (m_falseOutput == falseOutput)
            return;

        m_falseOutput = falseOutput;
        emit falseOutputChanged(m_falseOutput);
    }

    void setTrueOutput(QString trueOutput)
    {
        if (m_trueOutput == trueOutput)
            return;

        m_trueOutput = trueOutput;
        emit trueOutputChanged(m_trueOutput);
    }

    void setTimeout(int timeout)
    {
        if (m_timeout == timeout)
            return;

        m_timeout = timeout;
        emit timeoutChanged(m_timeout);
    }

    void setDelayAfterMoveToFalse(int delayAfterMoveToFalse)
    {
        if (m_delayAfterMoveToFalse == delayAfterMoveToFalse)
            return;

        m_delayAfterMoveToFalse = delayAfterMoveToFalse;
        emit delayAfterMoveToFalseChanged(m_delayAfterMoveToFalse);
    }

    void setDelayAfterMoveoToTrue(int delayAfterMoveoToTrue)
    {
        if (m_delayAfterMoveoToTrue == delayAfterMoveoToTrue)
            return;

        m_delayAfterMoveoToTrue = delayAfterMoveoToTrue;
        emit delayAfterMoveoToTrueChanged(m_delayAfterMoveoToTrue);
    }

    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

    void setNoFalseInputSNR(bool noFalseInputSNR)
    {
        if (m_noFalseInputSNR == noFalseInputSNR)
            return;

        m_noFalseInputSNR = noFalseInputSNR;
        emit noFalseInputSNRChanged(m_noFalseInputSNR);
    }

    void setNoTrueInputSNR(bool noTrueInputSNR)
    {
        if (m_noTrueInputSNR == noTrueInputSNR)
            return;

        m_noTrueInputSNR = noTrueInputSNR;
        emit noTrueInputSNRChanged(m_noTrueInputSNR);
    }

signals:
    void falseInputChanged(QString falseInput);
    void trueInputChanged(QString trueInput);
    void falseOutputChanged(QString falseOutput);
    void trueOutputChanged(QString trueOutput);
    void timeoutChanged(int timeout);
    void delayAfterMoveToFalseChanged(int delayAfterMoveToFalse);
    void delayAfterMoveoToTrueChanged(int delayAfterMoveoToTrue);
    void nameChanged(QString name);
    void noFalseInputSNRChanged(bool noFalseInputSNR);
    void noTrueInputSNRChanged(bool noTrueInputSNR);

private:
    QString m_name = "";
    QString m_falseInput = "";
    QString m_trueInput = "";
    QString m_falseOutput = "";
    QString m_trueOutput = "";
    int m_timeout = 3000;
    int m_delayAfterMoveToFalse = 0;
    int m_delayAfterMoveoToTrue = 0;
    bool m_noFalseInputSNR = false;
    bool m_noTrueInputSNR = false;
};

#endif    // CYLINDERCONFIG_H
