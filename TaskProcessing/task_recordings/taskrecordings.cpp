#include "taskrecordings.h"

TaskRecordings::TaskRecordings(QObject* parent)
    : QObject(parent) {
}

void TaskRecordings::register_tasks(const QString& field,
                                    std::shared_ptr<TaskRecords> tasks) {
	field_records[field] = std::move(tasks);
}

void TaskRecordings::register_tasks(const int& field,
                                    std::shared_ptr<TaskRecords> tasks) {
	monthy_records[field] = std::move(tasks);
}

void TaskRecordings::register_tasks(const QDate& field,
                                    std::shared_ptr<TaskRecords> tasks) {
	date_records[field] = std::move(tasks);
}

std::shared_ptr<TaskRecords> TaskRecordings::get_task(const QString& field) {
	return field_records.value(field, nullptr);
}

std::shared_ptr<TaskRecords> TaskRecordings::get_task(const int& field) {
	return monthy_records.value(field, nullptr);
}

std::shared_ptr<TaskRecords> TaskRecordings::get_task(const QDate& field) {
	return date_records.value(field, nullptr);
}

bool TaskRecordings::remove_task(const QString& field) {
	return field_records.remove(field) > 0;
}

bool TaskRecordings::remove_task(const int& field) {
	return monthy_records.remove(field) > 0;
}

bool TaskRecordings::remove_task(const QDate& field) {
	return date_records.remove(field) > 0;
}
