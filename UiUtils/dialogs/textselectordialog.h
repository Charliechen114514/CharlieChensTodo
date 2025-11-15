#ifndef TEXTSELECTORDIALOG_H
#define TEXTSELECTORDIALOG_H
#include "UiUtils_Global.h"
#include <QDialog>
class CCTextField;
class CCLabel;
class CCButton;

class UiUtils_EXPORT TextSelectorDialog : public QDialog {
	Q_OBJECT
public:
	explicit TextSelectorDialog(QWidget* parent = nullptr);
signals:
	void confirmed(const QString& text, const QString& markdownPath);

private:
	void onChooseFile();
	void onClear();
	void onAccept();
	void onCancel();
	void updateOkState();

private:
	CCTextField* textField;
	CCLabel* fileLabel_;
	CCButton* clearButton;
	QString filePath_;
};

#endif // TEXTSELECTORDIALOG_H
