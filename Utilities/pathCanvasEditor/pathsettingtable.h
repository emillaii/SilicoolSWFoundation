#ifndef PATHSETTINGTABLE_H
#define PATHSETTINGTABLE_H

#include <QAbstractTableModel>
#include <QQmlParserStatus>
#include <QHash>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT PathSettingTable : public QAbstractTableModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QStringList horHeader READ getHorHeader WRITE setHorHeader NOTIFY horHeaderChanged)
    Q_PROPERTY(QJsonArray initData READ getInitData WRITE setInitData NOTIFY initDataChanged)

public:
    explicit PathSettingTable(QObject *parent = nullptr);

    QStringList getHorHeader() const;
    void setHorHeader(const QStringList &header);

    QJsonArray getInitData() const;
    void setInitData(const QJsonArray &jsonArr);

    // QQmlParserStatus：构造前
    void classBegin() override;
    // QQmlParserStatus：构造后
    void componentComplete() override;
    // 自定义role
    QHash<int,QByteArray> roleNames() const override;

    // 表头
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // 数据，这三个必须实现
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 编辑
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

signals:
    void horHeaderChanged();
    void initDataChanged();
private:
    // 组件是否初始化完成
    bool _completed=false;
    QJsonArray _initData;
    // 数据，我一般纯展示，用vector就行了
    QVector<QVector<QVariant>> _modelData;
    // 横项表头
    QList<QString> _horHeaderList;

public slots:
    void clearTable();
    void refreshData(QJsonArray data);
    void addPathVelocity();
    QString getCurrentJsonData();
};

#endif // PATHSETTINGTABLE_H
