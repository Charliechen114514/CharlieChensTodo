#ifndef TASKRECORDSHELPERS_H
#define TASKRECORDSHELPERS_H
#include <QDate>
#include <QMutex>
#include <QString>

class TaskRecords;
class TaskRecordings;
class PersistanceTaskHelper;

class TaskRecordsHelpers : public QObject {
	Q_OBJECT
public:
	TaskRecordsHelpers(PersistanceTaskHelper* persistance_helper, QObject* parent);
	~TaskRecordsHelpers();
	bool init();
	bool deinit();

	// ---------- field (QString) ----------
	bool registerField(const QString& field, std::shared_ptr<TaskRecords> records);
	std::shared_ptr<TaskRecords> getField(const QString& field);
	bool removeField(const QString& field);

	// ---------- month (int) ----------
	bool registerMonth(int month, std::shared_ptr<TaskRecords> records);
	std::shared_ptr<TaskRecords> getMonth(int month);
	bool removeMonth(int month);

	// ---------- date (QDate) ----------
	bool registerDate(const QDate& date, std::shared_ptr<TaskRecords> records);
	std::shared_ptr<TaskRecords> getDate(const QDate& date);
	bool removeDate(const QDate& date);

	QStringList allFields() const;
	QList<int> allMonths() const;
	QList<QDate> allDates() const;

	bool loadAllFromPersistance();

signals:
	void initialized(bool ok);
	void persistenceError(const QString& what);

private:
	PersistanceTaskHelper* dbHelper_;
	// memory storage
	TaskRecordings* recordings_;
	QMutex mutex_;

	std::shared_ptr<TaskRecords> makeRecordsFromPath(const QString& path) const;
};

#endif // TASKRECORDSHELPERS_H
