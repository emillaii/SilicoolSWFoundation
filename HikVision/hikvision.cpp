#include "hikvision.h"
#include "QThread"
HikVision::HikVision(QObject *parent)
{
    Init();
}

HikVision::~HikVision()
{
    SC_Close();
}

int HikVision::Init()
{
    if (m_handle != nullptr)
    {
        qDebug() << "m_handle!=nullptr";
        IMVS_PF_DestroyHandle(m_handle);
        m_handle = IMVS_NULL;
    }

    int nRet = IMVS_PF_CreateHandle(&m_handle, mHikVisionMasterServerPath.toLatin1().data());
    qDebug() << "IMVS_PF_CreateHandle:" << nRet;
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_CreateHandle Error:" << nRet;
        return -1;
    }
    else
    {
        qDebug() << "IMVS_PF_CreateHandle OK:" << nRet;
    }
    // nRet = IMVS_PF_RegisterResultCallBack_V30(m_handle, CallBackModuRes, this);
    nRet = IMVS_PF_RegisterResultCallBack_V32(m_handle, CallBackModuRes, this);
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_RegisterResultCallBack_V30 Error" << nRet;
    }
    else
    {
        qDebug() << "IMVS_PF_RegisterResultCallBack_V30 OK" << nRet;
    }
    nRet = IMVS_PF_LoadSolution(m_handle, mSolutionPath.toUtf8(), "");    // Asynchronous
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_LoadSolution Error" << nRet;
        return -2;
    }
    else
    {
        qDebug() << "IMVS_PF_LoadSolution OK" << nRet;
    }
    QElapsedTimer qTimer;
    qTimer.start();
    unsigned int nLoadprogress = 0;
    while (nRet == IMVS_EC_OK)
    {
        nRet = IMVS_PF_GetLoadProgress(m_handle, &nLoadprogress);

        if (nLoadprogress == 100)
        {
            qDebug() << "SolutionHaveBeenLoaded OK" << nRet;
            break;
        }
        if (qTimer.elapsed() > 5000)
        {
            qDebug() << "SolutionHaveBeenLoaded TimeOut" << nRet;
            break;
        }
    }
    return 0;
}

void HikVision::SC_Close()
{
    IMVS_PF_RegisterResultCallBack_V32(m_handle, nullptr, this);
    IMVS_PF_CloseSolution(m_handle);    //
    IMVS_PF_DestroyHandle(m_handle);    //
}

bool HikVision::performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
{
    // load vision master
    IMVS_PF_PROCESS_INFO_LIST stProcInfoList = { 0 };
    IMVS_PF_INPUT_IMAGE_INFO stImageData;

    // 2.picture
    stImageData.nDataType = 0;
    stImageData.nModuleID = 1;
    stImageData.stImageDataInfo.pImgData = (char *)image.bits();    // pixArray.data();//
    stImageData.stImageDataInfo.iImgDataLen = (int)image.sizeInBytes();
    stImageData.stImageDataInfo.iWidth = image.width();
    stImageData.stImageDataInfo.iHeight = image.height();
    stImageData.stImageDataInfo.iImgFormat = 1;
    int nRet = IMVS_PF_SetImageData(m_handle, &stImageData);    //
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_SetImageData Error" << nRet;
    }
    else
    {
        qDebug() << "IMVS_PF_SetImageData OK" << nRet;
    }
    stImageData.nModuleID = 3;
    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    stImageData.nModuleID = 4;
    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);

    stImageData.nModuleID = 7;
    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    stImageData.nModuleID = 9;
    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    //    stImageData.nModuleID = 11;
    //    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    //    stImageData.nModuleID = 13;
    //    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    //    stImageData.nModuleID = 15;
    //    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    //    stImageData.nModuleID = 17;
    //    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    //    stImageData.nModuleID = 19;
    //    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);

    mCTimeSpent.SC_StartClock();
    // nRet = IMVS_PF_ExecuteOnce(m_handle, NULL);  //execute solution once

    nRet = IMVS_PF_ExecuteOnce_V30(m_handle, 10000, nullptr);    // excute process onece
    nRet = IMVS_PF_ExecuteOnce_V30(m_handle, 10001, nullptr);
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_ExecuteOnce Error" << nRet;
    }
    else
    {
        qDebug() << "IMVS_PF_ExecuteOnce OK" << nRet;
    }

    //    nRet = IMVS_PF_GetAllProcessList(m_handle, &stProcInfoList);
    //    qDebug()<<"-->Process count:"<<stProcInfoList.nNum;
    //    if (stProcInfoList.nNum > 0)
    //    {
    //        for (int iLop = 0; iLop < int(stProcInfoList.nNum); iLop++)
    //        {
    //          qDebug()<<"-->Process ID:"<<stProcInfoList.astProcessInfo[iLop].nProcessID;
    //          nRet = IMVS_PF_ExecuteOnce_V30(m_handle, stProcInfoList.astProcessInfo[iLop].nProcessID, nullptr);
    //        }
    //    }
    //    QFuture<void> future = QtConcurrent::run([=]() {
    //        int nRet = IMVS_PF_ExecuteOnce_V30(m_handle, 10000 /*stProcInfoList.astProcessInfo[0].nProcessID*/, nullptr);
    //        if (IMVS_EC_OK != nRet)
    //        {
    //            qDebug() << "1-IMVS_PF_ExecuteOnce_V30 Error" << nRet;
    //        }
    //        else
    //        {
    //            qDebug() << "1-IMVS_PF_ExecuteOnce_V30 OK" << nRet;
    //        }
    //        // Code in this block will run in another thread
    //    });
    //    future = QtConcurrent::run([=]() {
    //        int nRet = IMVS_PF_ExecuteOnce_V30(m_handle, 10001 /*stProcInfoList.astProcessInfo[1].nProcessID*/, nullptr);
    //        if (IMVS_EC_OK != nRet)
    //        {
    //            qDebug() << "2-IMVS_PF_ExecuteOnce_V30 Error" << nRet;
    //        }
    //        else
    //        {
    //            qDebug() << "2-IMVS_PF_ExecuteOnce_V30 OK" << nRet;
    //        }
    //        // Code in this block will run in another thread
    //    });

    // HikVisionLocationConfig *hikVLCfg = qobject_cast<HikVisionLocationConfig *>(prConfig);
    // SC_ASSERT(hikVLCfg != nullptr);
    // HikVisionResultImageInfo *hikResultImageInfo = new HikVisionResultImageInfo();
    //(*resultImageInfo) = hikResultImageInfo;

    // TBD
    return true;
}

