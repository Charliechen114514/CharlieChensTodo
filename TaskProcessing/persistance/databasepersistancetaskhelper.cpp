#include "databasepersistancetaskhelper.h"
#include <QDebug>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

DataBasePersistanceTaskHelper::DataBasePersistanceTaskHelper(const QString& dbFilePath,
                                                             QObject* parent)
    : PersistanceTaskHelper(parent)
    , db_(QSqlDatabase())
    , dbFilePath_(dbFilePath) {
}

DataBasePersistanceTaskHelper::~DataBasePersistanceTaskHelper() {
	DataBasePersistanceTaskHelper::deinit();
}

bool DataBasePersistanceTaskHelper::openDb() {
	if (db_.isValid() && db_.isOpen())
		return true;

	const QString connName = QStringLiteral("TaskHelper_%1").arg(reinterpret_cast<quintptr>(this));

	if (QSqlDatabase::contains(connName)) {
		db_ = QSqlDatabase::database(connName);
	} else {
		db_ = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connName);
		db_.setDatabaseName(dbFilePath_);
	}

	if (!db_.open()) {
		qWarning() << "Failed to open sqlite db:" << db_.lastError().text();
		return false;
	}
	return true;
}

bool DataBasePersistanceTaskHelper::closeDb() noexcept {
	if (!db_.isValid())
		return false;
	const QString connName = db_.connectionName();
	if (db_.isOpen())
		db_.close();
	db_ = QSqlDatabase();
	QSqlDatabase::removeDatabase(connName);
	return true;
}

bool DataBasePersistanceTaskHelper::ensureTables() {
	if (!openDb())
		return false;

	QSqlQuery q(db_);
	// fields: field TEXT PRIMARY KEY, path TEXT NOT NULL
	QString sql1 = QStringLiteral(
	    "CREATE TABLE IF NOT EXISTS fields ("
	    "field TEXT PRIMARY KEY,"
	    "path TEXT NOT NULL"
	    ");");

	// months: month INTEGER PRIMARY KEY, path TEXT NOT NULL
	QString sql2 = QStringLiteral(
	    "CREATE TABLE IF NOT EXISTS months ("
	    "month INTEGER PRIMARY KEY,"
	    "path TEXT NOT NULL"
	    ");");

	// dates: date TEXT PRIMARY KEY (ISO), path TEXT NOT NULL
	QString sql3 = QStringLiteral(
	    "CREATE TABLE IF NOT EXISTS dates ("
	    "date TEXT PRIMARY KEY,"
	    "path TEXT NOT NULL"
	    ");");

	if (!q.exec(sql1)) {
		qWarning() << "create fields failed:" << q.lastError().text();
		return false;
	}
	if (!q.exec(sql2)) {
		qWarning() << "create months failed:" << q.lastError().text();
		return false;
	}
	if (!q.exec(sql3)) {
		qWarning() << "create dates failed:" << q.lastError().text();
		return false;
	}
	return true;
}

bool DataBasePersistanceTaskHelper::init() {
	if (!openDb())
		return false;
	return ensureTables();
}

bool DataBasePersistanceTaskHelper::deinit() {
	if (!openDb())
		return false;
	return closeDb();
}

/* ---------- fields ---------- */

bool DataBasePersistanceTaskHelper::addField(const QString& field, const QString& mdPath) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("INSERT OR REPLACE INTO fields(field, path) VALUES(:f, :p);");
	q.bindValue(":f", field);
	q.bindValue(":p", mdPath);
	if (!q.exec()) {
		qWarning() << "addField failed:" << q.lastError().text();
		return false;
	}
	return true;
}

bool DataBasePersistanceTaskHelper::updateField(const QString& field, const QString& mdPath) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("UPDATE fields SET path = :p WHERE field = :f;");
	q.bindValue(":p", mdPath);
	q.bindValue(":f", field);
	if (!q.exec()) {
		qWarning() << "updateField failed:" << q.lastError().text();
		return false;
	}
	return q.numRowsAffected() > 0;
}

