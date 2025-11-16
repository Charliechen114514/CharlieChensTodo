#ifndef TODOPREINITHELPER_H
#define TODOPREINITHELPER_H
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QVariant>

class ToDoInitItem;
class ToDoPreInitHelper : public QObject {
	Q_OBJECT
public:
	explicit ToDoPreInitHelper(
	    const QList<ToDoInitItem*>& items, QObject* parent = nullptr);
	~ToDoPreInitHelper();
	bool init();
	QVariant& query(const QString& conf);

	QString getConfig_json_path() const;
	void setConfig_json_path(const QString& newConfig_json_path);
	bool update(const QString& conf, const QVariant& newValue, bool saveNow = true);

signals:
	void initialized(bool success,
	                 const QString& conf,
	                 const QVariant& value);

	void initializedAll(bool success);
	void updated(const QString& conf, const QVariant& newValue);

private:
	bool checkConfigFileAccessible();
	bool loadIfConfigFileMissing();

private:
	QJsonObject config_json_object;
	QString config_json_path;
	QMap<QString, ToDoInitItem*> init_items;
	QMap<QString, QVariant> query_cache;
	bool saveConfigFile();
};

#endif // TODOPREINITHELPER_H
