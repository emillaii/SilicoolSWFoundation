#include "scvision.h"

void SCVision::init()
{
    if (isInit)
    {
        return;
    }
    initImpl();
    isInit = true;
}

void SCVision::dispose()
{
    if (isInit)
    {
        isInit = false;
        disposeImpl();
    }
}
