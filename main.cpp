#include "loggy/loggerbackend.h"
#include "todomainwindow.h"
#include <QApplication>
#include <QDateTime>

void initSession(TodoMainWindow* w);

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	auto& logger_instance = LoggerBackend::instance();
	logger_instance.addLoggerFilePath("logger/logger" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt");

	TodoMainWindow w;
	initSession(&w);
	w.show();
	return a.exec();
}
