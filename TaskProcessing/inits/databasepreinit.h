#ifndef DATABASEPREINIT_H
#define DATABASEPREINIT_H

#include "../init/todoinititem.h"

class DataBasePreInit : public ToDoInitItem {
public:
	explicit DataBasePreInit(QObject* parent = nullptr);
	QString itemName() const override { return "todo_db"; }
	bool loadIfAlready(const QVariant& v) override;
	bool checkIfIsItemInvalid(const QVariant& v) override;
	QVariant loadIfIsItemInvalid(const QVariant& v) override;
	QVariant loadIfKeyMissing() override;
	bool isCritical() override { return true; }

private:
	QString db_path;
};

#endif // DATABASEPREINIT_H
