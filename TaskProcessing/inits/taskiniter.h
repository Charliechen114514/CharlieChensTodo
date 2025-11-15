#ifndef TASKINITER_H
#define TASKINITER_H
#include <QString>
class TaskRecordsHelpers;
class TaskIniter {
public:
	static TaskRecordsHelpers* sync_init(const QString& database_path, QObject* parent);
};

#endif // TASKINITER_H
