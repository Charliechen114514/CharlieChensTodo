#ifndef MARKDOWNPREVIEWWINDOW_H
#define MARKDOWNPREVIEWWINDOW_H

#include <QMainWindow>
class CCMarkdownBrowserWidget;

class MarkdownPreviewWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MarkdownPreviewWindow(QWidget* parent = nullptr);
	bool setMarkdownDisplayPath(const QString& markdown_path);
	void setMarkdown(const QString& markdown);

signals:

private:
	CCMarkdownBrowserWidget* markdown_widget;
};

#endif // MARKDOWNPREVIEWWINDOW_H
