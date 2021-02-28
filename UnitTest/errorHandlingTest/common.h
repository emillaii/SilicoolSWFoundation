#ifndef COMMON_H
#define COMMON_H

#include <QObject>

inline int myRand(int start, int end)
{
    srand(time(NULL));
    return rand() % (end - start) + start;
}

inline int myRand(int end)
{
    srand(time(NULL));
    return rand() % end;
}

#endif    // COMMONMETHOD_H
