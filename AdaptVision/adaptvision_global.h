#ifndef ADAPTVISION_GLOBAL_H
#define ADAPTVISION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ADAPTVISION_LIBRARY)
#  define ADAPTVISIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ADAPTVISIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ADAPTVISION_GLOBAL_H
