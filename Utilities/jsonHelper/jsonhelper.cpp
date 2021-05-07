#include "jsonhelper.h"

void JsonHelper::addFilteredPropertyName(const QString &propertyName)
{
    m_filteredPropertyName.append(propertyName);
}

void JsonHelper::addFilteredPropertyNames(const QStringList &propertyNames)
{
    m_filteredPropertyName.append(propertyNames);
}
