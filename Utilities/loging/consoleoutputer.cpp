#include "consoleoutputer.h"

void ConsoleOutputer::outputLogToConsole(int logLevel, const QString &msg)
{
    QMutexLocker t(&locker);

    if (logLevel < LogLevel::Warning)
    {
        if (lastConsoleColor != White)
        {
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
            lastConsoleColor = White;
        }
        std::cout << QTextCodec::codecForLocale()->fromUnicode(msg).toStdString() << std::endl;
    }
    else if (logLevel == LogLevel::Warning)
    {
        if (lastConsoleColor != Yellow)
        {
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED);
            lastConsoleColor = Yellow;
        }
        std::cout << QTextCodec::codecForLocale()->fromUnicode(msg).toStdString() << std::endl;
    }
    else
    {
        if (lastConsoleColor != Red)
        {
            setConsoleColor(FOREGROUND_RED);
            lastConsoleColor = Red;
        }
        std::cout << QTextCodec::codecForLocale()->fromUnicode(msg).toStdString() << std::endl;
    }
}
