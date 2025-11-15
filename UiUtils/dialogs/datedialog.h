#ifndef DATEDIALOG_H
#define DATEDIALOG_H
#include "UiUtils_Global.h"
#include <QDialog>
class QCalendarWidget;
class QDateEdit;
class CCLabel;

class UiUtils_EXPORT DateDialog : public QDialog {
	Q_OBJECT
public:
	explicit DateDialog(QWidget* parent = nullptr);
signals:
	void confirmed(const QDate& date, const QString& markdownPath);

private slots:
	void onChooseFile();
	void onClear();
	void onAccept();
	void onCancel();
	void updateOkState();

private:
	QCalendarWidget* calendar_;
	QDateEdit* dateEdit_;
	CCLabel* fileLabel_;
	QPushButton* clearButton_;
	QString file_path;
};

#endif // DATEDIALOG_H
