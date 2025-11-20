#ifndef LOGGERBACKEND_H
#define LOGGERBACKEND_H
#include "formater.h"
#include "logger_io.h"
#include <QMessageLogger>
#include <QMutex>
#include <QObject>
#include <QStringList>
class LoggerWindowBackend;
namespace Clog {
class CCLogger;
class LoggerIO;
}

class LoggerBackend {
public:
	static LoggerBackend& instance();

	enum class SupportFormat {
		Simplified,
		Standard,
		OnlyContent
	};

	void addLoggerFilePath(const QString& p,
	                       const SupportFormat format = SupportFormat::Simplified);
	QStringList loggerPath() const { return loggers_local_path; }
	void registerIOBackEnd(std::unique_ptr<Clog::LoggerIO> backend,
	                       std::unique_ptr<Clog::LoggerFormatter> formater);
	~LoggerBackend();

private:
	LoggerBackend();
	void qtToCCLoggerHandler(QtMsgType type,
	                         const QMessageLogContext& context,
	                         const QString& msg);
	static void __qtToCCLoggerHandler(QtMsgType type,
	                                  const QMessageLogContext& context,
	                                  const QString& msg) {
		instance().qtToCCLoggerHandler(type, context, msg);
	}
	LoggerWindowBackend* loggerWindowBackend;
	QtMessageHandler old_handler;
	QStringList loggers_local_path;
	Clog::CCLogger* logger;
	QMutex m;
	QMutex log_self;
};

#endif // LOGGERBACKEND_H
