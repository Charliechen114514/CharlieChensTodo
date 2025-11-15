#ifndef UTILITIES_GLOBAL_H
#define UTILITIES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTILITIES_LIBRARY)
#define UTILITIES_EXPORT Q_DECL_EXPORT
#else
#define UTILITIES_EXPORT Q_DECL_IMPORT
#endif

#endif // UTILITIES_GLOBAL_H