int __stdcall HikVision::CallBackModuRes(IN IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo, IN void *const pUser)
{
    HikVision *pCtrlDemoThis = (HikVision *)pUser;
    int nRet = IMVS_EC_UNKNOWN;

    if (pCtrlDemoThis)
    {
        nRet = pCtrlDemoThis->CallBackModuResFunc(pstInputPlatformInfo);
        if (IMVS_EC_OK != nRet)
        {
            return nRet;
        }
    }
    return IMVS_EC_OK;
}

int HikVision::CallBackModuResFunc(IN IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo)
{
    if (IMVS_NULL == pstInputPlatformInfo)
    {
        return IMVS_EC_NULL_PTR;
    }
    if (IMVS_NULL == (pstInputPlatformInfo->pData))
    {
        return IMVS_EC_NULL_PTR;
    }
    qDebug() << "HikVision::CallBackModuResFunc" << pstInputPlatformInfo->nInfoType;
    if (IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_MODULE_RESULT == pstInputPlatformInfo->nInfoType)
    {
        // Parsing module callback results after V32
        IMVS_PF_MODULE_RESULT_INFO_LIST *pstPFModuResInfoList = (IMVS_PF_MODULE_RESULT_INFO_LIST *)pstInputPlatformInfo->pData;

        // Parsing module callback results after V3.0
        // IMVS_PF_MODU_RES_INFO *pstPFModuResInfoList = (IMVS_PF_MODU_RES_INFO *)pstInputPlatformInfo->pData;

        if ((10000 == pstPFModuResInfoList->nProcessID || 10001 == pstPFModuResInfoList->nProcessID))
        {
            qDebug() << "GetV32ResFromCallBack start" << pstPFModuResInfoList->nProcessID;
            GetV32ResFromCallBack(pstPFModuResInfoList);
            // CopyModuResultByModu(pstPFModuResInfoList); //v30
            qDebug() << "GetV32ResFromCallBack end";
        }
    }
}

