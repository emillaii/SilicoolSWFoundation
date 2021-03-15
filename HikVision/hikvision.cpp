#include "hikvision.h"
#include "QThread"
HikVision::HikVision(QObject *parent)
{
    Init();
}

HikVision::~HikVision()
{
    // unload vision master
}

int HikVision::Init()
{
    qDebug() << "Start server...";

    if(m_handle!=nullptr)
    {
        qDebug()<<"m_handle!=nullptr";
        IMVS_PF_DestroyHandle(m_handle);
        m_handle = IMVS_NULL;
    }
    qDebug()<<"m_handle==nullptr";

    int nRet = IMVS_PF_CreateHandle(&m_handle, mHikVisionMasterServerPath.toLatin1().data());
    qDebug()<<"IMVS_PF_CreateHandle" << nRet;
    //    if(IMVS_EC_OK != nRet)
    //    {
    //        nRet = IMVS_PF_CreateHandle(&m_handle);
    //         qDebug()<<"IMVS_PF_CreateHandle2" << nRet;
    //    }

    unsigned int nLoadprogress = 0;

    //    nRet = IMVS_PF_StartVisionMaster(m_handle, mHikVisionMasterAppPath.toUtf8(),50000);
    //    qDebug()<<"IMVS_PF_StartVisionMaster" << nRet;

    //    nRet = IMVS_PF_ShowVisionMaster(m_handle, 1);
    //    qDebug()<<"IMVS_PF_ShowVisionMaster" << nRet;

    nRet = IMVS_PF_RegisterResultCallBack_V30(m_handle, CallBackModuRes, this);
    if (IMVS_EC_OK != nRet)
    {
        qDebug()<<"IMVS_PF_RegisterResultCallBack_V30 Error" << nRet;
    }
    else
    {
        qDebug()<<"IMVS_PF_RegisterResultCallBack_V30 OK" << nRet;
    }
    nRet = IMVS_PF_LoadSolution(m_handle, mSolutionPath.toUtf8(), "");
    qDebug()<<"IMVS_PF_LoadSolution" << nRet;
    QElapsedTimer qTimer;
    qTimer.start();
    bool IsTimeOut = false;
    while (nRet == IMVS_EC_OK)
    {
        nRet = IMVS_PF_GetLoadProgress(m_handle, &nLoadprogress);

        if(nLoadprogress == 100)
        {
            qDebug()<<"SolutionHaveBeenLoaded OK" << nRet;
            break;
        }
        if(qTimer.elapsed()>5000)
        {
            qDebug()<<"SolutionHaveBeenLoaded TimeOut" << nRet;
            break;
        }
    }
    return 0;
}

void HikVision::Close()
{

    IMVS_PF_CloseSolution(m_handle); //
    IMVS_PF_DestroyHandle(m_handle); //
}

