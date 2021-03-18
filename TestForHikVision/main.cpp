#include "../HikVision/hikvision.h"
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <QtCore/QCoreApplication>
#include <stdio.h>
#include <string.h>
#include <windows.h>

BOOL HandlerRoutine(DWORD CtrlType);
HikVision mhik;
SCTimeSpent mCTimeSpent;

int main(int argc, char *argv[])
{
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, TRUE);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    QCoreApplication a(argc, argv);
    /*QTextStream in(stdin);      //
    QTextStream out(stdout);  */  //

    QImage myImage1;
    VisionLocationConfig *prConfig;
    PRResultImageInfo *resultImageInfo;
    PRResultStruct prResult;

    QString mPicturePath = "C:\\Users\\Aini\\Desktop\\VisionMasterLearning\\LensUplookRectLocation_18_39_14_634.png";
    myImage1 = QImage(mPicturePath.toUtf8());

    qDebug() << "image.width():" << myImage1.width() << "image.height()" << myImage1.height();

    // rgb to gray
    QImage gray = myImage1.convertToFormat(QImage::Format_Grayscale8);    // QImage::Format_Grayscale8//Format_Indexed8
    // gray.save("SaveTest.png","PNG",-1);

    QString str;
    qDebug() << "Please input any char--myShowModuleInterface";
    getchar();

    mCTimeSpent.SC_StartClock();
    qDebug() << "start myShowModuleInterface";
    mhik.SC_ShowModuleInterface(str.toUInt());
    // int i = mhik.performPr(gray, prConfig, &resultImageInfo, prResult);
    mCTimeSpent.SC_EndClock("performPr Time spent:");
    qDebug() << "Please input any char--saveSolution";
    getchar();
    mhik.SC_SaveSolution("C:\\Users\\Aini\\Desktop\\VisionMasterLearning\\GetImageFromOutput.sol", "");
    //    qDebug() << "Please input any char--reLoadSolution";
    //    getchar();
    //    mhik.reLoadSolution("C:\\Users\\Aini\\Desktop\\VisionMasterLearning\\GetImageFromOutput.sol", "");
    qDebug() << "Please input any char--performPr";
    getchar();
    int i = mhik.performPr(gray, prConfig, &resultImageInfo, prResult);

    qDebug() << "Please input any char";
    getchar();
    mhik.SC_Close();
    qDebug() << "isClosed";

    // system("pause");
    return a.exec();
}

// bool KillProcess(QString ProcessName)
//{
//    bool result = false;
//    QString str1;
//    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0) ;
//    PROCESSENTRY32 pInfo;
//    pInfo.dwSize = sizeof(pInfo);
//    Process32First(hSnapShot, &pInfo);
//    do
//    {
//        str1 = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(pInfo.szExeFile)));
//        if (str1 == ProcessName)
//        {
//            result = true;
//            QString cmd;
//            cmd = QString("taskkill /F /PID %1 /T").arg(pInfo.th32ProcessID);
//            qDebug()<<cmd<<"VisionMasterServer.exe";
//            system(cmd.toUtf8());
//        }
//    } while(Process32Next(hSnapShot, &pInfo) );
//    return result;
//}

BOOL HandlerRoutine(DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
        case CTRL_C_EVENT:
            printf("ctrl+c\n");
            return TRUE;
        case CTRL_CLOSE_EVENT:
            mhik.SC_Close();
            printf("ctrl close\n");
            return TRUE;
        case CTRL_BREAK_EVENT:
            printf("CTRL_BREAK_EVENT\n");
        case CTRL_LOGOFF_EVENT:
            printf("CTRL_LOGOFF_EVENT\n");
        case CTRL_SHUTDOWN_EVENT:
            printf("CTRL_SHUTDOWN_EVENT\n");
        default:
            return FALSE;
    }
}
