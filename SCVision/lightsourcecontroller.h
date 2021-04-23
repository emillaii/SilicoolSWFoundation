#ifndef LIGHTSOURCECONTROLLER_H
#define LIGHTSOURCECONTROLLER_H

#include "scvision_global.h"

class SCVISIONSHARED_EXPORT LightSourceController
{
public:
    virtual void init() = 0;
    virtual void dispose() {}
    virtual ~LightSourceController() {}
    virtual void setBrightness(int channel, int brightness) = 0;
    virtual bool getBrightness(int channel, int &brightness) = 0;
    virtual void open(int channel) = 0;
    virtual void close(int channel) = 0;
};

class SCVISIONSHARED_EXPORT DummyLSC : public LightSourceController
{
public:
    void init() override
    {
        qInfo("Init dummy light source controller...");
    }
    void setBrightness(int channel, int brightness) override
    {
        Q_UNUSED(channel)
        Q_UNUSED(brightness)
    }
    bool getBrightness(int channel, int &brightness) override
    {
        Q_UNUSED(channel)
        Q_UNUSED(brightness)
        return true;
    }
    void open(int channel) override
    {
        Q_UNUSED(channel)
    }
    void close(int channel) override
    {
        Q_UNUSED(channel)
    }
};

#endif    // LIGHTSOURCECONTROLLER_H
