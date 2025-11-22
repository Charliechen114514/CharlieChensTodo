#ifndef TODOMAINWINDOW_H
#define TODOMAINWINDOW_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {
class TodoMainWindow;
}
QT_END_NAMESPACE

class CCToolBox;
class LoggerSubWindow;
class UiEventHandler;
class TaskProcessingWidgets;
class TaskRecordsHelpers;
class MainPageWidget;
class ToDoPreInitHelper;
class MarkdownPreviewWindow;

class TodoMainWindow : public QMainWindow {
	Q_OBJECT

public:
	TodoMainWindow(QWidget* parent = nullptr);
	void late_init();
	void setTaskHandler(TaskRecordsHelpers* helpers);
	void setIniter(ToDoPreInitHelper* initer);
	~TodoMainWindow();

public slots:
	void updateIniter(const QString key, const QVariant& v, bool request_update_now);
	void open_markdown_preview();
	void open_logger_window();

protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	void setup_self();
	void setup_toolbox_panel();
	void setup_main_page();
	void setup_task_page();
	void init_connections();
	void late_init_for_initer();

private:
	MarkdownPreviewWindow* preview_markdown;
	LoggerSubWindow* loggerWindow;
	ToDoPreInitHelper* initer;
	Ui::TodoMainWindow* ui;
	MainPageWidget* mainPageWidget;
	TaskProcessingWidgets* taskWidget;
	CCToolBox* box;
	QMap<int, int> index_mappings;
};
#endif // TODOMAINWINDOW_H
