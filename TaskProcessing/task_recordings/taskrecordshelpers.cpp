#include "task_recordings/taskrecordshelpers.h"
#include "persistance/persistancetaskhelper.h"
#include "task_recordings/taskrecordings.h"
#include "task_recordings/taskrecords.h"
#include "task_recordings/taskrecordshelpers.h"
#include <QDebug>

TaskRecordsHelpers::TaskRecordsHelpers(PersistanceTaskHelper* dbHelper,
                                       QObject* parent)
    : dbHelper_(dbHelper)
    , recordings_(new TaskRecordings(this))
    , QObject { parent } {
}

TaskRecordsHelpers::~TaskRecordsHelpers() {
}

bool TaskRecordsHelpers::init() {
	if (!dbHelper_) {
		emit persistenceError(QStringLiteral("No DB helper provided"));
		emit initialized(false);
		return false;
	}

	if (!dbHelper_->init()) {
		emit persistenceError(QStringLiteral("DB init failed"));
		emit initialized(false);
		return false;
	}

	bool ok = loadAllFromPersistance();
	emit initialized(ok);
	return ok;
}

bool TaskRecordsHelpers::deinit() {
	return dbHelper_->deinit();
}

std::shared_ptr<TaskRecords> TaskRecordsHelpers::makeRecordsFromPath(const QString& path) const {
	if (path.isEmpty())
		return nullptr;
	return std::make_shared<TaskRecords>(path);
}

/* ---------- field ---------- */
bool TaskRecordsHelpers::registerField(const QString& field, std::shared_ptr<TaskRecords> records) {
	if (!dbHelper_ || !recordings_ || !records)
		return false;
	const QString path = records->getTaskFilePath();
	if (!dbHelper_->addField(field, path)) {
		emit persistenceError(QStringLiteral("addField failed: %1").arg(field));
		return false;
	}

	QMutexLocker locker(&mutex_);
	recordings_->register_tasks(field, records);
	return true;
}

std::shared_ptr<TaskRecords> TaskRecordsHelpers::getField(const QString& field) {
	if (!recordings_ || !dbHelper_)
		return nullptr;

	{
		QMutexLocker locker(&mutex_);
		auto r = recordings_->get_task(field);
		if (r)
			return r;
	}

	QString path = dbHelper_->getFieldPath(field);
	if (path.isEmpty())
		return nullptr;

	auto records = makeRecordsFromPath(path);
	if (records) {
		QMutexLocker locker(&mutex_);
		recordings_->register_tasks(field, records);
	}
	return records;
}

bool TaskRecordsHelpers::removeField(const QString& field) {
	if (!dbHelper_ || !recordings_)
		return false;
	if (!dbHelper_->removeField(field)) {
		emit persistenceError(QStringLiteral("removeField failed: %1").arg(field));
	}
	QMutexLocker locker(&mutex_);
	recordings_->remove_task(field);
	return true;
}

/* ---------- month ---------- */
bool TaskRecordsHelpers::registerMonth(int month, std::shared_ptr<TaskRecords> records) {
	if (!dbHelper_ || !recordings_ || !records)
		return false;
	const QString path = records->getTaskFilePath();
	if (!dbHelper_->addMonth(month, path)) {
		emit persistenceError(QStringLiteral("addMonth failed: %1").arg(month));
		return false;
	}
	QMutexLocker locker(&mutex_);
	recordings_->register_tasks(month, records);
	return true;
}

std::shared_ptr<TaskRecords> TaskRecordsHelpers::getMonth(int month) {
	if (!recordings_ || !dbHelper_)
		return nullptr;

	{
		QMutexLocker locker(&mutex_);
		auto r = recordings_->get_task(month);
		if (r)
			return r;
	}

	QString path = dbHelper_->getMonthPath(month);
	if (path.isEmpty())
		return nullptr;

	auto records = makeRecordsFromPath(path);
	if (records) {
		QMutexLocker locker(&mutex_);
		recordings_->register_tasks(month, records);
	}
	return records;
}

bool TaskRecordsHelpers::removeMonth(int month) {
	if (!dbHelper_ || !recordings_)
		return false;
	if (!dbHelper_->removeMonth(month)) {
		emit persistenceError(QStringLiteral("removeMonth failed: %1").arg(month));
	}
	QMutexLocker locker(&mutex_);
	recordings_->remove_task(month);
	return true;
}

/* ---------- date ---------- */
bool TaskRecordsHelpers::registerDate(const QDate& date, std::shared_ptr<TaskRecords> records) {
	if (!dbHelper_ || !recordings_ || !records)
		return false;
	const QString path = records->getTaskFilePath();
	if (!dbHelper_->addDate(date, path)) {
		emit persistenceError(QStringLiteral("addDate failed: %1").arg(date.toString(Qt::ISODate)));
		return false;
	}
	QMutexLocker locker(&mutex_);
	recordings_->register_tasks(date, records);
	return true;
}

std::shared_ptr<TaskRecords> TaskRecordsHelpers::getDate(const QDate& date) {
	if (!recordings_ || !dbHelper_)
		return nullptr;

	{
		QMutexLocker locker(&mutex_);
		auto r = recordings_->get_task(date);
		if (r)
			return r;
	}

	QString path = dbHelper_->getDatePath(date);
	if (path.isEmpty())
		return nullptr;

	auto records = makeRecordsFromPath(path);
	if (records) {
		QMutexLocker locker(&mutex_);
		recordings_->register_tasks(date, records);
	}
	return records;
}

bool TaskRecordsHelpers::removeDate(const QDate& date) {
	if (!dbHelper_ || !recordings_)
		return false;
	if (!dbHelper_->removeDate(date)) {
		emit persistenceError(QStringLiteral("removeDate failed: %1").arg(date.toString(Qt::ISODate)));
	}
	QMutexLocker locker(&mutex_);
	recordings_->remove_task(date);
	return true;
}

QStringList TaskRecordsHelpers::allFields() const {
	if (!dbHelper_)
		return {};
	return dbHelper_->allFields();
}

QList<int> TaskRecordsHelpers::allMonths() const {
	if (!dbHelper_)
		return {};
	return dbHelper_->allMonths();
}

QList<QDate> TaskRecordsHelpers::allDates() const {
	if (!dbHelper_)
		return {};
	return dbHelper_->allDates();
}

/* ---------- load all ---------- */
bool TaskRecordsHelpers::loadAllFromPersistance() {
	if (!dbHelper_ || !recordings_)
		return false;

	// fields
	{
		auto allFields = dbHelper_->getAllFields();
		for (auto it = allFields.constBegin(); it != allFields.constEnd(); ++it) {
			const QString field = it.key();
			const QString path = it.value();
			auto records = makeRecordsFromPath(path);
			QMutexLocker locker(&mutex_);
			recordings_->register_tasks(field, records);
		}
	}

	// months
	{
		auto allMonths = dbHelper_->getAllMonths();
		for (auto it = allMonths.constBegin(); it != allMonths.constEnd(); ++it) {
			int month = it.key();
			const QString path = it.value();
			auto records = makeRecordsFromPath(path);
			QMutexLocker locker(&mutex_);
			recordings_->register_tasks(month, records);
		}
	}

	// dates
	{
		auto allDates = dbHelper_->getAllDates();
		for (auto it = allDates.constBegin(); it != allDates.constEnd(); ++it) {
			QDate d = it.key();
			const QString path = it.value();
			auto records = makeRecordsFromPath(path);
			QMutexLocker locker(&mutex_);
			recordings_->register_tasks(d, records);
		}
	}

	return true;
}