int HikVision::GetV32ResFromCallBack(IN IMVS_PF_MODULE_RESULT_INFO_LIST *const pstPFModuResInfoList)
{
    qDebug() << "V32-get PR result Process1-nResultNum:" << pstPFModuResInfoList->nResultNum;
    for (int i = 0; i < pstPFModuResInfoList->nResultNum; i++)
    {
        if (IMVS_PF_MODURES_TYPE_INT == pstPFModuResInfoList->pModuResInfo[i].nParamType)
        {
            if (0 == strcmp("MatchNum2", pstPFModuResInfoList->pModuResInfo[i].strParamName))
            {
                int n = *pstPFModuResInfoList->pModuResInfo[i].pIntValue;
                qDebug() << "MatchNum2:" << n;
            }
            mCTimeSpent.SC_EndClock("MatchNum2");
        }
        if (IMVS_PF_MODURES_TYPE_FLOAT == pstPFModuResInfoList->pModuResInfo[i].nParamType)
        {
            if (0 == strcmp("MatchPointX2", pstPFModuResInfoList->pModuResInfo[i].strParamName))
            {
                float f = *pstPFModuResInfoList->pModuResInfo[i].pFloatValue;
                qDebug() << "MatchPointX2:" << f;
                mCTimeSpent.SC_EndClock("MatchPointX2");
            }
            if (0 == strcmp("MatchPointY2", pstPFModuResInfoList->pModuResInfo[i].strParamName))
            {
                float f = *pstPFModuResInfoList->pModuResInfo[i].pFloatValue;
                qDebug() << "MatchPointY2:" << f;
                mCTimeSpent.SC_EndClock("MatchPointY2");
            }
            if (0 == strcmp("MatchPointX6", pstPFModuResInfoList->pModuResInfo[i].strParamName))
            {
                float f = *pstPFModuResInfoList->pModuResInfo[i].pFloatValue;
                qDebug() << "MatchPointX6:" << f;
                mCTimeSpent.SC_EndClock("MatchPointX6");
            }
        }
    }
    return 0;
}

/// \brief HikVision::CopyModuResultByModu
/// \param pstPFModuResInfoList
/// \return
int HikVision::CopyModuResultByModu(IN IMVS_PF_MODU_RES_INFO *const pstPFModuResInfoList)
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
                if (i < 64)    // This example shows 64 matches
                {
                    nCDfine.m_stMatchFeatureRect.nNum = i + 1;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX
                        = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtX;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY
                        = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtY;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fHeight;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fWidth;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fAngle;
                    qDebug() << "Match return"
                             << "fCenterX:" << nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX;
                    qDebug() << "Match return"
                             << "fCenterY:" << nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY;
                    qDebug() << "Match return"
                             << "fHeight:" << nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight;
                    qDebug() << "Match return"
                             << "fWidth:" << nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth;
                    qDebug() << "Match return"
                             << "fAngle:" << nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle;
                    mCTimeSpent.SC_EndClock("start PR To Match return--Process1");
                }
            }
        }
    }

    //    //    QtConcurrent::run([this, pstPFModuResInfoList]{
    //    if (0 == strcmp(MODU_NAME_SAVEIMAGE, (pstPFModuResInfoList->strModuleName)))
    //    {
    //        qDebug() << "get image" << QDateTime::currentMSecsSinceEpoch();
    //        QThread::msleep(1000);
    //        qDebug() << "after sleeep" << QDateTime::currentMSecsSinceEpoch();
    //        IMVS_PF_SAVEIMAGE_MODU_INFO__ * pOutPutImageModuRes = (IMVS_PF_SAVEIMAGE_MODU_INFO__ *)pstPFModuResInfoList->pData;

    //        int callBackImgFormate = pOutPutImageModuRes->stOutImgInfo.iImgFormat;
    //        auto qImgFormate = (callBackImgFormate == IMVS_PF_IMG_FORMAT_MONO8) ? QImage::Format_Indexed8 : QImage::Format_RGB888;
    //        QImage image(pOutPutImageModuRes->stOutImgInfo.iWidth, pOutPutImageModuRes->stOutImgInfo.iHeight, qImgFormate);

    //        qDebug() << "hik image info" << pOutPutImageModuRes->stOutImgInfo.iWidth << pOutPutImageModuRes->stOutImgInfo.iHeight <<
    //        pOutPutImageModuRes->stOutImgInfo.iImgDataLen;

    //        qDebug() << "image info before load" <<  image.width() << image.height() << image.format() << image.byteCount() << image.bytesPerLine();

    //        if(image.bytesPerLine() == image.width() * 3)
    //        {
    //            qDebug() << "copy 1";
    //            memcpy(image.bits(), pOutPutImageModuRes->stOutImgInfo.pImgData, pOutPutImageModuRes->stOutImgInfo.iImgDataLen);
    //        }
    //        else
    //        {
    //            qDebug() << "copy 2";
    //            int bytesPerLine = image.width() * 3;
    //            for (int i = 0; i < image.height(); i++)
    //            {
    //                memcpy(image.scanLine(i), pOutPutImageModuRes->stOutImgInfo.pImgData + i * bytesPerLine, bytesPerLine);
    //            }
    //        }

    //        QElapsedTimer timer;
    //        timer.start();

    //        //           for (int i = 0; i < 50; i++)
    //        //           {
    //        //              qDebug() <<   image.save(QString("test%1.jpg").arg(i));
    //        //           }
    //        qDebug() << "elapsed:" << timer.elapsed();
    //    }
    //    // HPFeatureMatch module
    //    if (0 == strcmp(MODU_NAME_HPFEATUREMATCHMODU, (pstPFModuResInfoList->strModuleName)))
    //    {
    //        qDebug() << "get result" << QDateTime::currentMSecsSinceEpoch();

    //        IMVS_PF_HPFEATUREMATCH_MODU_INFO * pstHPFeatureMatchModuRes = (IMVS_PF_HPFEATUREMATCH_MODU_INFO *)pstPFModuResInfoList->pData;
    //        m_nMatchPtNum = pstHPFeatureMatchModuRes->iMatchNum;
    //        for (int i = 0; i < (int)m_nMatchPtNum; i++)
    //        {
    //            if (i < 64)    //This example shows 64 matches
    //            {
    //                nCDfine.m_stMatchFeatureRect.nNum = i + 1;
    //                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX =
    //                pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtX;
    //                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY =
    //                pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtY;
    //                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fHeight;
    //                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fWidth;
    //                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fAngle;
    //                qDebug()<<"Match return"<<"fCenterX:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX; qDebug()<<"Match
    //                return"<<"fCenterY:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY; qDebug()<<"Match
    //                return"<<"fHeight:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight; qDebug()<<"Match
    //                return"<<"fWidth:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth; qDebug()<<"Match
    //                return"<<"fAngle:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle; mCTimeSpent.EndClock("start PR To Match return");
    //            }
    //        }
    //    }
    //    else
    //    {
    //        return 0;
    //    }
    //    //    });

    return 0;
}

