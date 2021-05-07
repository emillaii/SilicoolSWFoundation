#include "heightmap.h"

HeightMap::HeightMap(QObject *parent) : QAbstractTableModel(parent)
{
    roles[RawHeight] = "rawHeight";
    roles[IsValid] = "isValid";
    roles[ProcessedHeight] = "processedHeight";

    heightRoles << RawHeight << IsValid;
    proccessedHeightRoles << ProcessedHeight;

    connect(this, &HeightMap::reqReset, this, &HeightMap::onReqReset);
    connect(this, &HeightMap::reqSetHeight, this, &HeightMap::onReqSetHeight);
    connect(this, &HeightMap::reqSave, this, &HeightMap::onReqSave);
}

void HeightMap::setDataProcessParam(double targetRange, double offset)
{
    m_targetRange = targetRange;
    m_offset = offset;
}

void HeightMap::reset(int rows, int columns)
{
    setMinValue(9999999);
    setMaxValue(-9999999);
    setMinValueRowIndex(-1);
    setMinValueColIndex(-1);
    setMaxValueRowIndex(-1);
    setMaxValueColIndex(-1);

    emit reqReset(rows, columns);
}

void HeightMap::setHeight(int rowIndex, int columnIndex, double value)
{
    SC_ASSERT(rowIndex < m_rows)
    SC_ASSERT(columnIndex < m_columns)

    emit reqSetHeight(rowIndex, columnIndex, value);
}

void HeightMap::save(QString fileName)
{
    emit reqSave(fileName);
}

int HeightMap::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rows;
}

int HeightMap::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columns;
}

QVariant HeightMap::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < m_rows && index.column() >= 0 && index.column() < m_columns)
    {
        if (role == RawHeight)
        {
            return m_heightMap[index.row()][index.column()].rawHeight;
        }
        else if (role == IsValid)
        {
            return m_heightMap[index.row()][index.column()].isValid;
        }
        else if (role == ProcessedHeight)
        {
            return m_heightMap[index.row()][index.column()].processedHeight;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> HeightMap::roleNames() const
{
    return roles;
}

void HeightMap::onReqReset(int rows, int columns)
{
    isFirstData = true;

    beginResetModel();

    deleteMap();
    m_heightMap = new HeightStruct *[rows];
    for (int i = 0; i < rows; i++)
    {
        m_heightMap[i] = new HeightStruct[columns];
        memset(m_heightMap[i], 0, sizeof(HeightStruct) * columns);
    }
    m_rows = rows;
    m_columns = columns;

    endResetModel();
}

void HeightMap::onReqSetHeight(int rowIndex, int columnIndex, double value)
{
    m_heightMap[rowIndex][columnIndex].rawHeight = value;
    m_heightMap[rowIndex][columnIndex].isValid = true;

    auto thisIndex = index(rowIndex, columnIndex);

    emit dataChanged(thisIndex, thisIndex, heightRoles);

    bool minMaxChanged = false;

    if (value < m_minValue)
    {
        setMinValue(value);
        setMinValueRowIndex(rowIndex);
        setMinValueColIndex(columnIndex);
        minMaxChanged = true;
    }
    if (value > m_maxValue)
    {
        setMaxValue(value);
        setMaxValueRowIndex(rowIndex);
        setMaxValueColIndex(columnIndex);
        minMaxChanged = true;
    }

    if (isFirstData)
    {
        m_heightMap[rowIndex][columnIndex].processedHeight = m_targetRange / 2 + m_offset;
        isFirstData = false;
    }
    else
    {
        m_heightMap[rowIndex][columnIndex].processedHeight = (value - m_minValue) / (m_maxValue - m_minValue) * m_targetRange + m_offset;
    }
    emit dataChanged(thisIndex, thisIndex, proccessedHeightRoles);

    if (minMaxChanged)
    {
        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_columns; j++)
            {
                if (i == rowIndex && j == columnIndex)
                {
                    continue;
                }
                HeightStruct *heightStruct = &m_heightMap[i][j];
                if (heightStruct->isValid)
                {
                    heightStruct->processedHeight = (value - m_minValue) / (m_maxValue - m_minValue) * m_targetRange + m_offset;
                    emit dataChanged(index(i, j), index(i, j), proccessedHeightRoles);
                }
            }
        }
    }
}

void HeightMap::onReqSave(QString fileName)
{
    QString csvFileName = fileName + ".csv";
    QFileInfo fileInfo(csvFileName);
    QDir dir;
    if (!dir.exists(fileInfo.absolutePath()))
    {
        dir.mkpath(fileInfo.absolutePath());
    }
    QFile file(csvFileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    for (int i = 0; i < m_rows; i++)
    {
        QByteArray line(m_columns * 20, 0);
        for (int j = 0; j < m_columns; j++)
        {
            auto heightStruct = &m_heightMap[i][j];
            if (heightStruct->isValid)
            {
                line.append(QString::number(heightStruct->rawHeight));
            }
            if (j != m_columns - 1)
            {
                line.append(",");
            }
        }
        line.append("\r\n");
        file.write(line);
    }
    file.flush();
    file.close();

    QString imageName = fileInfo.absoluteFilePath();
    imageName.replace(".csv", ".jpg");
    emit reqSaveToImage(imageName);
}

void HeightMap::deleteMap()
{
    if (m_heightMap != nullptr)
    {
        for (int i = 0; i < m_rows; i++)
        {
            delete[] m_heightMap[i];
        }
        delete[] m_heightMap;
        m_rows = 0;
        m_columns = 0;
        m_heightMap = nullptr;
    }
}
