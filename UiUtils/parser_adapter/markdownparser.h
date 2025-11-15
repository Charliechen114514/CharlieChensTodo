#ifndef MARKDOWNPARSER_H
#define MARKDOWNPARSER_H
#include "UiUtils_Global.h"
#include <QStringList>
class QTreeWidget;
namespace cutils {
class UiUtils_EXPORT PlanParser {
public:
	virtual void parseMarkdown(
	    /* treeView is an in-out params and shell be modified */
	    const QString& markdown_bytes,
	    QTreeWidget* treeView,
	    QStringList& quotes);

	virtual QString fromTreeWidget(QTreeWidget* treeWidget);
};
}

#endif // MARKDOWNPARSER_H