int HikVision::SC_ShowModuleInterface(unsigned int nIndex)
{
    int nRet = IMVS_PF_StartVisionMaster(m_handle, mHikVisionMasterAppPath.toUtf8(), 10000);
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_StartVisionMaster Error" << nRet;
        return -1;
    }
    else
    {
        qDebug() << "IMVS_PF_StartVisionMaster OK" << nRet;
    }
    // nRet = IMVS_PF_ShowVisionMaster(m_handle, 1);
    // nRet = IMVS_PF_LoadSolution(m_handle, mSolutionPath.toUtf8(), "");
    QThread::msleep(300);
    nRet = IMVS_PF_ShowModuleInterface(m_handle, nIndex);
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_ShowModuleInterface Error" << nRet;
    }
    else
    {
        qDebug() << "IMVS_PF_ShowModuleInterface OK" << nRet;
    }
    return nRet;
}

int HikVision::SC_SaveSolution(QString nSolutionPath, QString nPassWord)
{
    IMVS_PF_SAVE_SOLUTION_INPUT stSaveInput = { 0 };
    _snprintf_s(stSaveInput.strPath, IMVS_PF_MAX_PATH_LENGTH - 1, "%s", nSolutionPath.toUtf8().data());
    _snprintf_s(stSaveInput.strPassWord, IMVS_PF_PASSWORD_LENGTH - 1, "%s", nPassWord.toUtf8().data());
    int nRet = IMVS_PF_SaveSolution(m_handle, &stSaveInput);
    if (IMVS_EC_OK != nRet)
    {
        qDebug() << "IMVS_PF_SaveSolution Error" << nRet;
        return -1;
    }
    else
    {
        qDebug() << "IMVS_PF_SaveSolution OK" << nRet;
    }
    unsigned int nSaveprogress = 0;
    QElapsedTimer qTimer;
    qTimer.start();
    bool isTimeOut = false;
    while (nRet == IMVS_EC_OK)
    {
        nRet = IMVS_PF_GetSaveProgress(m_handle, &nSaveprogress);

        if (nSaveprogress == 100)
        {
            qDebug() << "IMVS_PF_GetSaveProgress OK" << nRet;
            break;
        }
        if (qTimer.elapsed() > 5000)
        {
            qDebug() << "IMVS_PF_GetSaveProgress TimeOut" << nRet;
            isTimeOut = true;
            break;
        }
    }
    if (isTimeOut)
    {
        return -1;
    }
    return 0;
}

// If change module paramer ,not need to reloadSolution.
int HikVision::SC_ReloadSolution(QString nSolutionPath, QString nPassword)
{
    // int nRet = IMVS_PF_LoadSolution(m_handle, mSolutionPath.toUtf8(), "");
    int nRet = IMVS_PF_SyncLoadSolution(m_handle, nSolutionPath.toUtf8(), nPassword.toUtf8(), false);    // Synchronous
    qDebug() << "SC_ReloadSolution:" << nRet;
    return nRet;
}
