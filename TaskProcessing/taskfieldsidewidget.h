#ifndef TASKFIELDSIDEWIDGET_H
#define TASKFIELDSIDEWIDGET_H
#include <QWidget>
class QVBoxLayout;
class QPushButton;
class CCButton;

class TaskFieldSideWidget : public QWidget {
	Q_OBJECT
public:
	explicit TaskFieldSideWidget(QWidget* parent = nullptr);

	void setFields(const QStringList& l);
	void selectField(const QString& field);
	void addField(const QString& field);
	void removeField(const QString& field);

signals:
	void fieldClicked(const QString& field);
	void request_new_field_add();

private:
	void clearButtons();

private:
	QVBoxLayout* v;
	QString currentField_;
	QMap<QString, CCButton*> buttons_;
};

#endif // TASKFIELDSIDEWIDGET_H
