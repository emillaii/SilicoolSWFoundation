#include "hikvision.h"

SILICOOL_CREATE_LOGGING_CATEGORY(hikCate, "HikVision")

HikVision::HikVision(QObject *parent)
{
    hikVisionConfig = new HikVisionConfig(this);
    hikVisionConfigFile = new ConfigFile("HikVisionConfig", hikVisionConfig, "./config/hikVisionConfig.json", false);
    hikVisionConfigFile->populate(true);
}

HikVision::~HikVision() {}

HikVisionConfig *HikVision::config() const
{
    return hikVisionConfig;
}

void HikVision::initImpl()
{
    if (m_handle != nullptr)
    {
        IMVS_PF_DestroyHandle(m_handle);
        m_handle = IMVS_NULL;
    }

    CHECK_MVS_RES(IMVS_PF_CreateHandle(&m_handle, hikVisionConfig->visionServerPath().toUtf8()));
    CHECK_MVS_RES(IMVS_PF_RegisterResultCallBack_V30(m_handle, callBackModuRes, this));
    loadSolution();
}

void HikVision::disposeImpl()
{
    if (m_handle != nullptr)
    {
        IMVS_PF_RegisterResultCallBack_V30(m_handle, nullptr, this);
        IMVS_PF_CloseSolution(m_handle);
        IMVS_PF_CloseVisionMaster(m_handle);
        IMVS_PF_DestroyHandle(m_handle);
    }
}

bool HikVision::performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
{
    HikVisionLocationConfig *hikPrConfig = qobject_cast<HikVisionLocationConfig *>(prConfig);
    SC_ASSERT(hikPrConfig != nullptr);
    SC_ASSERT(image.format() == QImage::Format_Indexed8);


    IMVS_PF_INPUT_IMAGE_INFO stImageData;
    stImageData.nDataType = 0;
    stImageData.nModuleID = hikPrConfig->imageSourceModuleId();
    stImageData.stImageDataInfo.pImgData = (char *)image.bits();
    stImageData.stImageDataInfo.iImgDataLen = (int)image.sizeInBytes();
    stImageData.stImageDataInfo.iWidth = image.width();
    stImageData.stImageDataInfo.iHeight = image.height();
    stImageData.stImageDataInfo.iImgFormat = 1;

    int nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    if (IMVS_EC_OK != nRet)
    {
        qCCritical(hikCate()) << prConfig->locationName() << "IMVS_PF_SetImageData Error, error code:" << QString::number(nRet, 16);
        return false;
    }

    nRet = IMVS_PF_ExecuteOnce_V30(m_handle, hikPrConfig->processId(), nullptr);
    if (IMVS_EC_OK != nRet)
    {
        qCCritical(hikCate()) << prConfig->locationName() << "IMVS_PF_ExecuteOnce_V30 Error, error code:" << QString::number(nRet, 16);
        return false;
    }

    auto hikResult = new HikVisionResult();
    hikResult->resultModuleId = hikPrConfig->resultModuleId();
    auto hikResultImageInfo = new HikVisionResultImageInfo();
    (*resultImageInfo) = hikResultImageInfo;
    hikResult->resultImageInfo = hikResultImageInfo;
    AutoDeleter<HikVisionResult> t(hikResult);
    {
        QMutexLocker l(&hikResultsLocker);
        hikResults[hikPrConfig->processId()] = hikResult;
    }

    bool waitResult;
    {
        QMutex resultLocker;
        QMutexLocker l(&resultLocker);
        waitResult = hikResult->waiter.wait(&resultLocker, 3000);
    }
    {
        QMutexLocker l(&hikResultsLocker);
        hikResults.remove(hikPrConfig->processId());
    }

    if(!waitResult)
    {
        qCCritical(hikCate()) << prConfig->locationName() << "wait pr result timeout!";
        return false;
    }
    if(!hikResult->errString.isEmpty())
    {
        qCCritical(hikCate()) << prConfig->locationName() << hikResult->errString;
        return false;
    }

    prResult.x = hikResult->x;
    prResult.y = hikResult->y;
    prResult.theta = hikResult->theta;
    prResult.width = hikResult->width;
    prResult.height = hikResult->height;
    return true;
}

void HikVision::loadSolution()
{
    try
    {
        QString solutionPath = QDir(VisionConfigDir::getIns().dutRelatedConfigDir()).absoluteFilePath("HikSolution.sol");
        qDebug() << "Load solution..." << solutionPath;
        CHECK_MVS_RES(IMVS_PF_SyncLoadSolution(m_handle, solutionPath.toUtf8(), "", false));
    } catch (SilicoolException& se)
    {
        qCritical() << se.what();
    }
}

