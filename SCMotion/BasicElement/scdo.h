#ifndef SCDO_H
#define SCDO_H

#include "MotionManager/motionelementcontainer.h"
#include "configManager/configobject.h"
#include "errorHandling/silicolerror.h"
#include "ioconfig.h"
#include "motionutility.h"
#include "scmotion_global.h"
#include <QObject>

///
/// \brief The SCDO class
/// Silicool digital output
///
class SCMOTIONSHARED_EXPORT SCDO : public QObject
{
    Q_OBJECT
public:
    explicit SCDO(QString name, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::DO;
    }

    QString name() const
    {
        return objectName();
    }

    IOConfig *config() const
    {
        return m_config;
    }

    void setConfig(IOConfig *config)
    {
        m_config = config;
    }

    void init();

    ///
    /// \brief get
    /// Get do state from cache.
    /// Software will push the do state to cache, and give it
    /// a timestamp. If the elapsed time is greater than the given
    /// 'cacheInvalidationTime', software will update cache first.
    /// If you want to get the realtime do state, give 'cacheInvalidationTime'
    /// with -1.
    /// \param cacheInvalidationTime ms
    /// \return
    ///
    Q_INVOKABLE bool get(int cacheInvalidationTime = 2);

    Q_INVOKABLE void set(bool state);

protected:
    virtual bool getImpl() noexcept = 0;
    virtual void setImpl(bool state) noexcept = 0;
    virtual void initImpl() = 0;

private:
    bool isInit = false;
    IOConfig *m_config;
    bool lastDoState;
    qint64 lastUpdateDoStateTimeStamp = 0;
};

#endif    // SCDO_H
