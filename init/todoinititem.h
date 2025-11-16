#ifndef TODOINITITEM_H
#define TODOINITITEM_H
#include <QObject>
#include <QVariant>

class ToDoInitItem : public QObject {
	Q_OBJECT
public:
	explicit ToDoInitItem(QObject* parent = nullptr);
	// config Items Names
	virtual QString itemName() const = 0;

	virtual bool loadIfAlready(const QVariant& v) = 0;

	// if the config key is exsited but the value
	// is invalid
	virtual bool checkIfIsItemInvalid(const QVariant& v) = 0;
	virtual QVariant loadIfIsItemInvalid(const QVariant& v) = 0;

	// if the config key is missing, tell what should be the new seperate key
	virtual QVariant loadIfKeyMissing() = 0;

	virtual bool isCritical() = 0;

signals:
	void initialized(bool success, const QString& dbPath);
};

#endif // TODOINITITEM_H