void HikVision::startVisionMaster(bool start)
{
    if(start)
    {
        CHECK_MVS_RES(IMVS_PF_StartVisionMaster(m_handle, hikVisionConfig->visionMasterPath().toUtf8(), 10000));
    }
    else
    {
        CHECK_MVS_RES(IMVS_PF_CloseVisionMaster(m_handle));
    }
}

void HikVision::showVisionMaster(bool show)
{
    CHECK_MVS_RES(IMVS_PF_ShowVisionMaster(m_handle, show ? 1 : 0));
}

void HikVision::printProcessModuleIds()
{
    IMVS_PF_PROCESS_INFO_LIST processInfos;
    CHECK_MVS_RES(IMVS_PF_GetAllProcessList(m_handle, &processInfos));
    qDebug() << "Process count:" << processInfos.nNum;
    for(int i = 0; i < processInfos.nNum; i++)
    {
        IMVS_PF_PROCESS_INFO* processInfo = &processInfos.astProcessInfo[i];
        qDebug() << "Process" << processInfo->nProcessID << "name" << processInfo->strProcessName;

        IMVS_PF_MODULE_INFO_LIST moduleInfos;
        CHECK_MVS_RES(IMVS_PF_GetModulesByProcessId(m_handle, processInfo->nProcessID, &moduleInfos));
        qDebug() << "Module count:" << moduleInfos.nNum;

        for (int j = 0; j < moduleInfos.nNum; j++)
        {
            IMVS_PF_MODULE_INFO* moduleInfo = &moduleInfos.astModuleInfo[j];
            qDebug() << "process" << moduleInfo->nProcessID << "module id" << moduleInfo->nModuleID
                     << "moduleName" << moduleInfo->strModuleName;
        }
    }
}

int HikVision::callBackModuRes(IN IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo, IN void *const pUser)
{
    HikVision *pCtrlDemoThis = (HikVision *)pUser;
    if (pCtrlDemoThis == IMVS_NULL)
    {
        qCritical() << "pCtrlDemoThis is null!";
        return IMVS_EC_NULL_PTR;
    }
    if (pstInputPlatformInfo == IMVS_NULL)
    {
        qCritical() << "pstInputPlatformInfo is null!";
        return IMVS_EC_NULL_PTR;
    }
    if (IMVS_NULL == (pstInputPlatformInfo->pData))
    {
        qCritical() << "pstInputPlatformInfo->pData is null!";
        return IMVS_EC_NULL_PTR;
    }
    return pCtrlDemoThis->callBackModuResFunc(pstInputPlatformInfo);
}

