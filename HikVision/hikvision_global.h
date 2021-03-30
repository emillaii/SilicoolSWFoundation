#ifndef HIKVISION_GLOBAL_H
#define HIKVISION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HIKVISION_LIBRARY)
#  define HIKVISIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HIKVISIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HIKVISION_GLOBAL_H
