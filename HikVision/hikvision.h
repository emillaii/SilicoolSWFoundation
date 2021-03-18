#ifndef HIKVISION_H
#define HIKVISION_H

#include "QThread"
#include "defineforhik.h"
#include "errorHandling/scassert.h"
#include "hikvision_global.h"
#include "hikvisionlocationconfig.h"
#include "hikvisionresultimageinfo.h"
#include "scvision.h"
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QtDebug>

class SCTimeSpent
{

public:
    SCTimeSpent()
    {
        time_Start = 0.0;
        time_End = 0.0;
    }
    void SC_StartClock()
    {
        time_Start = (double)clock();
    }

    void SC_EndClock(QString msg)
    {
        time_End = (double)clock();
        qDebug() << msg << (time_End - time_Start) / 1000.0 << "s";
    }

private:
    double time_Start;
    double time_End;
};

class HIKVISIONSHARED_EXPORT HikVision : public SCVision
{
    Q_OBJECT
public:
    explicit HikVision(QObject *parent = nullptr);
    ~HikVision();

    // SCVision interface
public:
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
    double getObjectSharpness(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo)
    {
        throw SilicolAbort("Unimplemented function: getObjectSharpness");
    }
    void drawResultImage(QImage &image, PRResultImageInfo *resultImageInfo)
    {
        HikVisionResultImageInfo *hikResultImageInfo = qobject_cast<HikVisionResultImageInfo *>(resultImageInfo);
        SC_ASSERT(hikResultImageInfo != nullptr);
        // TBD
    }
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

public:
    int SC_ShowModuleInterface(unsigned int nIndex);
    int SC_SaveSolution(QString nSolutionPath, QString nPassWord);
    int SC_ReloadSolution(QString nSolutionPath, QString nPassword);
    void SC_Close();

private:
    int Init();
    static int __stdcall CallBackModuRes(IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo, void *const pUser);
    int CallBackModuResFunc(IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo);
    int CopyModuResultByModu(IMVS_PF_MODU_RES_INFO *const pstPFModuResInfoList);
    int CopyModuResult(IMVS_PF_MODULE_RESULT_INFO_LIST *const pstPFModuResInfoList);
    int GetV32ResFromCallBack(IMVS_PF_MODULE_RESULT_INFO_LIST *const pstPFModuResInfoList);

private:
    void *m_handle = nullptr;
    unsigned int m_nMatchPtNum;
    CDefine nCDfine;
    SCTimeSpent mCTimeSpent;

    const QString mHikVisionMasterServerPath = "E:\\VisionMaster\\VisionMaster3.4.0\\Applications\\Server\\VisionMasterServer.exe";
    const QString mHikVisionMasterAppPath = "E:\\VisionMaster\\VisionMaster3.4.0\\Applications\\VisionMaster.exe";
    const QString mSolutionPath = "C:\\Users\\Aini\\Desktop\\VisionMasterLearning\\GetImageFromOutput.sol";    // MatchTemplateGray8.sol
};

#endif    // HIKVISION_H
