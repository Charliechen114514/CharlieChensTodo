#include "databasepreinit.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QUuid>

namespace {
bool tryOpenSqlite(const QString& path, QString* outErr = nullptr) {
	if (path.isEmpty()) {
		if (outErr)
			*outErr = "empty path";
		return false;
	}
	QFileInfo fi(path);
	if (!fi.exists() || !fi.isFile()) {
		if (outErr)
			*outErr = QString("file does not exist or not a file: %1").arg(path);
		return false;
	}

	QString conn = QStringLiteral("__db_check__") + QUuid::createUuid().toString();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", conn);
	db.setDatabaseName(path);
	bool ok = db.open();
	if (!ok) {
		if (outErr)
			*outErr = db.lastError().text();
		QSqlDatabase::removeDatabase(conn);
		return false;
	}
	db.close();
	db = QSqlDatabase();
	QSqlDatabase::removeDatabase(conn);
	return true;
}
}

DataBasePreInit::DataBasePreInit(QObject* parent)
    : ToDoInitItem { parent } {
}

bool DataBasePreInit::loadIfAlready(const QVariant& v) {
	QString p = v.toString();
	QString err;
	bool ok = tryOpenSqlite(p, &err);
	if (!ok) {
		// treat as invalid
		return false;
	}
	db_path = p;
	emit initialized(true, db_path);
	return true;
}

bool DataBasePreInit::checkIfIsItemInvalid(const QVariant& v) {
	QString p = v.toString();
	QString err;
	bool ok = tryOpenSqlite(p, &err);
	return !ok;
}

QVariant DataBasePreInit::loadIfIsItemInvalid(const QVariant& v) {
	Q_UNUSED(v);

	QMessageBox msgBox;
	msgBox.setWindowTitle(QObject::tr("Initialize Todo Database"));
	msgBox.setText(QObject::tr("Do you want to create a new todo database or select an existing one?"));
	auto createBtn = msgBox.addButton(QObject::tr("Create new"), QMessageBox::AcceptRole);
	auto chooseBtn = msgBox.addButton(QObject::tr("Select existing"), QMessageBox::ActionRole);
	auto cancelBtn = msgBox.addButton(QObject::tr("Cancel"), QMessageBox::RejectRole);

	msgBox.exec();

	if (msgBox.clickedButton() == cancelBtn) {
		return {};
	}

	if (msgBox.clickedButton() == createBtn) {

		return loadIfKeyMissing();
	}

	// Ask user to select an existing todo.db file
	QString title = QObject::tr("Select existing todo.db file");
	QString filter = QObject::tr("SQLite DB (todo.db);;All DB Files (*.db *.sqlite);;All Files (*)");
	QString file = QFileDialog::getOpenFileName(nullptr, title, QDir::homePath(), filter);
	if (file.isEmpty())
		return {}; // user cancelled

	// if filename is not todo.db, ask user to confirm
	if (QFileInfo(file).fileName() != QLatin1String("todo.db")) {
		auto btn = QMessageBox::question(nullptr, QObject::tr("Filename not todo.db"),
		                                 QObject::tr("Selected file is not named 'todo.db'. Use it anyway?"));
		if (btn != QMessageBox::Yes)
			return {};
	}

	QString err;
	if (!tryOpenSqlite(file, &err)) {
		QMessageBox::critical(nullptr, QObject::tr("Invalid DB"), QObject::tr("Selected file is not a valid/openable SQLite DB:%1").arg(err));
		return {};
	}

	db_path = file;

	emit initialized(true, db_path);
	return true;
}

QVariant DataBasePreInit::loadIfKeyMissing() {
	// Ask user to choose folder to create todo.db in
	QString title = QObject::tr("Select folder to create todo.db");
	QString folder = QFileDialog::getExistingDirectory(nullptr, title, QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (folder.isEmpty())
		return QVariant(); // user cancelled

	QDir d(folder);
	QString newDbPath = d.filePath("todo.db");

	if (QFile::exists(newDbPath)) {
		auto btn = QMessageBox::question(nullptr, QObject::tr("File Exists"), QObject::tr("A file named todo.db already exists in the selected folder. Do you want to use it?"));
		if (btn != QMessageBox::Yes)
			return QVariant();
		// validate
		QString err;
		if (!tryOpenSqlite(newDbPath, &err)) {
			QMessageBox::critical(nullptr, QObject::tr("Invalid DB"), QObject::tr("Existing file is not a valid sqlite DB:%1").arg(err));
			return QVariant();
		}
	} else {
		// create an empty sqlite database
		QString conn = QStringLiteral("__todo_create_conn__") + QUuid::createUuid().toString();
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", conn);
		db.setDatabaseName(newDbPath);
		if (!db.open()) {
			QMessageBox::critical(nullptr, QObject::tr("Create DB Failed"), QObject::tr("Failed to create sqlite file at:%1").arg(newDbPath));
			QSqlDatabase::removeDatabase(conn);
			return QVariant();
		}
		db.close();
		QSqlDatabase::removeDatabase(conn);
	}

	db_path = newDbPath;
	emit initialized(true, db_path);
	return QVariant(db_path);
}
