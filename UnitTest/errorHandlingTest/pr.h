#ifndef PR_H
#define PR_H

#include "common.h"
#include <QObject>

struct PROffset
{
    double x;
    double y;
    double theta;
};

class PR
{
public:
    bool execute(PROffset &offset)
    {
        if (myRand(3) == 0)
        {
            return false;
        }
        offset.x = myRand(3);
        offset.y = myRand(3);
        offset.theta = myRand(3);
        return true;
    }
};

#endif    // PR_H
