#include "loggerwindowbackend.h"

LoggerWindowBackend::LoggerWindowBackend(QObject* parent)
    : QObject { parent } {
}

bool LoggerWindowBackend::write_logger(const std::string& log_info) {
	emit append_logger(QString::fromStdString(log_info));
	return true;
}

bool LoggerWindowBackend::flush() {
	return true;
}
