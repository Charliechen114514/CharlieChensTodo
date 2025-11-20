#ifndef LOGGERWINDOWBACKEND_H
#define LOGGERWINDOWBACKEND_H

#include "logger_io.h"
#include <QObject>

class LoggerWindowBackend : public QObject, public Clog::LoggerIO {
	Q_OBJECT
public:
	explicit LoggerWindowBackend(QObject* parent = nullptr);
	bool write_logger(const std::string& log_info) override;
	bool flush() override;
signals:
	void append_logger(const QString& f);
};

#endif // LOGGERWINDOWBACKEND_H
