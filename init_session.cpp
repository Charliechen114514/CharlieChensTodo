#include "TaskProcessing/inits/taskiniter.h"
#include "todomainwindow.h"
#include <QApplication>
#include <QMessageBox>
void initSession(TodoMainWindow* w) {
	TaskRecordsHelpers* handler = nullptr;
	try {
		handler = TaskIniter::sync_init("D:\\NewQtProjects\\CharliesTodo\\build\\databse\\todo.db", w);
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
}
