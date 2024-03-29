#ifndef POINTSTABLEMODEL_H
#define POINTSTABLEMODEL_H

#include "QFileInfo"
#include "utilities_global.h"
#include <QAbstractTableModel>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QQmlParserStatus>

class UTILITIESSHARED_EXPORT PointsTableModel : public QAbstractTableModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QStringList horHeader READ getHorHeader WRITE setHorHeader NOTIFY horHeaderChanged)
    Q_PROPERTY(QJsonArray initData READ getInitData WRITE setInitData NOTIFY initDataChanged)
    Q_PROPERTY(QJsonArray velocityData READ getVelocityData WRITE setVelocityData NOTIFY velocityDataChanged)

public:
    explicit PointsTableModel(QObject *parent = nullptr);

    QStringList getHorHeader() const;
    void setHorHeader(const QStringList &header);

    QJsonArray getInitData() const;
    void setInitData(const QJsonArray &jsonArr);

    QJsonArray getVelocityData() const;
    void setVelocityData(const QJsonArray &jsonArr);

    // QQmlParserStatus：构造前
    void classBegin() override;
    // QQmlParserStatus：构造后
    void componentComplete() override;
    // 自定义role
    QHash<int, QByteArray> roleNames() const override;

    // 表头
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // 数据，这三个必须实现
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 编辑
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    // bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    // bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    void loadData(const QJsonArray &data);

signals:
    void horHeaderChanged();
    void initDataChanged();
    void velocityDataChanged();
    void dataLoaded();
    void newPathAdded();

private:
    // 组件是否初始化完成
    bool _completed = false;
    QJsonArray _initData;
    QJsonArray _velocityData;
    // 数据，我一般纯展示，用vector就行了
    QVector<QVector<QVariant>> _modelData;
    // 横项表头
    QList<QString> _horHeaderList;

public slots:
    void clearTable();
    void refreshData(QJsonArray data);
    void addPoint(double x, double y, QString type);
    // Update model data
    void updateData(int row, int col, double newValue);
    // Save Output Dispense Path Json file
    void saveOutputJson(QString filename);
    void saveRawImage(QString imageUrl, QString filename);
    // Load DispensePath Json file
    void loadJson(QString filename);
    QJsonObject checkImageSize(QString filename);
};

#endif    // POINTSTABLEMODEL_H