bool DataBasePersistanceTaskHelper::removeField(const QString& field) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("DELETE FROM fields WHERE field = :f;");
	q.bindValue(":f", field);
	if (!q.exec()) {
		qWarning() << "removeField failed:" << q.lastError().text();
		return false;
	}
	return true;
}

QString DataBasePersistanceTaskHelper::getFieldPath(const QString& field) const {
	if (!db_.isValid() || !db_.isOpen())
		return {};
	QSqlQuery q(db_);
	q.prepare("SELECT path FROM fields WHERE field = :f LIMIT 1;");
	q.bindValue(":f", field);
	if (!q.exec()) {
		qWarning() << "getFieldPath failed:" << q.lastError().text();
		return {};
	}
	if (q.next())
		return q.value(0).toString();
	return {};
}

QMap<QString, QString> DataBasePersistanceTaskHelper::getAllFields() const {
	QMap<QString, QString> res;
	if (!db_.isValid() || !db_.isOpen())
		return res;
	QSqlQuery q(db_);
	if (!q.exec("SELECT field, path FROM fields;"))
		return res;
	while (q.next()) {
		res.insert(q.value(0).toString(), q.value(1).toString());
	}
	return res;
}

/* ---------- months (全部操作接口) ---------- */

bool DataBasePersistanceTaskHelper::addMonth(int month, const QString& mdPath) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("INSERT OR REPLACE INTO months(month, path) VALUES(:m, :p);");
	q.bindValue(":m", month);
	q.bindValue(":p", mdPath);
	if (!q.exec()) {
		qWarning() << "addMonth failed:" << q.lastError().text();
		return false;
	}
	return true;
}

bool DataBasePersistanceTaskHelper::updateMonth(int month, const QString& mdPath) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("UPDATE months SET path = :p WHERE month = :m;");
	q.bindValue(":p", mdPath);
	q.bindValue(":m", month);
	if (!q.exec()) {
		qWarning() << "updateMonth failed:" << q.lastError().text();
		return false;
	}
	return q.numRowsAffected() > 0;
}

bool DataBasePersistanceTaskHelper::removeMonth(int month) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("DELETE FROM months WHERE month = :m;");
	q.bindValue(":m", month);
	if (!q.exec()) {
		qWarning() << "removeMonth failed:" << q.lastError().text();
		return false;
	}
	return true;
}

QString DataBasePersistanceTaskHelper::getMonthPath(int month) const {
	if (!db_.isValid() || !db_.isOpen())
		return {};
	QSqlQuery q(db_);
	q.prepare("SELECT path FROM months WHERE month = :m LIMIT 1;");
	q.bindValue(":m", month);
	if (!q.exec()) {
		qWarning() << "getMonthPath failed:" << q.lastError().text();
		return {};
	}
	if (q.next())
		return q.value(0).toString();
	return {};
}

QMap<int, QString> DataBasePersistanceTaskHelper::getAllMonths() const {
	QMap<int, QString> res;
	if (!db_.isValid() || !db_.isOpen())
		return res;
	QSqlQuery q(db_);
	if (!q.exec("SELECT month, path FROM months;"))
		return res;
	while (q.next()) {
		res.insert(q.value(0).toInt(), q.value(1).toString());
	}
	return res;
}

bool DataBasePersistanceTaskHelper::monthExists(int month) const {
	if (!db_.isValid() || !db_.isOpen())
		return false;
	QSqlQuery q(db_);
	q.prepare("SELECT 1 FROM months WHERE month = :m LIMIT 1;");
	q.bindValue(":m", month);
	if (!q.exec()) {
		qWarning() << "monthExists failed:" << q.lastError().text();
		return false;
	}
	return q.next();
}

bool DataBasePersistanceTaskHelper::clearMonths() {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	if (!q.exec("DELETE FROM months;")) {
		qWarning() << "clearMonths failed:" << q.lastError().text();
		return false;
	}
	return true;
}

