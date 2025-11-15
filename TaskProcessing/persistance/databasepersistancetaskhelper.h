#pragma once

#include "persistancetaskhelper.h"
#include <QDate>
#include <QMap>
#include <QObject>
#include <QSqlDatabase>

class DataBasePersistanceTaskHelper : public PersistanceTaskHelper {
	Q_OBJECT
public:
	explicit DataBasePersistanceTaskHelper(const QString& dbFilePath,
	                                       QObject* parent = nullptr);
	~DataBasePersistanceTaskHelper() override;

	bool init() override;
	bool deinit() override;
	// ---------- fields ----------
	bool addField(const QString& field, const QString& mdPath) override;
	bool updateField(const QString& field, const QString& mdPath) override;
	bool removeField(const QString& field) override;
	QString getFieldPath(const QString& field) const override;
	QMap<QString, QString> getAllFields() const override;

	// ---------- months ----------
	bool addMonth(int month, const QString& mdPath) override;
	bool updateMonth(int month, const QString& mdPath) override;
	bool removeMonth(int month) override;
	QString getMonthPath(int month) const override;
	QMap<int, QString> getAllMonths() const override;
	bool monthExists(int month) const override;
	bool clearMonths() override;

	// ---------- dates ----------
	bool addDate(const QDate& date, const QString& mdPath) override;
	bool updateDate(const QDate& date, const QString& mdPath) override;
	bool removeDate(const QDate& date) override;
	QString getDatePath(const QDate& date) const override;
	QMap<QDate, QString> getAllDates() const override;

	QStringList allFields() const override;
	QList<int> allMonths() const override;
	QList<QDate> allDates() const override;

private:
	bool openDb();
	bool closeDb() noexcept;
	bool ensureTables();

private:
	QSqlDatabase db_;
	QString dbFilePath_;
};
