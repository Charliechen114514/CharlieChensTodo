#ifndef TODOMAINWINDOW_H
#define TODOMAINWINDOW_H

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui {
class TodoMainWindow;
}
QT_END_NAMESPACE

class CCToolBox;
class UiEventHandler;
class TaskProcessingWidgets;
class TaskRecordsHelpers;

class TodoMainWindow : public QMainWindow {
	Q_OBJECT

public:
	TodoMainWindow(QWidget* parent = nullptr);
	void late_init();
	void setTaskHandler(TaskRecordsHelpers* helpers);
	~TodoMainWindow();

protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	void setup_toolbox_panel();
	void setup_main_page();
	void setup_task_page();
	void init_connections();

private:
	Ui::TodoMainWindow* ui;
	TaskProcessingWidgets* taskWidget;
	CCToolBox* box;
	QMap<int, int> index_mappings;
};
#endif // TODOMAINWINDOW_H
