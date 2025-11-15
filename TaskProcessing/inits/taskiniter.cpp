#include "taskiniter.h"
#include "persistance/databasepersistancetaskhelper.h"
#include "task_recordings/taskrecordshelpers.h"

TaskRecordsHelpers* TaskIniter::sync_init(
    const QString& database_path, QObject* parent) {
	DataBasePersistanceTaskHelper* database = new DataBasePersistanceTaskHelper(database_path, parent);
	TaskRecordsHelpers* helper = new TaskRecordsHelpers(database, parent);
	if (!helper->init()) {
		return nullptr;
	}
	return helper;
}
