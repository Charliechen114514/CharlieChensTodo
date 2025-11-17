#include "loggerbackend.h"
#include "CCLogger.hpp"
#include "io/fileio.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace {
inline Clog::CCLoggerLevel convertLevel(QtMsgType type) {
	switch (type) {
	case QtDebugMsg:
		return Clog::CCLoggerLevel::Debug;
	case QtInfoMsg:
		return Clog::CCLoggerLevel::Info;
	case QtWarningMsg:
		return Clog::CCLoggerLevel::Warning;
	case QtCriticalMsg:
		return Clog::CCLoggerLevel::Error;
	case QtFatalMsg:
		return Clog::CCLoggerLevel::Error;
	}
	return Clog::CCLoggerLevel::Info;
}

}

LoggerBackend::LoggerBackend() {
	logger = new Clog::CCLogger;
	old_handler = qInstallMessageHandler(LoggerBackend::__qtToCCLoggerHandler);
}

LoggerBackend& LoggerBackend::instance() {
	static LoggerBackend b;
	return b;
}

void LoggerBackend::addLoggerFilePath(const QString& p) {
	QFileInfo info(p);
	QDir dir = info.dir();

	if (!dir.exists()) {
		if (!dir.mkpath(".")) {
			qWarning() << "Failed to create directory for log file:" << dir.absolutePath();
			return;
		}
	}
	QFile f(p);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		qWarning() << "Can not clean file: " + p.toStdString()
		           << "Loggers will append!";
	}
	f.close();
	QMutexLocker _(&m);
	logger->registerIOBackEnd(std::make_unique<Clog::FileLoggerIO>(p.toStdString()));
}

LoggerBackend::~LoggerBackend() {
	QMutexLocker _(&m);
	delete logger;
	logger = nullptr;
	qInstallMessageHandler(old_handler);
}

void LoggerBackend::qtToCCLoggerHandler(QtMsgType type,
                                        const QMessageLogContext& context,
                                        const QString& msg) {
	QMutexLocker _(&m);
	Clog::CCLoggerLevel l = convertLevel(type);
	const char* file = context.file ? context.file : "unknown files";
	const char* func = context.function ? context.function : "unknown functions";

	CCSourceLocation loc { file, func, static_cast<unsigned int>(context.line), 0 };
	logger->log(msg.toStdString(), l, loc);
}
