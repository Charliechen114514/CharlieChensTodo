#ifndef TASKRECORDINGS_H
#define TASKRECORDINGS_H
#include "taskrecords.h"
#include <QDate>
#include <QMap>
#include <QObject>
class TaskRecordings : public QObject {
	Q_OBJECT
public:
	explicit TaskRecordings(QObject* parent = nullptr);

	void register_tasks(const QString& field,
	                    std::shared_ptr<TaskRecords> tasks);
	void register_tasks(const int& field,
	                    std::shared_ptr<TaskRecords> tasks);
	void register_tasks(const QDate& field,
	                    std::shared_ptr<TaskRecords> tasks);

	std::shared_ptr<TaskRecords> get_task(const QString& field);
	std::shared_ptr<TaskRecords> get_task(const int& field);
	std::shared_ptr<TaskRecords> get_task(const QDate& field);

	bool remove_task(const QString& field);
	bool remove_task(const int& field);
	bool remove_task(const QDate& field);

private:
	QMap<QString, std::shared_ptr<TaskRecords>> field_records;
	QMap<int, std::shared_ptr<TaskRecords>> monthy_records;
	QMap<QDate, std::shared_ptr<TaskRecords>> date_records;
};

#endif // TASKRECORDINGS_H
