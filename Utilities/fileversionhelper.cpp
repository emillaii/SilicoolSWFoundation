#include "fileversionhelper.h"

QString getVersion(const QString &fullName)
{
    ulong dwLen = 0;
    char *lpData = nullptr;
    const wchar_t *str_path = fullName.toStdWString().c_str();
    bool bSuccess = false;
    QString fileInfomation;
    ulong vHandle = 0;
    dwLen = GetFileVersionInfoSize(str_path, &vHandle);
    if (0 == dwLen)
    {
        qCritical() << "GetFileVersionInfoSize failed!";
        return "";
    }
    lpData = (char *)malloc(dwLen + 1);
    if (nullptr == lpData)
    {
        qCritical() << "Malloc memory failed!";
        return "";
    }
    bSuccess = GetFileVersionInfo(str_path, 0, dwLen + 1, lpData);
    if (!bSuccess)
    {
        qCritical() << "GetFileVersionInfo failed!";
        return "";
    }
    LPVOID lpBuffer = NULL;
    UINT uLen = 0;

    bSuccess = VerQueryValue(lpData, (TEXT("\\VarFileInfo\\Translation")), &lpBuffer, &uLen);
    QString strTranslation, str1, str2;
    unsigned short int *p = (unsigned short int *)lpBuffer;
    str1.setNum(*p, 16);
    str1 = "000" + str1;
    strTranslation += str1.mid(str1.size() - 4, 4);
    str2.setNum(*(++p), 16);
    str2 = "000" + str2;
    strTranslation += str2.mid(str2.size() - 4, 4);

    QString code;
    code = "\\StringFileInfo\\" + strTranslation + "\\FileVersion";
    bSuccess = VerQueryValue(lpData, (code.toStdWString().c_str()), &lpBuffer, &uLen);
    if (!bSuccess)
    {
        qCritical() << "Get FileVersion failed!";
    }
    else
    {
        fileInfomation = QString::fromUtf16((const unsigned short int *)lpBuffer);
    }

    delete lpData;

    return fileInfomation;
}
