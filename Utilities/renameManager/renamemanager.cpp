#include "renamemanager.h"

void RenameHelper::subscribe(QObject *obj, const char *property)
{
    subscribedObjPros.append(SubscribedObjPro(obj, property));
}

void RenameHelper::rename(const QString &oldName, const QString &newName)
{
    foreach (auto subscribedObjPro, subscribedObjPros)
    {
        if (subscribedObjPro.obj->property(subscribedObjPro.pro).toString() == oldName)
        {
            if (!subscribedObjPro.obj->setProperty(subscribedObjPro.pro, newName))
            {
                qCritical() << QString(u8"重命名失败! 对象类型：%1，属性名：%2，原名称：%3，新名称：%4")
                                   .arg(subscribedObjPro.obj->metaObject()->className())
                                   .arg(subscribedObjPro.pro)
                                   .arg(oldName)
                                   .arg(newName);
            }
        }
    }
}

RenameManager::RenameManager()
{
    RenameMgrConfig config;
    ConfigFile configFile("RenameMgrConfig", &config, "./config/renameManagerConfig.json", false);
    configFile.populate(true);
    enable = config.enable();
}

void RenameManager::subscribeAxisNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    axisRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribeDiNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    diRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribeDoNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    doRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribeVacuumNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    vacuumRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribeCylNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    cylRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribeAxisModuleNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    axisModuleRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribeCameraNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    cameraRenameHelper.subscribe(obj, property);
}

void RenameManager::subscribePrNameChanged(QObject *obj, const char *property)
{
    if (!enable)
    {
        return;
    }
    prRenameHelper.subscribe(obj, property);
}

void RenameManager::renameAxis(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    axisRenameHelper.rename(oldName, newName);
}

void RenameManager::renameDi(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    diRenameHelper.rename(oldName, newName);
}

void RenameManager::renameDo(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    doRenameHelper.rename(oldName, newName);
}

void RenameManager::renameVacuum(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    vacuumRenameHelper.rename(oldName, newName);
}

void RenameManager::renameCyl(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    cylRenameHelper.rename(oldName, newName);
}

void RenameManager::renameAxisModule(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    axisModuleRenameHelper.rename(oldName, newName);
}

void RenameManager::renameCamera(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    cameraRenameHelper.rename(oldName, newName);
}

void RenameManager::renamePr(const QString &oldName, const QString &newName)
{
    if (!enable)
    {
        return;
    }
    prRenameHelper.rename(oldName, newName);
}
