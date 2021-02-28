#ifndef CONSOLEOUTPUTER_H
#define CONSOLEOUTPUTER_H

#include "loglevel.h"
#include <QMutex>
#include <QMutexLocker>
#include <QTextCodec>
#include <Windows.h>
#include <iostream>

class ConsoleOutputer
{
private:
    enum ConsoleColor
    {
        White,
        Yellow,
        Red
    };

public:
    void outputLogToConsole(int logLevel, const QString &msg);

private:
    void setConsoleColor(unsigned short forecolor = 4, unsigned short backgroudcolor = 0)
    {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);                //获取缓冲区句柄
        SetConsoleTextAttribute(hCon, forecolor | backgroudcolor);    //设置文本及背景色
    }

private:
    int lastConsoleColor = White;
    QMutex locker;
};

#endif    // CONSOLEOUTPUTER_H
