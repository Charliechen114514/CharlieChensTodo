#ifndef LOGGERBACKEND_H
#define LOGGERBACKEND_H
#include <QMessageLogger>
#include <QMutex>

namespace Clog {
class CCLogger;
}

class LoggerBackend {
public:
	static LoggerBackend& instance();
	void addLoggerFilePath(const QString& p);
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
	QtMessageHandler old_handler;

	Clog::CCLogger* logger;
	QMutex m;
	QMutex log_self;
};

#endif // LOGGERBACKEND_H
