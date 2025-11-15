#include "markdownparser.h"
#include "markdown_parser_privates.h"
#include "parser/markdown_parser.h"
#include <QTreeWidget>

namespace {
using IBaseNode = plan_parser::base_components::IBaseNode;
inline std::vector<std::string> fromQStringLists(const QStringList& texts) {
	std::vector<std::string> result {};
	for (const auto& each : texts) {
		result.emplace_back(each.toStdString());
	}
	return result;
}

void filledTreeView(QTreeWidget* treeWidget,
                    const std::vector<std::shared_ptr<IBaseNode>>& nodes,
                    QStringList& quotes) {
	for (const auto& n : nodes) {
		QTreeWidgetItem* it = cutils::privates::nodeToTreeItem(treeWidget, n, quotes);
		if (it)
			treeWidget->addTopLevelItem(it);
	}
}

}

namespace cutils {
void PlanParser::parseMarkdown(
    const QString& markdown_bytes,
    QTreeWidget* treeView,
    QStringList& quotes) {
	auto adapts = fromQStringLists(markdown_bytes.split('\n'));

	// Ok parse as treeView sessions
	using namespace plan_parser::parser;
	MarkdownParser parser;
	auto works = parser.parse(adapts);
	treeView->clear();
	filledTreeView(treeView, works, quotes);
}

QString PlanParser::fromTreeWidget(QTreeWidget* treeWidget) {
	QString md;
	for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
		QTreeWidgetItem* top = treeWidget->topLevelItem(i);
		md += privates::serializeItemRecursive(top, 0);
	}

	while (md.endsWith("\n\n"))
		md.chop(1); // leave a single trailing newline maybe
	return md;
}
}
