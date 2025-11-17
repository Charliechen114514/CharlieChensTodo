#include "loggy/loggerbackend.h"
#include "todomainwindow.h"
#include <QApplication>
#include <QDateTime>

void initSession(TodoMainWindow* w);

int main(int argc, char* argv[]) {
	auto& logger_instance = LoggerBackend::instance();
	logger_instance.addLoggerFilePath("logger/logger" + QDateTime::currentDateTime().toString() + ".txt");
	QApplication a(argc, argv);
	TodoMainWindow w;
	initSession(&w);
	w.show();
	return a.exec();
}
