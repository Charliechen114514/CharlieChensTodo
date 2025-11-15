#ifndef FLOATINGLABELHELPER_H
#define FLOATINGLABELHELPER_H

#include <QWidget>
class CCLabel;
class CCImageWidget;
class FloatingLabelHelper : public QWidget {
	Q_OBJECT
public:
	explicit FloatingLabelHelper(QWidget* parent = nullptr);
	void setTitleText(const QString& text);
	void displayBusyText();
	void auto_adjust_text_color(CCImageWidget* widget);
	void setTextColor(const QColor& c);
	void setFontSize(int fontSize);
signals:

private:
	void updateTextColor();

private:
	CCLabel* label;
};

#endif // FLOATINGLABELHELPER_H
