#ifndef COMMANDEXTRACTOR_H
#define COMMANDEXTRACTOR_H

#include <QDir>
#include <QFile>
#include <QMetaMethod>
#include <QObject>

///
/// Helper function
/// This function would extract all public slot methods and Q_INVOKABLE methods of the given class derived from QObject,
/// then save these method names to an js file.
/// You can import this js file in qml, and write functin name more convenient.
/// You can see more details in "TaskEngineeTest" project.
///
template <typename T>
void extractCommand(QString destJsFileDir = "")
{
    auto metaObj = T::staticMetaObject;
    QStringList cmds;
    for (int i = 0; i < metaObj.methodCount(); i++)
    {
        auto metaMethod = metaObj.method(i);
        if (metaMethod.access() == QMetaMethod::Public
            && (metaMethod.methodType() == QMetaMethod::Slot || metaMethod.methodType() == QMetaMethod::Method))
        {
            QString cmdName(metaMethod.name());
            if (cmdName != "deleteLater" && !cmds.contains(cmdName))
            {
                cmds.append(metaMethod.name());
            }
        }
    }
    if (cmds.count() > 0)
    {
        QString fileName = QString(metaObj.className()) + "Cmds.js";
        QString fileContent;
        foreach (auto cmd, cmds)
        {
            fileContent += QString("var %1 = \"%1\"\r\n").arg(cmd);
        }
        if (destJsFileDir.isEmpty())
        {
            destJsFileDir = "./";
        }
        QDir destDir(destJsFileDir);
        if (!destDir.exists())
        {
            destDir.mkpath(destJsFileDir);
        }
        QFile file(destJsFileDir + "/" + fileName);
        file.open(QFile::WriteOnly);
        file.write(fileContent.toUtf8());
        file.close();
    }
}

#endif    // COMMANDEXTRACTOR_H
