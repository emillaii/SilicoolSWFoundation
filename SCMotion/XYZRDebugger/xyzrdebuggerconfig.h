#ifndef XYZRDEBUGGERCONFIG_H
#define XYZRDEBUGGERCONFIG_H

#include "configManager/configobject.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT XYZRDebuggerDefinition : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString debuggerName READ debuggerName WRITE setDebuggerName NOTIFY debuggerNameChanged)
    Q_PROPERTY(QString shortcut READ shortcut WRITE setShortcut NOTIFY shortcutChanged)

public:
    Q_INVOKABLE XYZRDebuggerDefinition(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    QString debuggerName() const
    {
        return m_debuggerName;
    }

    QString shortcut() const
    {
        return m_shortcut;
    }

public slots:
    void setDebuggerName(QString debuggerName)
    {
        if (m_debuggerName == debuggerName)
            return;

        m_debuggerName = debuggerName;
        emit debuggerNameChanged(m_debuggerName);
    }

    void setShortcut(QString shortcut)
    {
        if (m_shortcut == shortcut)
            return;

        m_shortcut = shortcut;
        emit shortcutChanged(m_shortcut);
    }

signals:
    void debuggerNameChanged(QString debuggerName);

    void shortcutChanged(QString shortcut);

private:
    QString m_debuggerName;
    QString m_shortcut;
};

class SCMOTIONSHARED_EXPORT XYZRDebuggerConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString debuggerName READ debuggerName WRITE setDebuggerName NOTIFY debuggerNameChanged)
    Q_PROPERTY(QString xAxisName READ xAxisName WRITE setXAxisName NOTIFY xAxisNameChanged)
    Q_PROPERTY(bool reverseXDir READ reverseXDir WRITE setReverseXDir NOTIFY reverseXDirChanged)
    Q_PROPERTY(QString yAxisName READ yAxisName WRITE setYAxisName NOTIFY yAxisNameChanged)
    Q_PROPERTY(bool reverseYDir READ reverseYDir WRITE setReverseYDir NOTIFY reverseYDirChanged)
    Q_PROPERTY(QString zAxisName READ zAxisName WRITE setZAxisName NOTIFY zAxisNameChanged)
    Q_PROPERTY(QString rAxisName READ rAxisName WRITE setRAxisName NOTIFY rAxisNameChanged)
    Q_PROPERTY(QString shortcut READ shortcut WRITE setShortcut NOTIFY shortcutChanged)

public:
    Q_INVOKABLE XYZRDebuggerConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("debuggerName");
        setReadOnlyProperty("shortcut");
        init();
    }

    void setOptionalAxisName(QVariantList &axisNames)
    {
        setOptionalProperty("xAxisName", axisNames);
        setOptionalProperty("yAxisName", axisNames);
        setOptionalProperty("zAxisName", axisNames);
        setOptionalProperty("rAxisName", axisNames);
    }

    QString debuggerName() const
    {
        return m_debuggerName;
    }

    QString xAxisName() const
    {
        return m_xAxisName;
    }

    bool reverseXDir() const
    {
        return m_reverseXDir;
    }

    QString yAxisName() const
    {
        return m_yAxisName;
    }

    bool reverseYDir() const
    {
        return m_reverseYDir;
    }

    QString zAxisName() const
    {
        return m_zAxisName;
    }

    QString rAxisName() const
    {
        return m_rAxisName;
    }

    QString shortcut() const
    {
        return m_shortcut;
    }

public slots:
    void setDebuggerName(QString debuggerName)
    {
        if (m_debuggerName == debuggerName)
            return;

        m_debuggerName = debuggerName;
        emit debuggerNameChanged(m_debuggerName);
    }

    void setXAxisName(QString xAxisName)
    {
        if (m_xAxisName == xAxisName)
            return;

        m_xAxisName = xAxisName;
        emit xAxisNameChanged(m_xAxisName);
    }

    void setReverseXDir(bool reverseXDir)
    {
        if (m_reverseXDir == reverseXDir)
            return;

        m_reverseXDir = reverseXDir;
        emit reverseXDirChanged(m_reverseXDir);
    }

    void setYAxisName(QString yAxisName)
    {
        if (m_yAxisName == yAxisName)
            return;

        m_yAxisName = yAxisName;
        emit yAxisNameChanged(m_yAxisName);
    }

    void setReverseYDir(bool reverseYDir)
    {
        if (m_reverseYDir == reverseYDir)
            return;

        m_reverseYDir = reverseYDir;
        emit reverseYDirChanged(m_reverseYDir);
    }

    void setZAxisName(QString zAxisName)
    {
        if (m_zAxisName == zAxisName)
            return;

        m_zAxisName = zAxisName;
        emit zAxisNameChanged(m_zAxisName);
    }

    void setRAxisName(QString rAxisName)
    {
        if (m_rAxisName == rAxisName)
            return;

        m_rAxisName = rAxisName;
        emit rAxisNameChanged(m_rAxisName);
    }

    void setShortcut(QString shortcut)
    {
        if (m_shortcut == shortcut)
            return;

        m_shortcut = shortcut;
        emit shortcutChanged(m_shortcut);
    }

signals:
    void debuggerNameChanged(QString debuggerName);

    void xAxisNameChanged(QString xAxisName);

    void reverseXDirChanged(bool reverseXDir);

    void yAxisNameChanged(QString yAxisName);

    void reverseYDirChanged(bool reverseYDir);

    void zAxisNameChanged(QString zAxisName);

    void rAxisNameChanged(QString rAxisName);

    void shortcutChanged(QString shortcut);

private:
    QString m_debuggerName;
    QString m_xAxisName;
    bool m_reverseXDir = false;
    QString m_yAxisName;
    bool m_reverseYDir = false;
    QString m_zAxisName;
    QString m_rAxisName;
    QString m_shortcut;
};

#endif    // XYZRDEBUGGERCONFIG_H
