#include "taskrecords.h"

TaskRecords::TaskRecords(const QString& filePath)
    : taskFilePath(filePath) {
}

QString TaskRecords::getTaskFilePath() const {
	return taskFilePath;
}

void TaskRecords::setTaskFilePath(const QString& newTaskFilePath) {
	taskFilePath = newTaskFilePath;
}
