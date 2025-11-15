#ifndef YEARMONTHDIALOG_H
#define YEARMONTHDIALOG_H
#include "UiUtils_Global.h"
#include <QDialog>
class QCheckBox;
class QSpinBox;
class CCLabel;

class UiUtils_EXPORT YearMonthDialog : public QDialog {
	Q_OBJECT
public:
	explicit YearMonthDialog(QWidget* parent = nullptr);

signals:
	// Emitted with which parts are selected and their values
	void confirmed(int composed_month, const QString& markdownPath);

private slots:
	void onChooseFile();
	void onClear();
	void onAccept();
	void onCancel();
	void updateOkState();

private:
	QCheckBox* yearCheck;
	QSpinBox* yearSpin_;
	QCheckBox* month_check;
	QSpinBox* month_spin;

	CCLabel* fileLabel_;
	QPushButton* clearButton;
	QString filePath_;
};

#endif // YEARMONTHDIALOG_H
