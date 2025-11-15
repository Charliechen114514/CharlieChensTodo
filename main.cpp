
#include "todomainwindow.h"
#include <QApplication>

void initSession(TodoMainWindow* w);

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	TodoMainWindow w;
	initSession(&w);
	w.show();
	return a.exec();
}
