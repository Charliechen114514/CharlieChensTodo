#ifndef APODDATADEFAULT_H
#define APODDATADEFAULT_H
#include "../init/todoinititem.h"

class APODDataDefault : public ToDoInitItem {
public:
	explicit APODDataDefault(QObject* parent = nullptr);
	QString itemName() const override;
	bool loadIfAlready(const QVariant& v) override;
	bool checkIfIsItemInvalid(const QVariant& v) override;
	QVariant loadIfIsItemInvalid(const QVariant& v) override;
	QVariant loadIfKeyMissing() override;
	bool isCritical() override;
};

#endif // APODDATADEFAULT_H
