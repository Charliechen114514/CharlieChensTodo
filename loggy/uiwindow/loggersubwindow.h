#ifndef LOGGERSUBWINDOW_H
#define LOGGERSUBWINDOW_H

#include <QMainWindow>
class QTextBrowser;
class QFileSystemWatcher;

class LoggerSubWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit LoggerSubWindow(QWidget* parent = nullptr);
	void openMe();
public slots:
	void appendLogger(const QString& logger);

private:
	QTextBrowser* field;
};

#endif // LOGGERSUBWINDOW_H
