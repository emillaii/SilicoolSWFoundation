#include "traymap.h"

TrayMap::TrayMap(TrayConfig *trayConfig, QString trayMapName, QString materialDataStoreFileName, const QMetaObject *materialDataMetaObj)
    : trayConfig(trayConfig), trayMapName(trayMapName)
{
    materialDatas = new ConfigObjectArray(materialDataMetaObj, this);
    materailDatasStoreFile = new ConfigFile(trayMapName, materialDatas, materialDataStoreFileName);
    materailDatasStoreFile->populate();
    if (trayConfig->materialCount() != materialDatas->count())
    {
        qWarning() << tr("Material data count was not equal to materail count! Now reset %1!").arg(trayMapName);
        reset();
    }
    connect(trayConfig, &TrayConfig::unitRowsChanged, this, &TrayMap::onTrayRowColumnCountChanged);
    connect(trayConfig, &TrayConfig::unitColumnsChanged, this, &TrayMap::onTrayRowColumnCountChanged);
    connect(trayConfig, &TrayConfig::rowsInsideUnitChanged, this, &TrayMap::onTrayRowColumnCountChanged);
    connect(trayConfig, &TrayConfig::columnsInsideUnitChanged, this, &TrayMap::onTrayRowColumnCountChanged);
    connectStateChangedSignal();
}

void TrayMap::reset(int materialState)
{
    beginResetModel();
    int materialCount = trayConfig->materialCount();
    while (materialDatas->count() > materialCount)
    {
        materialDatas->executeRemoveConfigObject(materialDatas->count() - 1);
    }
    while (materialDatas->count() < materialCount)
    {
        materialDatas->executeAddConfigObject(materialDatas->count());
    }
    for (int i = 0; i < materialCount; i++)
    {
        materialDatas->getConfig<MaterialData>(i)->reset(materialState);
    }
    connectStateChangedSignal();
    endResetModel();
}

void TrayMap::setMaterialState(int rowIndexStart, int columnIndexStart, int rowIndexEnd, int columnIndexEnd, QString targetState)
{
    for (int r = rowIndexStart; r <= rowIndexEnd; r++)
    {
        for (int c = columnIndexStart; c <= columnIndexEnd; c++)
        {
            getData<MaterialData>(r, c)->setMaterialStateStr(targetState);
        }
    }
}

QJsonValue TrayMap::toJson()
{
    QJsonValue jv;
    materialDatas->write(jv);
    return jv;
}

void TrayMap::fromJson(QJsonValue jv)
{
    materialDatas->read(jv);
}

void TrayMap::grabMapToImage(QString imageFileFullName, int imageWidth, int imageHeight)
{
    QFileInfo fileInfo(imageFileFullName);
    QDir dir;
    if (!dir.exists(fileInfo.absolutePath()))
    {
        dir.mkpath(fileInfo.absolutePath());
    }
    saveMapResult = false;
    emit reqGrabMapToImage(imageFileFullName, imageWidth, imageHeight, {});
    if (!silicoolWait(1000, [this] { return saveMapResult; }))
    {
        qCritical() << tr("Grab map to image timeout, trayMap: %1, targetFileName: %2").arg(trayMapName).arg(imageFileFullName);
    }
}

void TrayMap::onTrayRowColumnCountChanged(int count)
{
    Q_UNUSED(count)
    qInfo() << tr("Tray row or column count changed! Now reset %1!").arg(trayMapName);
    reset();
}

void TrayMap::onMaterialStateChanged(int rowIndex, int columnIndex)
{
    emit dataChanged(index(rowIndex, columnIndex), index(rowIndex, columnIndex));
}

int TrayMap::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return trayConfig->rows();
}

int TrayMap::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return trayConfig->columns();
}

QVariant TrayMap::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (index.row() < 0 || index.row() >= trayConfig->rows())
    {
        return QVariant();
    }
    if (index.column() < 0 || index.column() >= trayConfig->columns())
    {
        return QVariant();
    }

    return materialDatas->getConfig<MaterialData>(trayConfig->columns() * index.row() + index.column())->getMaterialStateStr();
}

QHash<int, QByteArray> TrayMap::roleNames() const
{
    static QHash<int, QByteArray> m_roleNames;
    if (m_roleNames.count() == 0)
    {
        m_roleNames[Qt::DisplayRole] = "status";
    }
    return m_roleNames;
}

void TrayMap::grabMapToImageDone()
{
    saveMapResult = true;
}

void TrayMap::connectStateChangedSignal()
{
    for (int i = 0; i < materialDatas->count(); i++)
    {
        auto data = materialDatas->getConfig<MaterialData>(i);
        int columnIndex = i % trayConfig->columns();
        int rowIndex = i / trayConfig->columns();
        data->setIndex(rowIndex, columnIndex);
        if (!data->hasStateChangedSignal)
        {
            connect(data, &MaterialData::stateChanged, this, &TrayMap::onMaterialStateChanged);
            data->hasStateChangedSignal = true;
        }
    }
}
