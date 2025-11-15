#ifndef UTILITIES_GLOBAL_H
#define UTILITIES_GLOBAL_H
#include <QtCore/qglobal.h>
#if defined(UiUtils_LIBRARY)
#define UiUtils_EXPORT Q_DECL_EXPORT
#else
#define UiUtils_EXPORT Q_DECL_IMPORT
#endif

#endif // UTILITIES_GLOBAL_H
