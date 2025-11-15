#ifndef TASKPROCESSINGWIDGETS_H
#define TASKPROCESSINGWIDGETS_H
#include "../interfaces/IPagedWidget.h"
#include "task_recordings/taskrecords.h"
#include <QWidget>
class CCToolBox;
class CCTreeWidget;
class TaskSideWidget;
class TaskRecordsHelpers;
class TaskFieldSideWidget;
class UiEventHandler;

class TaskProcessingWidgets : public QWidget, IPagedWidget {
public:
	explicit TaskProcessingWidgets(QWidget* parent = nullptr);
	~TaskProcessingWidgets() override;
	UiEventHandler* ui_event_handler() const { return ui_handler; }
	void applyToToolBox(CCToolBox* toolbox,
	                    QStackedWidget* stacked_widget,
	                    QMap<int, int>& index_mappings) override;
	void setTaskHelpers(TaskRecordsHelpers* helpers);
	void preview_drug(const QUrl& file_path);

public slots:
	void collapseAllTasks(); // should be clear
	void expandAllTasks(); // expand all

	/**
	 * @brief parse_markdowns parsed as markdowns and shown issues
	 * @param markdown_bytes
	 */
	void parse_markdowns(const QString& markdown_bytes);
signals:

private slots:

	void handle_date_request(QDate date);
	void handle_field_request(QString field);

	void onNewFieldsTask(const QString field, std::shared_ptr<TaskRecords> t);
	void onNewDateTask(const QDate date, std::shared_ptr<TaskRecords> t);
	void onNewMonthyTask(const int month, std::shared_ptr<TaskRecords> t);
	void rewrite_current_file();

private:
	QString current_file_path;
	UiEventHandler* ui_handler;
	TaskRecordsHelpers* helpers;
	TaskSideWidget* sideWidget;
	TaskFieldSideWidget* fieldWidget;
	CCTreeWidget* task_lists_displays;
};

#endif // TASKPROCESSINGWIDGETS_H