bool HikVision::performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
{
    //load vision master
    IMVS_PF_PROCESS_INFO_LIST stProcInfoList = {0};
    IMVS_PF_INPUT_IMAGE_INFO stImageData;

    //2.picture
    stImageData.nDataType = 0;
    stImageData.nModuleID = 1;
    stImageData.stImageDataInfo.pImgData = (char *)image.bits();//pixArray.data();//
    stImageData.stImageDataInfo.iImgDataLen = (int)image.sizeInBytes();
    stImageData.stImageDataInfo.iWidth = image.width();
    stImageData.stImageDataInfo.iHeight = image.height();
    stImageData.stImageDataInfo.iImgFormat = 1;
    int  nRet = IMVS_PF_SetImageData(m_handle, &stImageData); //
    if (IMVS_EC_OK != nRet)
    {
        qDebug()<<"IMVS_PF_SetImageData Error" << nRet;
    }
    else
    {
        qDebug()<<"IMVS_PF_SetImageData OK" << nRet;
    }
    stImageData.nModuleID = 3;
    nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
    mCTimeSpent.StartClock();
    nRet = IMVS_PF_ExecuteOnce(m_handle, NULL);
    if (IMVS_EC_OK != nRet)
    {
        qDebug()<<"IMVS_PF_ExecuteOnce Error" << nRet;
    }
    else
    {
        qDebug()<<"IMVS_PF_ExecuteOnce OK" << nRet;
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
    QFuture<void> future = QtConcurrent::run([=]() {
       int nRet = IMVS_PF_ExecuteOnce_V30(m_handle, stProcInfoList.astProcessInfo[0].nProcessID, nullptr);
       if (IMVS_EC_OK != nRet)
       {
           qDebug()<<"1-IMVS_PF_ExecuteOnce_V30 Error" << nRet;
       }
       else
       {
           qDebug()<<"1-IMVS_PF_ExecuteOnce_V30 OK" << nRet;
       }
       // Code in this block will run in another thread
    });
     future = QtConcurrent::run([=]() {
       int nRet = IMVS_PF_ExecuteOnce_V30(m_handle, stProcInfoList.astProcessInfo[1].nProcessID, nullptr);
       if (IMVS_EC_OK != nRet)
       {
           qDebug()<<"2-IMVS_PF_ExecuteOnce_V30 Error" << nRet;
       }
       else
       {
           qDebug()<<"2-IMVS_PF_ExecuteOnce_V30 OK" << nRet;
       }
       // Code in this block will run in another thread
    });
    //    IMVS_PF_CloseSolution(m_handle); //
    //    IMVS_PF_DestroyHandle(m_handle); //
    //QString strImgPath1 = QObject::tr("E:\\VisionMaster\\VisionMaster3.4.0\\Applications\\VisionMaster.exe");
    //QThread::msleep(10000);

    //HikVisionLocationConfig *hikVLCfg = qobject_cast<HikVisionLocationConfig *>(prConfig);
    //SC_ASSERT(hikVLCfg != nullptr);

    //HikVisionResultImageInfo *hikResultImageInfo = new HikVisionResultImageInfo();
    //(*resultImageInfo) = hikResultImageInfo;

    // TBD
    return true;
}

int __stdcall HikVision::CallBackModuRes(IN IMVS_PF_OUTPUT_PLATFORM_INFO * const pstInputPlatformInfo, IN void * const pUser)
{
    HikVision * pCtrlDemoThis = (HikVision *)pUser;
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

int HikVision::CallBackModuResFunc(IN IMVS_PF_OUTPUT_PLATFORM_INFO * const pstInputPlatformInfo)
{
    //	if (CDefine::IMVS_PF_STATUS_STOP_CALLBACK == m_nCallBackStatus)
    //	{
    //		return IMVS_EC_NOT_SUPPORT;
    //	}

    if (IMVS_NULL == pstInputPlatformInfo)
    {
        return IMVS_EC_INVALID_HANDLE;
    }
    if (IMVS_NULL == (pstInputPlatformInfo->pData))
    {
        return IMVS_EC_INVALID_HANDLE;
    }
    qDebug()<<"HikVision::CallBackModuResFunc"<<pstInputPlatformInfo->nInfoType;
    if (IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_MODULE_RESULT == pstInputPlatformInfo->nInfoType)
    {
        // Parsing module callback results after V3.0
        IMVS_PF_MODU_RES_INFO * pstPFModuResInfoList = (IMVS_PF_MODU_RES_INFO *)pstInputPlatformInfo->pData;

        // Determine if the process ID matches

        //int m_nProgressFlag = 0;
        qDebug()<<"CopyModuResultByModu start"<<pstPFModuResInfoList->nProcessID;
        if ((10000 == pstPFModuResInfoList->nProcessID))
        {
            CopyModuResultByModu(pstPFModuResInfoList);
            qDebug()<<"CopyModuResultByModu end";
        }
        if ((10001 == pstPFModuResInfoList->nProcessID))
        {
            GetResultFromProcess2(pstPFModuResInfoList);
            qDebug()<<"GetResultFromProcess2 end";
        }

    }
}

int HikVision::GetResultFromProcess2(IN IMVS_PF_MODU_RES_INFO * const pstPFModuResInfoList)
{
    if (0 == strcmp(MODU_NAME_HPFEATUREMATCHMODU, (pstPFModuResInfoList->strModuleName)))
    {
        if (6 == pstPFModuResInfoList->nModuleID)
        {
            qDebug() << "GetResultFromProcess2" << QDateTime::currentMSecsSinceEpoch();

            IMVS_PF_HPFEATUREMATCH_MODU_INFO * pstHPFeatureMatchModuRes = (IMVS_PF_HPFEATUREMATCH_MODU_INFO *)pstPFModuResInfoList->pData;
            m_nMatchPtNum = pstHPFeatureMatchModuRes->iMatchNum;
            for (int i = 0; i < (int)m_nMatchPtNum; i++)
            {
                if (i < 64)    //This example shows 64 matches
                {
                    nCDfine.m_stMatchFeatureRect.nNum = i + 1;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtX;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtY;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight  = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fHeight;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fWidth;
                    nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fAngle;
                    qDebug()<<"Match return"<<"fCenterX:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX;
                    qDebug()<<"Match return"<<"fCenterY:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY;
                    qDebug()<<"Match return"<<"fHeight:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight;
                    qDebug()<<"Match return"<<"fWidth:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth;
                    qDebug()<<"Match return"<<"fAngle:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle;
                    mCTimeSpent.EndClock("start PR To Match return--Process2");
                }
            }
        }
    }
    return 0;
}

int HikVision::CopyModuResultByModu(IN IMVS_PF_MODU_RES_INFO * const pstPFModuResInfoList)
{
        if (0 == strcmp(MODU_NAME_SAVEIMAGE, (pstPFModuResInfoList->strModuleName)))
        {
            if (0 == pstPFModuResInfoList->nModuleID)
            {

                IMVS_PF_SAVEIMAGE_MODU_INFO__ * pOutPutImageModuRes = (IMVS_PF_SAVEIMAGE_MODU_INFO__ *)pstPFModuResInfoList->pData;

                int callBackImgFormate = pOutPutImageModuRes->stOutImgInfo.iImgFormat;
                auto qImgFormate = (callBackImgFormate == IMVS_PF_IMG_FORMAT_MONO8) ? QImage::Format_Indexed8 : QImage::Format_RGB888;
                QImage image(pOutPutImageModuRes->stOutImgInfo.iWidth, pOutPutImageModuRes->stOutImgInfo.iHeight, qImgFormate);

                qDebug() << "hik image info" << pOutPutImageModuRes->stOutImgInfo.iWidth << pOutPutImageModuRes->stOutImgInfo.iHeight << pOutPutImageModuRes->stOutImgInfo.iImgDataLen;

                qDebug() << "image info before load" <<  image.width() << image.height() << image.format() << image.byteCount() << image.bytesPerLine();

                if(image.bytesPerLine() == image.width() * 3)
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

                IMVS_PF_HPFEATUREMATCH_MODU_INFO * pstHPFeatureMatchModuRes = (IMVS_PF_HPFEATUREMATCH_MODU_INFO *)pstPFModuResInfoList->pData;
                m_nMatchPtNum = pstHPFeatureMatchModuRes->iMatchNum;
                for (int i = 0; i < (int)m_nMatchPtNum; i++)
                {
                    if (i < 64)    //This example shows 64 matches
                    {
                        nCDfine.m_stMatchFeatureRect.nNum = i + 1;
                        nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtX;
                        nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtY;
                        nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight  = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fHeight;
                        nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fWidth;
                        nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fAngle;
                        qDebug()<<"Match return"<<"fCenterX:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX;
                        qDebug()<<"Match return"<<"fCenterY:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY;
                        qDebug()<<"Match return"<<"fHeight:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight;
                        qDebug()<<"Match return"<<"fWidth:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth;
                        qDebug()<<"Match return"<<"fAngle:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle;
                        mCTimeSpent.EndClock("start PR To Match return--Process1");
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

//        qDebug() << "hik image info" << pOutPutImageModuRes->stOutImgInfo.iWidth << pOutPutImageModuRes->stOutImgInfo.iHeight << pOutPutImageModuRes->stOutImgInfo.iImgDataLen;

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
//                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtX;
//                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtY;
//                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight  = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fHeight;
//                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fWidth;
//                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fAngle;
//                qDebug()<<"Match return"<<"fCenterX:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX;
//                qDebug()<<"Match return"<<"fCenterY:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY;
//                qDebug()<<"Match return"<<"fHeight:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight;
//                qDebug()<<"Match return"<<"fWidth:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth;
//                qDebug()<<"Match return"<<"fAngle:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle;
//                mCTimeSpent.EndClock("start PR To Match return");
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