/* ---------- dates ---------- */

bool DataBasePersistanceTaskHelper::addDate(const QDate& date, const QString& mdPath) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("INSERT OR REPLACE INTO dates(date, path) VALUES(:d, :p);");
	q.bindValue(":d", date.toString(Qt::ISODate));
	q.bindValue(":p", mdPath);
	if (!q.exec()) {
		qWarning() << "addDate failed:" << q.lastError().text();
		return false;
	}
	return true;
}

bool DataBasePersistanceTaskHelper::updateDate(const QDate& date, const QString& mdPath) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("UPDATE dates SET path = :p WHERE date = :d;");
	q.bindValue(":p", mdPath);
	q.bindValue(":d", date.toString(Qt::ISODate));
	if (!q.exec()) {
		qWarning() << "updateDate failed:" << q.lastError().text();
		return false;
	}
	return q.numRowsAffected() > 0;
}

bool DataBasePersistanceTaskHelper::removeDate(const QDate& date) {
	if (!openDb())
		return false;
	QSqlQuery q(db_);
	q.prepare("DELETE FROM dates WHERE date = :d;");
	q.bindValue(":d", date.toString(Qt::ISODate));
	if (!q.exec()) {
		qWarning() << "removeDate failed:" << q.lastError().text();
		return false;
	}
	return true;
}

QString DataBasePersistanceTaskHelper::getDatePath(const QDate& date) const {
	if (!db_.isValid() || !db_.isOpen())
		return {};
	QSqlQuery q(db_);
	q.prepare("SELECT path FROM dates WHERE date = :d LIMIT 1;");
	q.bindValue(":d", date.toString(Qt::ISODate));
	if (!q.exec()) {
		qWarning() << "getDatePath failed:" << q.lastError().text();
		return {};
	}
	if (q.next())
		return q.value(0).toString();
	return {};
}

QMap<QDate, QString> DataBasePersistanceTaskHelper::getAllDates() const {
	QMap<QDate, QString> res;
	if (!db_.isValid() || !db_.isOpen())
		return res;
	QSqlQuery q(db_);
	if (!q.exec("SELECT date, path FROM dates;"))
		return res;
	while (q.next()) {
		res.insert(QDate::fromString(q.value(0).toString(), Qt::ISODate), q.value(1).toString());
	}
	return res;
}

QStringList DataBasePersistanceTaskHelper::allFields() const {
	QStringList results;
	if (!db_.isOpen())
		return results;

	QSqlQuery query(db_);
	query.prepare("SELECT field FROM fields ORDER BY field ASC;"); // 修改表名为 fields
	if (!query.exec()) {
		qWarning() << "[DB] allFields() error:" << query.lastError();
		return results;
	}

	while (query.next())
		results.append(query.value(0).toString());

	return results;
}

QList<int> DataBasePersistanceTaskHelper::allMonths() const {
	QList<int> results;
	if (!db_.isOpen())
		return results;

	QSqlQuery query(db_);
	query.prepare("SELECT month FROM months ORDER BY month ASC;"); // 修改表名为 months
	if (!query.exec()) {
		qWarning() << "[DB] allMonths() error:" << query.lastError();
		return results;
	}

	while (query.next())
		results.append(query.value(0).toInt());

	return results;
}

QList<QDate> DataBasePersistanceTaskHelper::allDates() const {
	QList<QDate> results;
	if (!db_.isOpen())
		return results;

	QSqlQuery query(db_);
	query.prepare("SELECT date FROM dates ORDER BY date ASC;"); // 修改表名为 dates
	if (!query.exec()) {
		qWarning() << "[DB] allDates() error:" << query.lastError();
		return results;
	}

	while (query.next()) {
		QString dateStr = query.value(0).toString();
		QDate d = QDate::fromString(dateStr, Qt::ISODate);

		if (d.isValid())
			results.append(d);
		else
			qWarning() << "[DB] Invalid date format in DB:" << dateStr;
	}

	return results;
}
