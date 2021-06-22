#include "acserrorhandle.h"

void checkACSCom(int res, QString msg)
{
    if(res == 0)
    {
        qCritical() << "excute ACSC command: " << msg << "failed! errCode: "<< res;
    }
}
