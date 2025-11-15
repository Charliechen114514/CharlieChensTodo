#include "markdown_parser_privates.h"
#include "base_components/base_node.h"
#include "base_components/heading.h"
#include "base_components/list.h"
#include "base_components/list_item.h"
#include "base_components/paragraph.h"
#include "base_components/quote.h"
#include <QDebug>
#include <QTreeWidget>

namespace cutils {
namespace privates {
	QString serializeItemRecursive(QTreeWidgetItem* item, int indent);
}
}

namespace {

bool isOnlyHyphensAndThreeOrMore(const QString& text) {
	QRegularExpression regex("^-{3,}$");
	return regex.match(text).hasMatch();
}

int getTypeRole(QTreeWidgetItem* item) {
	QVariant v = item->data(0, Qt::UserRole + 1);
	if (v.isValid() && v.canConvert<int>())
		return v.toInt();
	return 0;
}

bool ifOrdered(QTreeWidgetItem* item) {
	QVariant v = item->data(0, Qt::UserRole + 3);
	if (v.isValid() && v.canConvert<bool>()) {
		return v.toBool();
	}
	return false;
}

int get_level(QTreeWidgetItem* item) {
	QVariant v = item->data(0, Qt::UserRole + 2);
	if (v.isValid() && v.canConvert<int>()) {
		return v.toInt();
	}
	return 0;
}

QString serializeListNode(QTreeWidgetItem* listNode, int indent) {
	QString out;
	for (int i = 0; i < listNode->childCount(); ++i) {
		QTreeWidgetItem* li = listNode->child(i);
		out += cutils::privates::serializeItemRecursive(li, indent);
	}
	return out;
}

QTreeWidgetItem* findAncestorList(QTreeWidgetItem* item) {
	QTreeWidgetItem* p = item->parent();
	while (p) {
		if (getTypeRole(p) == static_cast<int>(plan_parser::base_components::BaseNodeType::LIST))
			return p;
		p = p->parent();
	}
	return nullptr;
}

QString serializeListItemNode(QTreeWidgetItem* item, int indent) {
	using plan_parser::base_components::BaseNodeType;
	QString text = item->text(0);
	QTreeWidgetItem* listParent = findAncestorList(item);
	bool ordered = false;
	int index = 0;
	if (listParent) {
		ordered = listParent->data(0, Qt::UserRole + 3).toBool();
		index = listParent->indexOfChild(item) + 1; // 1-based index
	}

	QString indentStr = indent ? QString(indent * 2, ' ') : QString();

	QString prefix;
	bool isTaskItem = (item->flags() & Qt::ItemIsUserCheckable) && !ordered;
	if (isTaskItem) {
		Qt::CheckState checkState = item->checkState(0);
		if (checkState == Qt::Checked) {
			prefix = indentStr + "- [x] ";
		} else {
			prefix = indentStr + "- [ ] ";
		}
	} else if (ordered) {
		prefix = indentStr + QString::number(index) + ". ";
	} else {
		prefix = indentStr + "- ";
	}
	QString outLine = prefix + text + "\n\n";

	QString childrenOut;
	for (int ci = 0; ci < item->childCount(); ++ci) {
		QTreeWidgetItem* ch = item->child(ci);
		int chType = getTypeRole(ch);

		if (chType == static_cast<int>(BaseNodeType::LIST)) {
			childrenOut += serializeListNode(ch, indent + 1);
		} else {
			childrenOut += cutils::privates::serializeItemRecursive(ch, indent + 1);
		}
	}

	return outLine + childrenOut;
}
}

namespace cutils {
namespace privates {

	QString serializeItemRecursive(QTreeWidgetItem* item, int indent) {
		using plan_parser::base_components::BaseNodeType;
		QString out;
		int nodeType = getTypeRole(item);

		switch (nodeType) {
		case static_cast<int>(BaseNodeType::HEADING): {
			QString hashes(get_level(item), '#');
			out += hashes + " " + item->text(0) + "\n\n";
			for (int i = 0; i < item->childCount(); ++i) {
				out += cutils::privates::serializeItemRecursive(item->child(i), 0);
			}
			return out;
		}

		case static_cast<int>(BaseNodeType::PARAGRAPH): {
			out += QString(indent ? QString(indent * 2, ' ') : "") + item->text(0) + "\n\n";
			for (int i = 0; i < item->childCount(); ++i) {
				out += cutils::privates::serializeItemRecursive(item->child(i), 0);
			}
			return out;
		}

		case static_cast<int>(BaseNodeType::QUOTE): {
			QString block;
			for (int i = 0; i < item->childCount(); ++i) {
				QTreeWidgetItem* ch = item->child(i);
				block += serializeItemRecursive(ch, 0); // inside quote treat children as top-level for content
			}

			// prefix lines
			QStringList lines = block.split('\n');
			QString qout;
			for (int i = 0; i < lines.size(); ++i) {
				QString ln = lines.at(i);
				if (ln.isEmpty())
					qout += ">\n";
				else
					qout += "> " + ln + "\n";
			}
			qout += "\n";
			return qout;
		}

		case static_cast<int>(BaseNodeType::LIST): {
			return serializeListNode(item, indent);
		}

		case static_cast<int>(BaseNodeType::LISTITEM): {
			return serializeListItemNode(item, indent);
		}

		default:
			out += QString(indent ? QString(indent * 2, ' ') : "") + item->text(0) + "\n";
			for (int i = 0; i < item->childCount(); ++i) {
				out += cutils::privates::serializeItemRecursive(item->child(i), indent);
			}
			return out;
		}
	}

