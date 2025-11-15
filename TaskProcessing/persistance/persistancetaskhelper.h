#ifndef PERSISTANCETASKHELPER_H
#define PERSISTANCETASKHELPER_H

#include <QObject>

class PersistanceTaskHelper : public QObject {
	Q_OBJECT
public:
	explicit PersistanceTaskHelper(QObject* parent = nullptr);
	// init / deinits
	virtual bool init() = 0;
	virtual bool deinit() = 0;

	// ---------- fields ----------
	virtual bool addField(const QString& field, const QString& mdPath) = 0;
	virtual bool updateField(const QString& field, const QString& mdPath) = 0;
	virtual bool removeField(const QString& field) = 0;
	virtual QString getFieldPath(const QString& field) const = 0;
	virtual QMap<QString, QString> getAllFields() const = 0;
	virtual QStringList allFields() const = 0;

	// ---------- months ----------
	virtual bool addMonth(int month, const QString& mdPath) = 0;
	virtual bool updateMonth(int month, const QString& mdPath) = 0;
	virtual bool removeMonth(int month) = 0;
	virtual QString getMonthPath(int month) const = 0;
	virtual QMap<int, QString> getAllMonths() const = 0;
	virtual bool monthExists(int month) const = 0;
	virtual bool clearMonths() = 0;
	virtual QList<int> allMonths() const = 0;

	// ---------- dates ----------
	virtual bool addDate(const QDate& date, const QString& mdPath) = 0;
	virtual bool updateDate(const QDate& date, const QString& mdPath) = 0;
	virtual bool removeDate(const QDate& date) = 0;
	virtual QString getDatePath(const QDate& date) const = 0;
	virtual QMap<QDate, QString> getAllDates() const = 0;
	virtual QList<QDate> allDates() const = 0;
};

#endif // PERSISTANCETASKHELPER_H
