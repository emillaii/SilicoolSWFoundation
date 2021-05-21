#ifndef RENAMEMANAGER_H
#define RENAMEMANAGER_H

#include "configManager/configfile.h"
#include "renamemanagerconfig.h"
#include "utilities_global.h"
#include <QObject>

struct SubscribedObjPro
{
public:
    SubscribedObjPro(QObject *obj, const char *pro) : obj(obj), pro(pro) {}

    QObject *obj;
    const char *pro;
};

class RenameHelper
{
public:
    void subscribe(QObject *obj, const char *property);

    void rename(const QString &oldName, const QString &newName);

private:
    QList<SubscribedObjPro> subscribedObjPros;
};

class UTILITIESSHARED_EXPORT RenameManager
{
private:
    RenameManager();

public:
    static RenameManager &getIns()
    {
        static RenameManager instance;
        return instance;
    }

    void subscribeAxisNameChanged(QObject *obj, const char *property);
    void subscribeDiNameChanged(QObject *obj, const char *property);
    void subscribeDoNameChanged(QObject *obj, const char *property);
    void subscribeVacuumNameChanged(QObject *obj, const char *property);
    void subscribeCylNameChanged(QObject *obj, const char *property);
    void subscribeAxisModuleNameChanged(QObject *obj, const char *property);
    void subscribeCameraNameChanged(QObject *obj, const char *property);
    void subscribePrNameChanged(QObject *obj, const char *property);

    void renameAxis(const QString &oldName, const QString &newName);
    void renameDi(const QString &oldName, const QString &newName);
    void renameDo(const QString &oldName, const QString &newName);
    void renameVacuum(const QString &oldName, const QString &newName);
    void renameCyl(const QString &oldName, const QString &newName);
    void renameAxisModule(const QString &oldName, const QString &newName);
    void renameCamera(const QString &oldName, const QString &newName);
    void renamePr(const QString &oldName, const QString &newName);

private:
    bool enable = false;

    RenameHelper axisRenameHelper;
    RenameHelper diRenameHelper;
    RenameHelper doRenameHelper;
    RenameHelper vacuumRenameHelper;
    RenameHelper cylRenameHelper;
    RenameHelper axisModuleRenameHelper;
    RenameHelper cameraRenameHelper;
    RenameHelper prRenameHelper;
};

#endif    // RENAMEMANAGER_H
