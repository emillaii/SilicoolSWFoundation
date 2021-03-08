#ifndef MAGAZINEMAP_H
#define MAGAZINEMAP_H

#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "errorHandling/silicolerror.h"
#include "magazineconfig.h"
#include "traydata.h"
#include <QAbstractListModel>
#include <QObject>

class UTILITIESSHARED_EXPORT MagazineMap : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MagazineMap(MagazineConfig *magazineConfig,
                         QString magazineMapName,
                         QString trayDataStoreFileName,
                         const QMetaObject *trayDataMetaObj,
                         QObject *parent = nullptr);

    Q_INVOKABLE void reset(int trayState = 0);

    Q_INVOKABLE void setMaterialState(int indexStart, int indexEnd, QString targetState);

    template <typename T>
    T *getData(int index)
    {
        if (index >= magazineConfig->trayCount())
        {
            throw SilicolAbort(tr("Tray index exceeded limit! TrayMap: %1, TrayCount: %2, TrayIndex: %3")
                                   .arg(magazineMapName)
                                   .arg(magazineConfig->trayCount())
                                   .arg(index),
                               EX_LOCATION);
        }
        return trayDatas->getConfig<T>(index);
    }

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void reqEndResetModel(QPrivateSignal);

private slots:
    void onTrayCountChanged(int count);
    void onTrayStateChanged(int trayIndex);
    void onReqEndResetModel();

private:
    void connectStateChangedSignal();

private:
    MagazineConfig *magazineConfig;
    QString magazineMapName;
    ConfigObjectArray *trayDatas;
    ConfigFile *trayDatasStoreFile;
};

#endif    // MAGAZINEMAP_H
