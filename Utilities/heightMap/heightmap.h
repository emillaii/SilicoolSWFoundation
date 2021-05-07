#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "errorHandling/scassert.h"
#include "utilities_global.h"
#include <QAbstractTableModel>
#include <QObject>

struct HeightStruct
{
    double rawHeight = 0;
    bool isValid = false;
    double processedHeight = 0;
};

enum HeightStructRole
{
    RawHeight = Qt::UserRole + 1,
    IsValid,
    ProcessedHeight
};

class UTILITIESSHARED_EXPORT HeightMap : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(int minValueRowIndex READ minValueRowIndex WRITE setMinValueRowIndex NOTIFY minValueRowIndexChanged)
    Q_PROPERTY(int minValueColIndex READ minValueColIndex WRITE setMinValueColIndex NOTIFY minValueColIndexChanged)
    Q_PROPERTY(double minValue READ minValue WRITE setMinValue NOTIFY minValueChanged)
    Q_PROPERTY(int maxValueRowIndex READ maxValueRowIndex WRITE setMaxValueRowIndex NOTIFY maxValueRowIndexChanged)
    Q_PROPERTY(int maxValueColIndex READ maxValueColIndex WRITE setMaxValueColIndex NOTIFY maxValueColIndexChanged)
    Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)

public:
    explicit HeightMap(QObject *parent = nullptr);

    ~HeightMap() override;

    void deleteMap();

    void setDataProcessParam(double targetRange, double offset);

    void reset(int rows, int columns);

    void setHeight(int rowIndex, int columnIndex, double value);

    void save(QString fileName);

    int minValueRowIndex() const
    {
        return m_minValueRowIndex;
    }

    int minValueColIndex() const
    {
        return m_minValueColIndex;
    }

    double minValue() const
    {
        return m_minValue;
    }

    int maxValueRowIndex() const
    {
        return m_maxValueRowIndex;
    }

    int maxValueColIndex() const
    {
        return m_maxValueColIndex;
    }

    double maxValue() const
    {
        return m_maxValue;
    }

public:
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setMinValueRowIndex(int minValueRowIndex)
    {
        if (m_minValueRowIndex == minValueRowIndex)
            return;

        m_minValueRowIndex = minValueRowIndex;
        emit minValueRowIndexChanged(m_minValueRowIndex);
    }

    void setMinValueColIndex(int minValueColIndex)
    {
        if (m_minValueColIndex == minValueColIndex)
            return;

        m_minValueColIndex = minValueColIndex;
        emit minValueColIndexChanged(m_minValueColIndex);
    }

    void setMinValue(double minValue)
    {
        if (qFuzzyCompare(m_minValue, minValue))
            return;

        m_minValue = minValue;
        emit minValueChanged(m_minValue);
    }

    void setMaxValueRowIndex(int maxValueRowIndex)
    {
        if (m_maxValueRowIndex == maxValueRowIndex)
            return;

        m_maxValueRowIndex = maxValueRowIndex;
        emit maxValueRowIndexChanged(m_maxValueRowIndex);
    }

    void setMaxValueColIndex(int maxValueColIndex)
    {
        if (m_maxValueColIndex == maxValueColIndex)
            return;

        m_maxValueColIndex = maxValueColIndex;
        emit maxValueColIndexChanged(m_maxValueColIndex);
    }

    void setMaxValue(double maxValue)
    {
        if (qFuzzyCompare(m_maxValue, maxValue))
            return;

        m_maxValue = maxValue;
        emit maxValueChanged(m_maxValue);
    }

signals:
    void minValueRowIndexChanged(int minValueRowIndex);

    void minValueColIndexChanged(int minValueColIndex);

    void minValueChanged(double minValue);

    void maxValueRowIndexChanged(int maxValueRowIndex);

    void maxValueColIndexChanged(int maxValueColIndex);

    void maxValueChanged(double maxValue);

signals:
    void reqReset(int rows, int columns);
    void reqSetHeight(int rowIndex, int columnIndex, double value);
    void reqSave(QString fileName);
    void reqSaveToImage(QString imageName);

private slots:
    void onReqReset(int rows, int columns);
    void onReqSetHeight(int rowIndex, int columnIndex, double value);
    void onReqSave(QString fileName);

private:
    int m_rows = 0;
    int m_columns = 0;
    HeightStruct **m_heightMap = nullptr;

    QHash<int, QByteArray> roles;
    QVector<int> heightRoles;
    QVector<int> proccessedHeightRoles;

    double m_targetRange = 1;
    double m_offset = 0;

    bool isFirstData = true;

    int m_minValueRowIndex = -1;
    int m_minValueColIndex = -1;
    double m_minValue = 9999999;
    int m_maxValueRowIndex = -1;
    int m_maxValueColIndex = -1;
    double m_maxValue = -9999999;
};

#endif    // HEIGHTMAP_H
