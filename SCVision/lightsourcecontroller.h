#ifndef LIGHTSOURCECONTROLLER_H
#define LIGHTSOURCECONTROLLER_H

#include "scvision_global.h"

class SCVISIONSHARED_EXPORT LightSourceController
{
public:
    virtual void init() = 0;
    virtual ~LightSourceController() {}
    virtual void setBrightness(int channel, int brightness) = 0;
    virtual bool getBrightness(int channel, int &brightness) = 0;
    virtual void open(int channel) = 0;
    virtual void close(int channel) = 0;
};

#endif    // LIGHTSOURCECONTROLLER_H
