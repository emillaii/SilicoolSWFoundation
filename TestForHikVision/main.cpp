#include <stdio.h>
#include <QCoreApplication>
#include <QTextStream>
#include "../HikVision/hikvision.h"
#include <windows.h>
#include <string.h>
#include <tlhelp32.h>
#include <QTextCodec>
#include <QDebug>

void StartClock();
void EndClock(QString msg);
double time_Start = 0.0;
double time_End = 0.0;

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    QCoreApplication a(argc, argv);
    QTextStream in(stdin);//
    QTextStream out(stdout);//

    StartClock();
    HikVision hik;
    EndClock("HikVision init Time spent:");

    QImage myImage1; VisionLocationConfig *prConfig; PRResultImageInfo *resultImageInfo; PRResultStruct prResult;

    QString mPicturePath = "C:\\Users\\Aini\\Desktop\\VisionMasterLearning\\BaslerPicture.png";
    myImage1  = QImage(mPicturePath.toUtf8());

    qDebug()<<"image.width():"<<myImage1.width()<<"image.height()"<<myImage1.height();

    //rgb to gray
    QImage gray = myImage1.convertToFormat(QImage::Format_Grayscale8);//QImage::Format_Grayscale8
    //gray.save("SaveTest.png","PNG",-1);

    StartClock();
    int i = hik.performPr(gray,prConfig,&resultImageInfo,prResult);
    EndClock("performPr Time spent:");

    hik.Close();
    system("pause");
    return a.exec();
}

void StartClock()
{
    time_Start = (double)clock();
}
void EndClock(QString msg)
{
    time_End = (double)clock();
    qDebug()<<msg<<(time_End - time_Start)/1000.0<<"s";
}

bool KillProcess(QString ProcessName)
{
    bool result = false;
    QString str1;

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0) ;
    PROCESSENTRY32 pInfo;
    pInfo.dwSize = sizeof(pInfo);

    Process32First(hSnapShot, &pInfo);
    do
    {
        str1 = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(pInfo.szExeFile)));
        if (str1 == ProcessName)
        {
            result = true;
            QString cmd;
            cmd = QString("taskkill /F /PID %1 /T").arg(pInfo.th32ProcessID);
            qDebug()<<cmd<<"VisionMasterServer.exe";
            system(cmd.toUtf8());
        }
    } while(Process32Next(hSnapShot, &pInfo) );
    return result;
}