	inline QString from_std_string(const std::string& s) {
		return QString::fromStdString(s);
	}

	static QTreeWidgetItem* last_headings = nullptr;
	// UserRole + 1: type
	// +2: Heading Level
	// +3: if ordered
	QTreeWidgetItem* nodeToTreeItem(
	    QTreeWidget* widget,
	    const std::shared_ptr<IBaseNode>& node,
	    QStringList& quotes, bool request_hide) {
		using namespace plan_parser::base_components;
		switch (node->get_node_type()) {
		case plan_parser::base_components::BaseNodeType::LIST: {
			auto lst = std::dynamic_pointer_cast<List>(node);
			QTreeWidgetItem* parent_item = nullptr;
			if (!last_headings) {
				QTreeWidgetItem* item = new QTreeWidgetItem(widget);
				item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
				item->setData(0, Qt::UserRole + 1, (int)node->get_node_type());
				item->setData(0, Qt::UserRole + 3, lst->ordered.get());
				item->setText(0, "Working Sheets");
				parent_item = item;
				item->setHidden(request_hide);
			} else {
				parent_item = last_headings;
			}

			for (const auto& it : lst->items) {
				// create for each
				auto li = std::dynamic_pointer_cast<ListItem>(it);

				QTreeWidgetItem* listItem = new QTreeWidgetItem(parent_item);
				listItem->setFlags(listItem->flags() & ~Qt::ItemIsUserCheckable);
				listItem->setData(0, Qt::UserRole + 1, (int)it->get_node_type());
				listItem->setText(0, QString::fromStdString(li->text.get()));

				if (li->task) {
					listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);
					listItem->setCheckState(0, li->done ? Qt::Checked : Qt::Unchecked);
				}

				// process sub childs
				last_headings = listItem;
				for (const auto& child : li->children) {
					QTreeWidgetItem* childItem = nodeToTreeItem(nullptr, child, quotes, request_hide);
					if (!childItem)
						continue;

					if (childItem == listItem || childItem->parent() == listItem) {
						qDebug() << "  skip adding: already child or equal self";
					} else {
						listItem->addChild(childItem);
					}
				}
				listItem->setHidden(request_hide);
				last_headings = parent_item;
			}
			return parent_item;
		} break;
		case plan_parser::base_components::BaseNodeType::LISTITEM: {
			// dont handle, these is a quote
			qWarning() << "Meeting a lonely item, dont handle";
		} break;
		case plan_parser::base_components::BaseNodeType::HEADING: {
			QTreeWidgetItem* item = new QTreeWidgetItem(widget);
			item->setHidden(request_hide);
			auto h = std::dynamic_pointer_cast<Heading>(node);
			item->setData(0, Qt::UserRole + 1, (int)node->get_node_type());

			item->setData(0, Qt::UserRole + 2, h->level.get());
			item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);

			last_headings = item;
			if (!h) {
				qWarning() << "Find unexpected transform of Heading";
				break;
			}
			item->setText(0, QString::fromStdString(h->text.get()));
			// qDebug() << h->level.get() << " : " << item->text(0);
			QFont f = item->font(0);
			f.setBold(true);
			int baseSize = 10 + std::max(0, 6 - (int)h->level.get());
			f.setPointSize(baseSize);
			item->setFont(0, f);
			return item;
		} break;
		case plan_parser::base_components::BaseNodeType::PARAGRAPH: {
			QTreeWidgetItem* item = new QTreeWidgetItem(widget);
			item->setHidden(request_hide);
			auto p = std::dynamic_pointer_cast<Paragraph>(node);
			const auto str = from_std_string(p->text.get());
			if (isOnlyHyphensAndThreeOrMore(str)) {
				item->setHidden(true);
			}
			item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
			item->setData(0, Qt::UserRole + 1, (int)node->get_node_type());
			item->setText(0, str);
			return item;
		} break;
		case plan_parser::base_components::BaseNodeType::QUOTE: {
			auto q = std::dynamic_pointer_cast<Quote>(node);
			QTreeWidgetItem* item = new QTreeWidgetItem(widget);
			item->setData(0, Qt::UserRole + 1, (int)node->get_node_type());
			QTreeWidgetItem* old_parent = last_headings;
			last_headings = item;
			for (const auto& ch : q->children) {
				QTreeWidgetItem* child = nodeToTreeItem(nullptr, ch, quotes, true);
				if (child)
					item->addChild(child);
			}
			last_headings = old_parent;
			item->setHidden(true);
			return item;
		} break;
		}
		return nullptr;
	}
}
}
