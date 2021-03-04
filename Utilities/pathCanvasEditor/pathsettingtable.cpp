#include "pathsettingtable.h"
#include <QDebug>
#include <QJsonDocument>

PathSettingTable::PathSettingTable(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QStringList PathSettingTable::getHorHeader() const
{
    return _horHeaderList;
}

void PathSettingTable::setHorHeader(const QStringList &header)
{
    _horHeaderList=header;
    emit horHeaderChanged();
}

void PathSettingTable::classBegin()
{
}

void PathSettingTable::componentComplete()
{
    _completed=true;
}

QHash<int, QByteArray> PathSettingTable::roleNames() const
{
    //value表示取值，edit表示编辑
    return QHash<int,QByteArray>{
        { Qt::DisplayRole,"value" },
        { Qt::EditRole,"edit" }
    };
}

QVariant PathSettingTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    //返回表头数据，无效的返回None
    if(role==Qt::DisplayRole){
        if(orientation==Qt::Horizontal){
            return _horHeaderList.value(section,QString::number(section));
        }else if(orientation==Qt::Vertical){
            return QString::number(section);
        }
    }
    return QVariant();
}

bool PathSettingTable::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        if(orientation==Qt::Horizontal&&role==Qt::EditRole){
            _horHeaderList[section]=value.toString();
            emit headerDataChanged(orientation, section, section);
            return true;
        }
    }
    return false;
}


int PathSettingTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _modelData.count();
}

int PathSettingTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _horHeaderList.count();
}

QVariant PathSettingTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return _modelData.at(index.row()).at(index.column());
    default:
        break;
    }
    return QVariant();
}

bool PathSettingTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (value.isValid()&&index.isValid()&&(data(index, role) != value)) {
        if(Qt::EditRole==role){
            _modelData[index.row()][index.column()]=value;
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags PathSettingTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
}


QJsonArray PathSettingTable::getInitData() const
{
    return _initData;
}

void PathSettingTable::setInitData(const QJsonArray &jsonArr)
{
    _initData=jsonArr;
}

void PathSettingTable::clearTable()
{
    emit beginResetModel();
    _modelData.clear();
    emit endResetModel();
}

void PathSettingTable::addPathVelocity()
{
    qInfo("Add path velocity");
    QVector<QVariant> newRow;
    newRow.append(10);  //Default velocity
    emit beginResetModel();
    _modelData.append(newRow);
    emit endResetModel();
}

QString PathSettingTable::getCurrentJsonData()
{
    QJsonArray *pointList = new QJsonArray();
    for (int i = 0; i < _modelData.length(); i++) {
        QJsonObject obj;
        obj["v"] = _modelData[i].at(0).toDouble();
        pointList->append(obj);
    }
    QJsonDocument saveDoc(*pointList);
    return saveDoc.toJson();
}

void PathSettingTable::refreshData(QJsonArray data){
    QVector<QVector<QVariant>> newData;
    QJsonArray::const_iterator iter;
    for(iter=data.begin();iter!=data.end();++iter){
        QVector<QVariant> newRow;
        const QJsonObject itemRow=(*iter).toObject();
        double v = itemRow["v"].toDouble();
        newRow.append(v);
        newData.append(newRow);
    }
    emit beginResetModel();
    _modelData=newData;
    emit endResetModel();
}
