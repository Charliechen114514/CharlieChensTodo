#ifndef URL_READER_H
#define URL_READER_H

#include <QString>
class QUrl;

#if defined(UTILITIES_LIBRARY)
#define UTILITIES_EXPORT Q_DECL_EXPORT
#else
#define UTILITIES_EXPORT Q_DECL_IMPORT
#endif

namespace cutils {
/**
 * @brief readFromURL
 * @return bytes read from url
 * @exception runtime_error: file not find or can not read
 */
QString UTILITIES_EXPORT readFromURL(const QUrl& url);

QString UTILITIES_EXPORT readFromLocalPath(const QString& path);

void UTILITIES_EXPORT truncate_write(const QString& path, const QString& bytes);
}

#endif // URL_READER_H
