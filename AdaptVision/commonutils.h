#ifndef COMMONUTILS_H
#define COMMONUTILS_H
#include <QObject>
#include <QFileInfo>

static bool checkFileExist(QString filename){
    QFileInfo fileInfo(filename);
    if(!fileInfo.isFile())
    {
        qInfo("pr file name not exist: %s", filename.toStdString().c_str());
        return false;
    }
    return true;
}

#endif // COMMONUTILS_H
