#ifndef TASKRECORDS_H
#define TASKRECORDS_H
#include <QString>

class TaskRecords {
public:
	TaskRecords(const QString& filePath);

	QString getTaskFilePath() const;
	void setTaskFilePath(const QString& newTaskFilePath);

private:
	QString taskFilePath;
};

#endif // TASKRECORDS_H
