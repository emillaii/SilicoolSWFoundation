#ifndef TRAYMAP_H
#define TRAYMAP_H

#include "QMetaObject"
#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "errorHandling/silicolerror.h"
#include "materialdata.h"
#include "trayconfig.h"
#include "utilities_global.h"
#include <QAbstractTableModel>

class UTILITIESSHARED_EXPORT TrayMap : public QAbstractTableModel
{
    Q_OBJECT
public:
    TrayMap(TrayConfig *trayConfig, QString trayMapName, QString materialDataStoreFileName, const QMetaObject *materialDataMetaObj);

    Q_INVOKABLE void reset(int materialState = 0);

    Q_INVOKABLE void setMaterialState(int rowIndexStart, int columnIndexStart, int rowIndexEnd, int columnIndexEnd, QString targetState);

    template <typename T>
    T *getData(int rowIndex, int columnIndex)
    {
        if (rowIndex >= trayConfig->rows())
        {
            throw SilicolAbort(
                tr("Row index exceeded limit! TrayMap: %1, Rows: %2, RowInde: %3").arg(trayMapName).arg(trayConfig->rows()).arg(rowIndex),
                EX_LOCATION);
        }
        if (columnIndex >= trayConfig->columns())
        {
            throw SilicolAbort(tr("Column index exceeded limit! TrayMap: %1, Columns: %2, ColumnIndex: %3")
                                   .arg(trayMapName)
                                   .arg(trayConfig->columns())
                                   .arg(columnIndex),
                               EX_LOCATION);
        }
        return materialDatas->getConfig<T>(trayConfig->columns() * rowIndex + columnIndex);
    }

    QJsonValue toJson();

    void fromJson(QJsonValue jv);

    Q_INVOKABLE void grabMapToImage(QString imageFileFullName, int imageWidth = -1, int imageHeight = -1);

public slots:
    void onTrayRowColumnCountChanged(int count);
    void onMaterialStateChanged(int rowIndex, int columnIndex);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void reqGrabMapToImage(QString imageFileFullName, int imageWidth, int imageHeight, QPrivateSignal);
    // For TrayMapEditor UI
public slots:
    void grabMapToImageDone();

private:
    void connectStateChangedSignal();

protected:
    TrayConfig *trayConfig;

private:
    QString trayMapName;
    ConfigObjectArray *materialDatas;
    ConfigFile *materailDatasStoreFile;
    bool saveMapResult = false;
};

#endif    // TRAYMAP_H
