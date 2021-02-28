#ifndef ERRORHANDLINGOPTION_H
#define ERRORHANDLINGOPTION_H

#include <QObject>

///
/// \brief All value must be unique!
///

const QString Retry_ = QObject::tr("Retry");
const QString CustomRetry_ = QObject::tr("DoRetry");
const QString Ignore_ = QObject::tr("Ignore");
const QString Stop_ = QObject::tr("Stop");
const QString Abort_ = QObject::tr("Abort");
const QString Reset_ = QObject::tr("Reset");
const QString Skip_ = QObject::tr("Skip");
const QString ManuallyTakeAway_ = QObject::tr("手动拿走");
const QString ManuallyPutAway_ = QObject::tr("手动放好");
const QString PlaceWithoutPr_ = QObject::tr("盲放");

#endif    // ERRORHANDLINGOPTION_H
