#ifndef LOGGERSUBWINDOW_H
#define LOGGERSUBWINDOW_H

#include <QMainWindow>
class CCTextField;
class QFileSystemWatcher;

class LoggerSubWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit LoggerSubWindow(QWidget* parent = nullptr);
	void register_monitor_window(const QString& f);
	void openMe();
private slots:
	void onReloadFile(const QString& path);

private:
	QFileSystemWatcher* watcher;
	CCTextField* field;
	QString monitoring_logger_path;
};

#endif // LOGGERSUBWINDOW_H
