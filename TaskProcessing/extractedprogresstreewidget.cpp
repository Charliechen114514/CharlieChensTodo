#include "extractedprogresstreewidget.h"
#include "CCProgressBar/progressbar.h"
#include "cclabel.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

ExtractedProgressTreeWidget::ExtractedProgressTreeWidget(QTreeWidget* source, QWidget* parent)
    : QWidget(parent)
    , monitored_tree(source) {
	QVBoxLayout* v = new QVBoxLayout(this);
	CCLabel* l = new CCLabel("Progress", this);
	v->addWidget(l);
	progress_tree = new QTreeWidget;
	progress_tree->setColumnCount(2);
	progress_tree->setHeaderLabels({ tr("Item"), tr("Progress") });
	progress_tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	progress_tree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	v->addWidget(progress_tree);

	rebuild();

	if (monitored_tree) {
		connect(monitored_tree, &QTreeWidget::itemChanged, this, &ExtractedProgressTreeWidget::onSourceItemChanged);
	}
}

void ExtractedProgressTreeWidget::rebuild() {
	progress_tree->clear();

	if (!monitored_tree)
		return;

	for (int i = 0; i < monitored_tree->topLevelItemCount(); ++i) {
		QTreeWidgetItem* srcTop = monitored_tree->topLevelItem(i);
		QTreeWidgetItem* dstTop = copyIfHasCheckableDescendants(srcTop, nullptr);
		if (dstTop) {
			progress_tree->addTopLevelItem(dstTop);
			int total = 0, checked = 0;
			countDescendantsCheckable(srcTop, total, checked);
			addProgressBarForItem(dstTop, total, checked);
		}
	}
}

void ExtractedProgressTreeWidget::onSourceItemChanged(QTreeWidgetItem*, int) {
	rebuild();
}

void ExtractedProgressTreeWidget::countDescendantsCheckable(QTreeWidgetItem* node, int& total, int& checked) {
	if (!node)
		return;
	for (int i = 0; i < node->childCount(); ++i) {
		QTreeWidgetItem* c = node->child(i);
		if (c->flags() & Qt::ItemIsUserCheckable) {
			++total;
			if (c->checkState(0) == Qt::Checked)
				++checked;
		}
		if (c->childCount() > 0)
			countDescendantsCheckable(c, total, checked);
	}
}

bool ExtractedProgressTreeWidget::hasCheckableDescendants(QTreeWidgetItem* node) {
	if (!node)
		return false;
	int total = 0, checked = 0;
	countDescendantsCheckable(node, total, checked);
	return total > 0;
}

QTreeWidgetItem* ExtractedProgressTreeWidget::copyIfHasCheckableDescendants(QTreeWidgetItem* srcNode, QTreeWidgetItem* dstParent) {
	if (!srcNode)
		return nullptr;

	// 如果 srcNode 的子树没有可勾选项，则不要创建该分支
	if (!hasCheckableDescendants(srcNode)) {
		return nullptr;
	}

	// 创建 dst 节点（保留原文本）
	QTreeWidgetItem* dstNode = nullptr;
	if (dstParent) {
		dstNode = new QTreeWidgetItem(dstParent);
	} else {
		dstNode = new QTreeWidgetItem();
	}
	dstNode->setText(0, srcNode->text(0));

	for (int i = 0; i < srcNode->childCount(); ++i) {
		QTreeWidgetItem* srcChild = srcNode->child(i);
		QTreeWidgetItem* dstChild = copyIfHasCheckableDescendants(srcChild, dstNode);
	}

	return dstNode;
}

void ExtractedProgressTreeWidget::addProgressBarForItem(QTreeWidgetItem* dstItem, int total, int checked) {
	if (!dstItem)
		return;
	CCProgressBar* bar = new CCProgressBar(this);
	bar->setRange(0, 100);
	int percent = 0;
	if (total > 0)
		percent = static_cast<int>((checked * 100.0) / total + 0.5);
	bar->setValue(percent);
	bar->setTextVisible(true);
	bar->setFixedWidth(140);
	progress_tree->setItemWidget(dstItem, 1, bar);
}
