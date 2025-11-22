#include "markdownpreviewwindow.h"
#include "CCMarkdownWidget/ccmarkdownbrowserwidget.h"
#include "url_reader.h"
#include <QMessageBox>

MarkdownPreviewWindow::MarkdownPreviewWindow(QWidget* parent)
    : QMainWindow { parent } {
	markdown_widget = new CCMarkdownBrowserWidget(this);
	setCentralWidget(markdown_widget);
}

bool MarkdownPreviewWindow::setMarkdownDisplayPath(
    const QString& markdown_path) {
	if (markdown_path.isEmpty()) {
		QMessageBox::information(this, "Preview Mode", "Current is on Preview Mode! So can not preview markdown in window");
		return false;
	}

	try {
		const QString md = cutils::readFromLocalPath(markdown_path);
		setMarkdown(md);
	} catch (const std::exception& e) {
		QMessageBox::critical(this, "Can not open markdown!", "Can not handle path: " + markdown_path + " due to reason:" + e.what());
		return false;
	}

	return true;
}

void MarkdownPreviewWindow::setMarkdown(const QString& markdown) {
	markdown_widget->setMarkdown(markdown);
}