int HikVision::callBackModuResFunc(IN IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo)
{
//    if(pstInputPlatformInfo->nInfoType != 7)
//    {
//        qDebug() << "CallBackModuResFunc" << pstInputPlatformInfo->nInfoType;
//    }
//    if(IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_WORK_STATE == pstInputPlatformInfo->nInfoType)
//    {
//        IMVS_PF_MODULE_WORK_STAUS__ *info = (IMVS_PF_MODULE_WORK_STAUS__ *)pstInputPlatformInfo->pData;
//        qDebug() << "nWorkStatus" << info->nWorkStatus <<"nProcessID" <<  info->nProcessID;
//    }


    if(IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_STOP == pstInputPlatformInfo->nInfoType)
    {
        IMVS_PF_STATUS_STOP_INFO__ *info = (IMVS_PF_STATUS_STOP_INFO__ *)pstInputPlatformInfo->pData;

        QMutexLocker l(&hikResultsLocker);
        if(hikResults.contains(info->nProcessID))
        {
            HikVisionResult* hikResult = hikResults[info->nProcessID];
            hikResult->errString = "Process stopped!";
            hikResult->waiter.wakeAll();
        }
        return IMVS_EC_OK;
    }

    if (IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_MODULE_RESULT == pstInputPlatformInfo->nInfoType)
    {
        IMVS_PF_MODU_RES_INFO *info = (IMVS_PF_MODU_RES_INFO *)pstInputPlatformInfo->pData;

        QMutexLocker l(&hikResultsLocker);
        if(!hikResults.contains(info->nProcessID))
        {
            return IMVS_EC_OK;
        }
        auto hikResult = hikResults[info->nProcessID];
        if(info->nStatus != 1)
        {
            hikResult->errString = QString("%1 failed! Error code: %2").arg(info->strModuleName).arg(QString::number(info->nErrorCode, 16));
            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }
        if(info->nModuleID != hikResult->resultModuleId)
        {
            return IMVS_EC_OK;
        }

        if(strcmp(info->strModuleName, MODU_NAME_HPFEATUREMATCHMODU) == 0)
        {
            IMVS_PF_HPFEATUREMATCH_MODU_INFO__* hpMathInfo = (IMVS_PF_HPFEATUREMATCH_MODU_INFO__*) info->pData;

            if(hpMathInfo->iMatchNum < 1 || hpMathInfo->iModuStatu != 1)
            {
                hikResult->errString = "feature match failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            auto matchBaseInfo = hpMathInfo->pstMatchBaseInfo[0];
            hikResult->theta = matchBaseInfo.stMatchBox.fAngle;
            hikResult->x = matchBaseInfo.stMatchPt.stMatchPt.fPtX;
            hikResult->y = matchBaseInfo.stMatchPt.stMatchPt.fPtY;

            // set hik  hikResult->resultImageInfo
            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }

        if(strcmp(info->strModuleName, MODU_NAME_FASTFEATUREMATCHMODU) == 0)
        {
            IMVS_PF_FASTFEATUREMATCH_MODU_INFO__* fMathInfo = (IMVS_PF_FASTFEATUREMATCH_MODU_INFO__*)info->pData;

            if(fMathInfo->iMatchNum < 1 || fMathInfo->iModuStatu != 1)
            {
                hikResult->errString = "feature match failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            auto matchBaseInfo = fMathInfo->pstMatchBaseInfo[0];
            hikResult->theta = matchBaseInfo.stMatchBox.fAngle;
            hikResult->x = matchBaseInfo.stMatchPt.stMatchPt.fPtX;
            hikResult->y = matchBaseInfo.stMatchPt.stMatchPt.fPtY;

            // set hik  hikResult->resultImageInfo
            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }

        if(strcmp(info->strModuleName, MODU_NAME_QUADRANGEFINDMODULE) == 0)
        {
            IMVS_PF_QUADRANGEFIND_MODU_INFO* rectInfo = (IMVS_PF_QUADRANGEFIND_MODU_INFO*)  info->pData;

            if(rectInfo->iModuStatu != 1)
            {
                hikResult->errString = "match rectangle failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            hikResult->x = rectInfo->stDiagIntersectionPt.fPtX;
            hikResult->y = rectInfo->stDiagIntersectionPt.fPtY;
            hikResult->theta = calcAngle(rectInfo->stEdgeLine0);

            // set hik  hikResult->resultImageInfo
            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }
    }
    return IMVS_EC_OK;
}

int HikVision::copyModuResultByModu(IN IMVS_PF_MODU_RES_INFO *const pstPFModuResInfoList)
{
    if (0 == strcmp(MODU_NAME_SAVEIMAGE, (pstPFModuResInfoList->strModuleName)))
    {
        if (0 == pstPFModuResInfoList->nModuleID)
        {

            IMVS_PF_SAVEIMAGE_MODU_INFO__ *pOutPutImageModuRes = (IMVS_PF_SAVEIMAGE_MODU_INFO__ *)pstPFModuResInfoList->pData;

            int callBackImgFormate = pOutPutImageModuRes->stOutImgInfo.iImgFormat;
            auto qImgFormate = (callBackImgFormate == IMVS_PF_IMG_FORMAT_MONO8) ? QImage::Format_Indexed8 : QImage::Format_RGB888;
            QImage image(pOutPutImageModuRes->stOutImgInfo.iWidth, pOutPutImageModuRes->stOutImgInfo.iHeight, qImgFormate);

            qDebug() << "hik image info" << pOutPutImageModuRes->stOutImgInfo.iWidth << pOutPutImageModuRes->stOutImgInfo.iHeight
                     << pOutPutImageModuRes->stOutImgInfo.iImgDataLen;

            qDebug() << "image info before load" << image.width() << image.height() << image.format() << image.byteCount() << image.bytesPerLine();

            if (image.bytesPerLine() == image.width() * 3)
            {
                qDebug() << "copy 1";
                memcpy(image.bits(), pOutPutImageModuRes->stOutImgInfo.pImgData, pOutPutImageModuRes->stOutImgInfo.iImgDataLen);
            }
            else
            {
                qDebug() << "copy 2";
                int bytesPerLine = image.width() * 3;
                for (int i = 0; i < image.height(); i++)
                {
                    memcpy(image.scanLine(i), pOutPutImageModuRes->stOutImgInfo.pImgData + i * bytesPerLine, bytesPerLine);
                }
            }
        }
    }
    else if (0 == strcmp(MODU_NAME_HPFEATUREMATCHMODU, (pstPFModuResInfoList->strModuleName)))
    {
        if (2 == pstPFModuResInfoList->nModuleID)
        {
            qDebug() << "get PR result Process1" << QDateTime::currentMSecsSinceEpoch();

            IMVS_PF_HPFEATUREMATCH_MODU_INFO *pstHPFeatureMatchModuRes = (IMVS_PF_HPFEATUREMATCH_MODU_INFO *)pstPFModuResInfoList->pData;
            m_nMatchPtNum = pstHPFeatureMatchModuRes->iMatchNum;
            for (int i = 0; i < (int)m_nMatchPtNum; i++)
            {
            }
        }
    }

    return 0;
}
