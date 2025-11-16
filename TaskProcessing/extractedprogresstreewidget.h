#ifndef EXTRACTEDPROGRESSTREEWIDGET_H
#define EXTRACTEDPROGRESSTREEWIDGET_H

#include <QWidget>
class QTreeWidget;
class QTreeWidgetItem;

class ExtractedProgressTreeWidget : public QWidget {
	Q_OBJECT
public:
	ExtractedProgressTreeWidget(QTreeWidget* source, QWidget* parent = nullptr);

public slots:
	void rebuild();

private slots:
	void onSourceItemChanged(QTreeWidgetItem* /*it*/, int /*col*/);

private:
	QTreeWidget* monitored_tree = nullptr;
	QTreeWidget* progress_tree = nullptr;

	void countDescendantsCheckable(QTreeWidgetItem* node, int& total, int& checked);
	bool hasCheckableDescendants(QTreeWidgetItem* node);

	QTreeWidgetItem* copyIfHasCheckableDescendants(QTreeWidgetItem* srcNode, QTreeWidgetItem* dstParent);

	void addProgressBarForItem(QTreeWidgetItem* dstItem, int total, int checked);
};

#endif // EXTRACTEDPROGRESSTREEWIDGET_H
