#include "loggerbackend.h"
#include "CCLogger.hpp"
#include "formater/simplify_formarter.h"
#include "formater/standard_formater.h"
#include "io/fileio.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLoggingCategory>

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

std::unique_ptr<Clog::LoggerFormatter>
get_formater(const LoggerBackend::SupportFormat format) {
	switch (format) {
	case LoggerBackend::SupportFormat::Simplified:
		return std::make_unique<Clog::SimplifiedFormater>();
		break;
	case LoggerBackend::SupportFormat::Standard:
		return std::make_unique<Clog::StandardFormater>();
		break;
	case LoggerBackend::SupportFormat::OnlyContent:
		return std::make_unique<Clog::BlankFormater>();
		break;
	default:
		return std::make_unique<Clog::SimplifiedFormater>();
		break;
	}
}
}

LoggerBackend::LoggerBackend() {
	logger = new Clog::CCLogger;
	QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
	old_handler = qInstallMessageHandler(LoggerBackend::__qtToCCLoggerHandler);
}

LoggerBackend& LoggerBackend::instance() {
	static LoggerBackend b;
	return b;
}

void LoggerBackend::addLoggerFilePath(const QString& p, const SupportFormat format) {
	QFileInfo info(p);
	QDir dir = info.dir();

	if (!dir.exists()) {
		if (!dir.mkpath(".")) {
			qWarning() << "Failed to create directory for log file:" << dir.absolutePath();
			return;
		}
	}
	QFile f(p);
	if (!f.open(QIODevice::WriteOnly
	            | QIODevice::Truncate
	            | QIODevice::Text)) {
		qWarning() << "Cannot create file:" << p
		           << "=>" << f.errorString().toUtf8();
		return;
	}

	f.close();

	qInfo() << "Select the Formats type: " << static_cast<int>(format);
	std::unique_ptr<Clog::LoggerFormatter> formater = get_formater(format);

	QMutexLocker _(&m);
	logger->registerIOBackEnd(
	    std::make_unique<Clog::FileLoggerIO>(p.toStdString()),
	    std::move(formater));
	loggers_local_path.emplaceBack(p);
}

void LoggerBackend::registerIOBackEnd(std::unique_ptr<Clog::LoggerIO> backend, std::unique_ptr<Clog::LoggerFormatter> formater) {
	QMutexLocker _(&m);
	logger->registerIOBackEnd(
	    std::move(backend),
	    std::move(formater));
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
