#include "duttypemanager.h"

void DutTypeManager::addDutType(QString dutType, QString copyFromDutType)
{
    if (m_dutTypes.contains(dutType))
    {
        UIOperation::getIns()->showError(tr("Dut type %1 already exist!").arg(dutType));
        return;
    }
    QDir targetDir(QString("./config/DutType/%1").arg(dutType));
    targetDir.mkpath(".");
    updateDutTypes();
    UIOperation::getIns()->showTip(tr("Add new dut type (%1) successful!").arg(dutType));
    if (!copyFromDutType.isEmpty())
    {
        QDir sourceDir(QString("./config/DutType/%1").arg(copyFromDutType));
        if (sourceDir.exists())
        {
            copyDir(sourceDir.path(), targetDir.path());
        }
        else
        {
            UIOperation::getIns()->showError(tr("Copy from %1 failed because %1 is not exist!").arg(dutType));
            return;
        }
    }
}

void DutTypeManager::removeDutType(QString dutType)
{
    if (!m_dutTypes.contains(dutType))
    {
        UIOperation::getIns()->showError(tr("Dut type %1 did not exist!").arg(dutType));
        return;
    }
    if (dutType == basicConfig->dutType())
    {
        UIOperation::getIns()->showError(tr("Current used dut type (%1) can not be removed!").arg(dutType));
        return;
    }
    if (!UIOperation::getIns()->yesNoConfirm(
            tr("Remove dut type will delete all related config files!\r\nAre you sure to do that?")))
    {
        return;
    }
    removeDir(QString("./config/DutType/%1").arg(dutType));
    updateDutTypes();
    UIOperation::getIns()->showTip(tr("Remove dut type (%1) successful!").arg(dutType));
}

void DutTypeManager::copyDir(const QString &source, const QString &target)
{
    QDir sourceDir(source);
    auto subFiles = sourceDir.entryInfoList();
    foreach (auto subFile, subFiles)
    {
        QString targetSubFile = QString("%1/%2").arg(target).arg(subFile.fileName());
        if (subFile.isFile())
        {
            QFile::copy(subFile.absoluteFilePath(), targetSubFile);
        }
        else
        {
            if (subFile.fileName() == "." || subFile.fileName() == "..")
            {
                continue;
            }
            QDir dir;
            dir.mkpath(targetSubFile);
            copyDir(subFile.absoluteFilePath(), targetSubFile);
        }
    }
}

void DutTypeManager::removeDir(const QString &dirName)
{
    QDir dir(dirName);
    if (!dir.exists())
    {
        return;
    }
    auto subFiles = dir.entryInfoList();
    foreach (auto subFile, subFiles)
    {
        if (subFile.isFile())
        {
            QFile::remove(subFile.absoluteFilePath());
        }
        else
        {
            if (subFile.fileName() == "." || subFile.fileName() == "..")
            {
                continue;
            }
            removeDir(subFile.absoluteFilePath());
        }
    }
    dir.rmdir(".");
}

void DutTypeManager::updateDutTypes()
{
    m_dutTypes.clear();
    QDir dutTypeDir("./config/DutType");
    auto subFiles = dutTypeDir.entryInfoList();
    foreach (auto subFile, subFiles)
    {
        if (subFile.isDir())
        {
            if (subFile.fileName() != "." && subFile.fileName() != "..")
            {
                m_dutTypes.append(subFile.fileName());
            }
        }
    }
    if (m_dutTypes.length() == 0)
    {
        dutTypeDir.mkpath(DefaultDut);
        m_dutTypes.append(DefaultDut);
    }
    emit dutTypesChanged(m_dutTypes);
}
