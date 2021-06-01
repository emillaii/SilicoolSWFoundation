#ifndef ERRORHANDLINGOPTION_H
#define ERRORHANDLINGOPTION_H

#include <QObject>

///
/// \brief All value must be unique!
///

const QString Retry_ = "Retry";
const QString CustomRetry_ = "DoRetry";
const QString Ignore_ = "Ignore";
const QString Stop_ = "Stop";
const QString Abort_ = "Abort";
const QString Reset_ = "Reset";
const QString Skip_ = "Skip";
const QString ManuallyTakeAway_ = u8"手动拿走";
const QString ManuallyPutAway_ = u8"手动放好";
const QString PlaceWithoutPr_ = u8"盲放";

#endif    // ERRORHANDLINGOPTION_H
