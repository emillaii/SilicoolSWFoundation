#ifndef SCDI_H
#define SCDI_H

#include "../MotionManager/motionelementcontainer.h"
#include "../motionutility.h"
#include "configManager/configobject.h"
#include "ioconfig.h"
#include "scmotion_global.h"
#include <QObject>

///
/// \brief The SCDI class
/// Silicool digital input
///
class SCMOTIONSHARED_EXPORT SCDI : public QObject
{
    Q_OBJECT
public:
    explicit SCDI(QString name, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::DI;
    }

    virtual ~SCDI();

    QString name() const
    {
        return objectName();
    }

    bool isInit() const
    {
        return m_isInit;
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

    void disableWait()
    {
        isDisableWait = true;
    }
    void enableWait()
    {
        isDisableWait = false;
    }
    void disableCheck()
    {
        isDisableCheck = true;
    }
    void enableCheck()
    {
        isDisableCheck = false;
    }
    void disableAll();
    void enableAll();

    ///
    /// \brief get
    /// Get di state from cache.
    /// Software will push the di state to cache, and give it
    /// a timestamp. If the elapsed time is greater than the given
    /// 'cacheInvalidationTime', software will update cache first.
    /// If you want to get the realtime di state, give 'cacheInvalidationTime'
    /// with -1.
    /// \param cacheInvalidationTime ms
    /// \return
    ///
    Q_INVOKABLE bool get(int cacheInvalidationTime = 2);

    bool waitState(bool targetState, int timeout = 200);

    bool checkState(bool targetState, int cacheInvalidationTime = 2);

protected:
    virtual bool getImpl() noexcept = 0;
    virtual void initImpl() = 0;

private:
    bool m_isInit = false;
    IOConfig *m_config;
    bool lastDiState;
    qint64 lastUpdateDiStateTimeStamp = 0;

    bool isDisableWait = false;
    bool isDisableCheck = false;
};

#endif    // SCDI_H
