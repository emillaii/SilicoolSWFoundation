#ifndef ADAPTVISIONLOCATIONCONFIG_H
#define ADAPTVISIONLOCATIONCONFIG_H

#include "adaptvision_global.h"
#include "scvision.h"
#include "visionconfigdir.h"
#include <QFileInfo>

class ADAPTVISIONSHARED_EXPORT AdaptVisionLocationConfig : public VisionLocationConfig
{
    Q_OBJECT

public:
    enum PrType
    {
        Generic_NCC_Template_Matching,
        Edge_Template_Matching,
        Circle_Edge_Template_Matching
    };
    Q_ENUM(PrType)

    Q_PROPERTY(QString prFileName READ prFileName WRITE setPrFileName NOTIFY prFileNameChanged)
    Q_PROPERTY(PrType prType READ prType WRITE setPrType NOTIFY prTypeChanged)
    Q_PROPERTY(double objectScore READ objectScore WRITE setObjectScore NOTIFY objectScoreChanged)

public:
    Q_INVOKABLE AdaptVisionLocationConfig(QObject *parent = nullptr) : VisionLocationConfig(parent)
    {
        init();
    }

    QString prFileName() const
    {
        return m_prFileName;
    }

    PrType prType() const
    {
        return m_prType;
    }

    double objectScore() const
    {
        return m_objectScore;
    }

public slots:
    void setPrFileName(QString prFileName)
    {
        if (m_prFileName == prFileName)
            return;

        m_prFileName = prFileName;
        emit prFileNameChanged(m_prFileName);

        if (prFileName.endsWith(".avdata"))
        {
            QString resultImageName = m_prFileName;
            resultImageName.replace(".avdata", "_resultImage.jpg");
            QString resultImageFullPath = VisionConfigDir::getIns().prConfigDir() + resultImageName;
            if (QFile::exists(resultImageFullPath))
            {
                setPrResultImage(QString("file:///") + resultImageFullPath);
            }
            else
            {
                setPrResultImage("");
            }
            emit prResultImageChanged();
        }
    }

    void setPrType(PrType prType)
    {
        if (m_prType == prType)
            return;

        m_prType = prType;
        emit prTypeChanged(m_prType);
    }

    void setObjectScore(double objectScore)
    {
        if (qFuzzyCompare(m_objectScore, objectScore))
            return;

        m_objectScore = objectScore;
        emit objectScoreChanged(m_objectScore);
    }

signals:

    void prFileNameChanged(QString prFileName);

    void prTypeChanged(PrType prType);

    void objectScoreChanged(double objectScore);

private:
    QString m_prFileName;
    PrType m_prType{ Edge_Template_Matching };
    double m_objectScore = 0.7;
};

#endif    // ADAPTVISIONLOCATIONCONFIG_H
