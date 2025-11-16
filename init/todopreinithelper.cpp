#include "todopreinithelper.h"
#include "todoinititem.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

ToDoPreInitHelper::ToDoPreInitHelper(
    const QList<ToDoInitItem*>& items, QObject* parent)
    : QObject { parent } {

	if (items.empty()) {
		throw std::runtime_error("init session is empty, this is unexpected!");
	}

	config_json_path = QDir(QCoreApplication::applicationDirPath()).filePath("configs/config.json");

	// populate map, keep the shared_ptr ownership outside
	for (const auto& it : items) {
		if (it) {
			// ensure QObject parent is set so items can qobject_cast back to helper if needed
			if (!it->parent())
				it->setParent(this);
			init_items.insert(it->itemName(), it);
		}
	}
}

ToDoPreInitHelper::~ToDoPreInitHelper() {
	try {
		saveConfigFile();
	} catch (const std::exception& e) {
		QMessageBox::critical(nullptr, "Error",
		                      QString("Error Occurs, file will not save") + e.what());
	}
}

QVariant& ToDoPreInitHelper::query(const QString& conf) {
	// Keep a live QVariant cache that callers can take a reference to.
	// Initialize from config_json_object when empty.
	if (!query_cache.contains(conf)) {
		if (config_json_object.contains(conf)) {
			query_cache.insert(conf, config_json_object.value(conf).toVariant());
		} else {
			query_cache.insert(conf, QVariant());
		}
	}
	return query_cache[conf];
}

bool ToDoPreInitHelper::init() {
	// Ok, we check as followings,

	try {
		checkConfigFileAccessible();

		QFile f(config_json_path);
		if (f.exists()) {
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
				throw std::runtime_error("Can not open the config json after the creation, these is unexpected!");
			}
			QByteArray data = f.readAll();
			f.close();
			QJsonParseError err;
			QJsonDocument doc = QJsonDocument::fromJson(data, &err);
			if (err.error != QJsonParseError::NoError || !doc.isObject()) {
				qWarning() << "Config JSON parse error:" << err.errorString();
				// recreate
				loadIfConfigFileMissing();
			} else {
				config_json_object = doc.object();
			}
		}

		// Build initial query_cache from config
		query_cache.clear();
		for (auto it = config_json_object.begin(); it != config_json_object.end(); ++it) {
			query_cache.insert(it.key(), it.value().toVariant());
		}

		// 3. Iterate items and perform their flows
		bool all_ok = true;
		auto keys = init_items.keys();
		for (const auto& key : std::as_const(keys)) {
			auto item = init_items.value(key);
			if (!item)
				continue;

			// Give item an opportunity to check general
			// state via parent access if needed
			// If config key exists
			if (config_json_object.contains(item->itemName())) {
				QVariant v = config_json_object.value(item->itemName()).toVariant();
				// invalid value?
				if (item->checkIfIsItemInvalid(v)) {
					auto new_variant = item->loadIfIsItemInvalid(v);

					bool ok = new_variant.isValid();
					if (!ok && item->isCritical()) {
						QMessageBox::critical(nullptr, "Error",
						                      "As the Item is Critical, Application Crashed!");
						QApplication::exit(-1);
					}
					query_cache.insert(item->itemName(), new_variant);
					config_json_object.insert(item->itemName(), QJsonValue::fromVariant(new_variant));
					emit initialized(ok, item->itemName(), query(item->itemName()));
					all_ok &= ok;
				} else {
					// already valid in config,
					// but item may still need to perform load actions
					bool ok = item->loadIfAlready(v);
					if (!ok && !item->isCritical()) {
						QMessageBox::critical(nullptr, "Error",
						                      "As the Item is Critical, Application Crashed!");
						QApplication::exit(-1);
					}
					emit initialized(ok, item->itemName(), query(item->itemName()));
					all_ok &= ok;
				}
			} else {
				// key missing
				QVariant newv = item->loadIfKeyMissing();
				// store into cache & json
				query_cache.insert(item->itemName(), newv);
				config_json_object.insert(item->itemName(), QJsonValue::fromVariant(newv));
				emit initialized(true, item->itemName(), query(item->itemName()));
			}
		}

		// 4. Save any updates back to file
		if (!saveConfigFile()) {
			qWarning() << "Failed to save updated config file";
			all_ok = false;
		}
		emit initializedAll(all_ok);
		return all_ok;
	} catch (const std::exception& e) {
		auto result = QMessageBox::information(
		    nullptr, "Config json error",
		    "config json error: " + QString(e.what()) + "These will create a new config json,"
		                                                "Continue To Create?");
		if (result != QMessageBox::Yes) {
			QApplication::exit(3);
		} else {
			try {
				loadIfConfigFileMissing();
			} catch (const std::exception& e) {
				QMessageBox::information(
				    nullptr, "Create json error",
				    "config json error: " + QString(e.what()));
				QApplication::exit(-1);
			}
		}

		return false;
	}
}

bool ToDoPreInitHelper::checkConfigFileAccessible() {
	QFile f(config_json_path);
	if (!f.exists())
		return false;
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw std::runtime_error("Cannot open config file:" + config_json_path.toStdString());
	}
	QByteArray data = f.readAll();
	f.close();

	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(data, &err);
	if (err.error != QJsonParseError::NoError || !doc.isObject()) {
		throw std::runtime_error("error parsing: " + err.errorString().toStdString());
	}

	config_json_object = doc.object();
	return true;
}

bool ToDoPreInitHelper::loadIfConfigFileMissing() {
	QDir dir(QFileInfo(config_json_path).absoluteDir());
	if (!dir.exists()) {
		if (!QDir().mkpath(dir.absolutePath())) {
			throw std::runtime_error("Failed to create configs folder:" + dir.absolutePath().toStdString());
		}
	}

	QJsonObject base;
	QFile f(config_json_path);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw std::runtime_error("Cannot open config file:" + config_json_path.toStdString());
	}
	QJsonDocument doc(base);
	f.write(doc.toJson(QJsonDocument::Indented));
	f.close();
	config_json_object = base;
	return true;
}

bool ToDoPreInitHelper::saveConfigFile() {
	// sync query_cache back to config_json_object
	for (auto it = query_cache.begin(); it != query_cache.end(); ++it) {
		config_json_object.insert(it.key(), QJsonValue::fromVariant(it.value()));
	}

	QDir cfgDir = QFileInfo(config_json_path).absoluteDir();
	if (!cfgDir.exists()) {
		if (!QDir().mkpath(cfgDir.absolutePath())) {
			throw std::runtime_error("Failed to create configs folder:" + cfgDir.absolutePath().toStdString());
		}
	}

	QFile f(config_json_path);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		throw std::runtime_error("Cannot open config file:" + config_json_path.toStdString());
	}

	QJsonDocument doc(config_json_object);
	f.write(doc.toJson(QJsonDocument::Indented));
	f.close();
	return true;
}

QString ToDoPreInitHelper::getConfig_json_path() const {
	return config_json_path;
}

void ToDoPreInitHelper::setConfig_json_path(const QString& newConfig_json_path) {
	config_json_path = newConfig_json_path;
}

bool ToDoPreInitHelper::update(const QString& conf, const QVariant& newValue, bool saveNow) {
	if (!config_json_object.contains(conf)) {
		qWarning() << "Config key not found:" << conf;
		return false;
	}

	config_json_object.insert(conf, QJsonValue::fromVariant(newValue));
	query_cache[conf] = newValue;

	if (saveNow) {
		if (!saveConfigFile()) {
			qWarning() << "Failed to save config file after update.";
			return false;
		}
	}

	emit updated(conf, newValue);

	return true;
}
