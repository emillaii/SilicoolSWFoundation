#include "PointsTableModel.h"
#include <QDebug>
#include <QImageReader>
#include <QJsonDocument>

PointsTableModel::PointsTableModel(QObject *parent) : QAbstractTableModel(parent) {}

QStringList PointsTableModel::getHorHeader() const
{
    return _horHeaderList;
}

void PointsTableModel::setHorHeader(const QStringList &header)
{
    _horHeaderList = header;
    emit horHeaderChanged();
}

void PointsTableModel::classBegin()
{
    // qDebug()<<"EasyTableModel::classBegin()";
}

void PointsTableModel::componentComplete()
{
    // qDebug()<<"EasyTableModel::componentComplete()";
    _completed = true;
}

QHash<int, QByteArray> PointsTableModel::roleNames() const
{
    // value表示取值，edit表示编辑
    return QHash<int, QByteArray>{ { Qt::DisplayRole, "value" }, { Qt::EditRole, "edit" } };
}

QVariant PointsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //返回表头数据，无效的返回None
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return _horHeaderList.value(section, QString::number(section));
        }
        else if (orientation == Qt::Vertical)
        {
            return QString::number(section);
        }
    }
    return QVariant();
}

bool PointsTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role))
    {
        if (orientation == Qt::Horizontal && role == Qt::EditRole)
        {
            _horHeaderList[section] = value.toString();
            emit headerDataChanged(orientation, section, section);
            return true;
        }
    }
    return false;
}

int PointsTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _modelData.count();
}

int PointsTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return _horHeaderList.count();
}

QVariant PointsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return _modelData.at(index.row()).at(index.column());
        default:
            break;
    }
    return QVariant();
}

bool PointsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (value.isValid() && index.isValid() && (data(index, role) != value))
    {
        if (Qt::EditRole == role)
        {
            _modelData[index.row()][index.column()] = value;
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags PointsTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QJsonArray PointsTableModel::getInitData() const
{
    return _initData;
}

void PointsTableModel::setInitData(const QJsonArray &jsonArr)
{
    _initData = jsonArr;
}

QJsonArray PointsTableModel::getVelocityData() const
{
    return _velocityData;
}

void PointsTableModel::setVelocityData(const QJsonArray &jsonArr)
{
    _velocityData = jsonArr;
}

void PointsTableModel::loadData(const QJsonArray &data)
{
    //如果要区分类型的话，可以用role，
    //这样ui中就能使用model.role来获取对应index的参数
    QVector<QVector<QVariant>> newData;
    QJsonArray::const_iterator iter;
    int index = 0;
    for (iter = data.begin(); iter != data.end(); ++iter)
    {
        QString dataZ = _modelData[index][2].toString();
        QVector<QVariant> newRow;
        const QJsonObject itemRow = (*iter).toObject();
        QString x = itemRow["x"].toString();
        QString y = itemRow["y"].toString();
        QString z = dataZ;
        if (x.isEmpty())
        {
            x = QString::number(itemRow["x"].toDouble());
        }
        if (y.isEmpty())
        {
            y = QString::number(itemRow["y"].toDouble());
        }
        newRow.append(x);
        newRow.append(y);
        newRow.append(z);
        newRow.append(itemRow.value("type"));
        newData.append(newRow);
        index++;
    }
    emit beginResetModel();
    _modelData = newData;
    emit endResetModel();
}

void PointsTableModel::clearTable()
{
    emit beginResetModel();
    _modelData.clear();
    emit endResetModel();
}

void PointsTableModel::addPoint(double x, double y, QString type)
{
    qInfo("Input point x: %f y: %f", x, y);
    QVector<QVariant> newRow;
    newRow.append(x);
    newRow.append(y);
    newRow.append(0);
    newRow.append(type);
    emit beginResetModel();
    _modelData.append(newRow);
    emit endResetModel();
    // Signal velocity table to add new path setting
    if (_modelData.length() > 1)
    {
        emit newPathAdded();
    }
}

void PointsTableModel::updateData(int row, int col, double newValue)
{
    qInfo("Update data row: %d col: %d value: %f", row, col, newValue);
    qInfo("Ori value: %f", _modelData[row][col].toDouble());
}

void PointsTableModel::saveOutputJson(QString filename)
{
    filename.replace("file:///", "");
    filename.replace(".json", "");
    filename.append(".json");
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QJsonObject outputJsonObj;
    QJsonArray *pointList = new QJsonArray();
    for (int i = 0; i < _modelData.length(); i++)
    {
        QJsonObject obj;
        obj["x"] = _modelData[i].at(0).toDouble();
        obj["y"] = _modelData[i].at(1).toDouble();
        obj["z"] = _modelData[i].at(2).toDouble();
        obj["type"] = _modelData[i].at(3).toString();
        pointList->append(obj);
    }

    outputJsonObj["points"] = *pointList;

    QJsonDocument saveDoc(outputJsonObj);
    file.write(saveDoc.toJson());
    file.close();
}

void PointsTableModel::loadJson(QString filename)
{
    filename.replace("file:///", "");
    qInfo("load json: %s", filename.toStdString().c_str());
    QString settings;
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();

    QJsonObject jsonObj = QJsonDocument::fromJson(settings.toUtf8()).object();
    QJsonArray jsonArray = jsonObj["points"].toArray();

    emit beginResetModel();
    _modelData.clear();
    for (QJsonValue json : jsonArray)
    {
        double x = json["x"].toDouble();
        double y = json["y"].toDouble();
        double z = json["z"].toDouble();
        QString type = json["type"].toString();
        QVector<QVariant> newRow;
        newRow.append(x);
        newRow.append(y);
        newRow.append(0);
        newRow.append(type);
        _modelData.append(newRow);
    }

    setInitData(jsonArray);

    emit endResetModel();
    emit dataLoaded();
}

void PointsTableModel::refreshData(QJsonArray data)
{
    loadData(data);
}

QJsonObject PointsTableModel::checkImageSize(QString filename)
{
    QJsonObject object;
    filename.replace("file:///", "");
    QImageReader reader(filename);
    QSize sizeOfImage = reader.size();
    int height = sizeOfImage.height();
    int width = sizeOfImage.width();
    qInfo("load image: %s height: %d width: %d", filename.toStdString().c_str(), height, width);
    object["width"] = sizeOfImage.width();
    object["height"] = sizeOfImage.height();
    return object;
}
