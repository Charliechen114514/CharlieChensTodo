#include "loggersubwindow.h"
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QTextBrowser>
#include <QVBoxLayout>

LoggerSubWindow::LoggerSubWindow(QWidget* parent)
    : QMainWindow { parent } {
	field = new QTextBrowser(this);
	setCentralWidget(field);
	resize(1024, 800);
}

void LoggerSubWindow::openMe() {
	this->show();
}

void LoggerSubWindow::appendLogger(const QString& logger) {
	field->append(logger);
}
