#include "loggersubwindow.h"
#include "CCTextField/cctextfield.h"
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QVBoxLayout>

LoggerSubWindow::LoggerSubWindow(QWidget* parent)
    : QMainWindow { parent } {
	QVBoxLayout* v = new QVBoxLayout(this);
	field = new CCTextField(this);
	field->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	v->addWidget(field);
	watcher = new QFileSystemWatcher(this);
	connect(watcher, &QFileSystemWatcher::fileChanged,
	        this, &LoggerSubWindow::onReloadFile);

	resize(1024, 800);
}

void LoggerSubWindow::register_monitor_window(const QString& f) {
	QFileInfo inf(f);
	if (!inf.exists()) {
		qWarning() << "Can not set the monitoring path due to " << f << " is not exsited!";
		return;
	}
	if (!monitoring_logger_path.isEmpty()) {
		watcher->removePath(monitoring_logger_path);
	}

	watcher->addPath(f);
	qInfo() << "Register the path: " << f;
	monitoring_logger_path = f;
	return;
}

void LoggerSubWindow::openMe() {
	QFileInfo inf(monitoring_logger_path);
	if (!inf.exists()) {
		qWarning() << "Can not set the monitoring path due to " << monitoring_logger_path << " is not exsited!";
		QMessageBox::critical(this, "Error", "Monitoring File Missing: " + monitoring_logger_path);
		return;
	}

	this->show();
}

void LoggerSubWindow::onReloadFile(const QString& path) {
	qDebug() << "[Watcher] Log file changed, Load to the subWindow once";

	if (!watcher->files().contains(monitoring_logger_path)) {
		watcher->addPath(monitoring_logger_path);
	}

	QFile f(monitoring_logger_path);
	if (f.open(QFile::ReadOnly | QFile::Text)) {
		QString content = f.readAll();
		field->setText(content);
	}
}
