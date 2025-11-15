#ifndef MARKDOWN_PARSER_PRIVATES_H
#define MARKDOWN_PARSER_PRIVATES_H
#include "base_components/base_node.h"
#include <QStringList>
#include <memory>
class QTreeWidgetItem;
class QTreeWidget;

namespace cutils {
namespace privates { // privates should not be used externly
	using IBaseNode = plan_parser::base_components::IBaseNode;

	QTreeWidgetItem* nodeToTreeItem(QTreeWidget* widget,
	                                const std::shared_ptr<IBaseNode>& node,
	                                QStringList& quotes, bool request_hide = false);

	QString serializeItemRecursive(QTreeWidgetItem* item, int indent = 0);
}
}

#endif // MARKDOWN_PARSER_PRIVATES_H
