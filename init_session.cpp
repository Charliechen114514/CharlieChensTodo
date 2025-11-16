#include "MainPage/init/apoddatadefault.h"
#include "TaskProcessing/inits/databasepreinit.h"
#include "TaskProcessing/inits/taskiniter.h"
#include "init/todopreinithelper.h"
#include "todomainwindow.h"
#include <QApplication>
#include <QMessageBox>

ToDoPreInitHelper* createAndLoadHelper(TodoMainWindow* w) {
	ToDoPreInitHelper* helper = nullptr;
	try {
		QList<ToDoInitItem*> items = {
			new DataBasePreInit(w),
			new APODDataDefault(w)
		};
		helper = new ToDoPreInitHelper(items, w);
		helper->init();
	} catch (...) {
		QApplication::exit(-1);
	}

	return helper;
}

void initSession(TodoMainWindow* w) {
	ToDoPreInitHelper* init_helper = createAndLoadHelper(w);
	TaskRecordsHelpers* handler = nullptr;
	try {
		auto db_path = init_helper->query("todo_db");
		if (!db_path.isValid()) {
			throw std::runtime_error("Json Loader thinks db is incorrect");
		}
		handler = TaskIniter::sync_init(db_path.toString(), w);
		if (!handler) {
			QMessageBox::critical(w, "Error Occurs", "Can not find the database!");
			QApplication::exit(-1);
		}
	} catch (const std::exception& e) {
		QMessageBox::critical(w, "Error Occurs", e.what());
		QApplication::exit(-1);
	}
	w->late_init();
	w->setTaskHandler(handler);
	w->setIniter(init_helper);
}
