#ifndef HIKVISION_H
#define HIKVISION_H

#include "configManager/configfile.h"
#include "deleter.h"
#include "errorHandling/scassert.h"
#include "hikvision_global.h"
#include "hikvisionconfig.h"
#include "hikvisionlocationconfig.h"
#include "hikvisionresult.h"
#include "hikvisionresultimageinfo.h"
#include "iMVS-6000PlatformSDKC.h"
#include "loghelper.h"
#include "scvision.h"
#include "visionconfigdir.h"
#include <QPainter>
#include <QThread>
#include <QWaitCondition>

#define CHECK_MVS_RES(calling)                                                                                                                       \
    {                                                                                                                                                \
        int ret = calling;                                                                                                                           \
        if (ret != IMVS_EC_OK)                                                                                                                       \
        {                                                                                                                                            \
            throw SilicolAbort(                                                                                                                      \
                QString("%1 failed! ErrorCode: %2, ErrorString: %3").arg(#calling).arg(QString::number(ret, 16)).arg(IMVS_PF_GetErrorMsg(ret)));     \
        }                                                                                                                                            \
    }

SILICOOL_DECLARE_LOGGING_CATEGORY(hikCate, )

class HIKVISIONSHARED_EXPORT HikVision : public SCVision
{
    Q_OBJECT

public:
    explicit HikVision(QObject *parent = nullptr);
    ~HikVision() override;
    HikVisionConfig *config() const;

    // SCVision interface
public:
    void initImpl() override;
    void disposeImpl() override;
    void handleDutTypeChanged() override;

    bool performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult);

    bool performPr(QImage &image,
                   VisionLocationConfig *prConfig,
                   PRResultImageInfo **resultImageInfo,
                   PRResultStruct &prResult,
                   int roiRowIndex,
                   int rowColIndex)
    {
        throw SilicolAbort("Unimplemented function: performPr");
    }
    void drawResultImage(QImage &image, PRResultImageInfo *resultImageInfo) override;
    bool glueCheck(QImage &imageBefore,
                   QImage &imageAfter,
                   double resoultion,
                   QImage &resultImage,
                   double &outMinGlueWidth,
                   double &outMaxGlueWidth,
                   double &outMaxAvgGlueWidth)
    {
        // TBD
        return true;
    }
    QVector<PathEndPoint> readDispensePath(QString fileName)
    {
        // TBD
        return QVector<PathEndPoint>();
    }
    bool checkDongle()
    {
        // TBD
        return true;
    }

public slots:
    void loadSolution();
    void startVisionMaster(bool start);
    void showVisionMaster(bool show);
    void printProcessModuleIds();

private:
    static int callBackModuRes(IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo, void *const pUser);
    int callBackModuResFunc(IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo);
    int copyModuResultByModu(IMVS_PF_MODU_RES_INFO *const pstPFModuResInfoList);
    int copyModuResult(IMVS_PF_MODULE_RESULT_INFO_LIST *const pstPFModuResInfoList);

private:
    double calcAngle(const IMVS_PF_LINE_INFO &line)
    {
        double radian = qAtan((line.stEndPt.fPtY - line.stStartPt.fPtY) / (line.stEndPt.fPtX - line.stStartPt.fPtX));
        return radian / M_PI * 180;
    }

private:
    void *m_handle = nullptr;
    unsigned int m_nMatchPtNum;

    const int LineLen = 60;
    const int LineWidth = 5;

    bool isOpenVisionMaster = false;

    QMap<int, HikVisionResult *> hikResults;
    QMutex hikResultsLocker;
    ConfigFile *hikVisionConfigFile;
    HikVisionConfig *hikVisionConfig;
};

#endif    // HIKVISION_H
