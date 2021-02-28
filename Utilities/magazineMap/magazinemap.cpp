#include "magazinemap.h"

MagazineMap::MagazineMap(MagazineConfig *magazineConfig,
                         QString magazineMapName,
                         QString trayDataStoreFileName,
                         const QMetaObject *trayDataMetaObj,
                         QObject *parent)
    : QAbstractListModel(parent), magazineConfig(magazineConfig), magazineMapName(magazineMapName)
{
    connect(this, &MagazineMap::reqEndResetModel, this, &MagazineMap::onReqEndResetModel);

    trayDatas = new ConfigObjectArray(trayDataMetaObj, this);
    trayDatasStoreFile = new ConfigFile(magazineMapName, trayDatas, trayDataStoreFileName);
    trayDatasStoreFile->populate();

    if (magazineConfig->trayCount() != trayDatas->count())
    {
        qWarning() << tr("Tray data count was not equal to tray count! Now reset %1!").arg(magazineMapName);
        reset();
    }
    connect(magazineConfig, &MagazineConfig::trayCountChanged, this, &MagazineMap::onTrayCountChanged);
    connectStateChangedSignal();
}

void MagazineMap::reset(int trayState)
{
    beginResetModel();
    int trayCount = magazineConfig->trayCount();
    while (trayDatas->count() > trayCount)
    {
        trayDatas->executeRemoveConfigObject(trayDatas->count() - 1);
    }
    while (trayDatas->count() < trayCount)
    {
        trayDatas->executeAddConfigObject(trayDatas->count());
    }
    for (int i = 0; i < trayCount; i++)
    {
        trayDatas->getConfig<TrayData>(i)->reset(trayState);
    }
    connectStateChangedSignal();
    emit reqEndResetModel({});
}

void MagazineMap::setMaterialState(int indexStart, int indexEnd, QString targetState)
{
    for (int i = indexStart; i <= indexEnd; i++)
    {
        getData<TrayData>(i)->setTrayStateStr(targetState);
    }
}

int MagazineMap::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return magazineConfig->trayCount();
}

QVariant MagazineMap::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (index.row() < 0 || index.row() >= magazineConfig->trayCount())
    {
        return QVariant();
    }

    return trayDatas->getConfig<TrayData>(index.row())->getTrayStateStr();
}

QHash<int, QByteArray> MagazineMap::roleNames() const
{
    static QHash<int, QByteArray> m_roleNames;
    if (m_roleNames.count() == 0)
    {
        m_roleNames[Qt::DisplayRole] = "status";
    }
    return m_roleNames;
}

void MagazineMap::onTrayCountChanged(int count)
{
    Q_UNUSED(count)
    qInfo() << tr("Magazine tray count changed! Now reset %1!").arg(magazineMapName);
    reset();
}

void MagazineMap::onTrayStateChanged(int trayIndex)
{
    emit dataChanged(index(trayIndex), index(trayIndex));
}

void MagazineMap::onReqEndResetModel()
{
    endResetModel();
}

void MagazineMap::connectStateChangedSignal()
{
    for (int i = 0; i < trayDatas->count(); i++)
    {
        auto data = trayDatas->getConfig<TrayData>(i);
        data->setIndex(i);
        if (!data->hasStateChangedSignal)
        {
            connect(data, &TrayData::stateChanged, this, &MagazineMap::onTrayStateChanged);
            data->hasStateChangedSignal = true;
        }
    }
}
