#ifndef LANGUAGECONFIG_H
#define LANGUAGECONFIG_H

#include "../configManager/configarray.h"
#include "../configManager/configobject.h"
#include "../utilities_global.h"

class UTILITIESSHARED_EXPORT LanguageConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(ConfigArray *languagePackageNames READ languagePackageNames)

public:
    enum Language
    {
        Chinese,
        English
    };
    Q_ENUM(Language)

    LanguageConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_languagePackageNames = new ConfigArray(ConfigElementInfo::Other, this);
        init();
    }

    void publishLanguage()
    {
        emit languageChanged(m_language);
    }

    int language() const
    {
        return m_language;
    }

    ConfigArray *languagePackageNames() const
    {
        return m_languagePackageNames;
    }

public slots:
    void setLanguage(int language)
    {
        if (m_language == language)
            return;

        m_language = language;
        emit languageChanged(m_language);
    }

signals:
    void languageChanged(int language);

private:
    int m_language = 0;
    ConfigArray *m_languagePackageNames;
};

#endif    // LANGUAGECONFIG_H
