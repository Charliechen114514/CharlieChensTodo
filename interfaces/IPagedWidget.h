#ifndef IPAGEDWIDGET_H
#define IPAGEDWIDGET_H
#include <QMap>
class CCToolBox;
class QStackedWidget;

class IPagedWidget {
public:
	virtual void applyToToolBox(CCToolBox* toolbox,
	                            QStackedWidget* stacked_widget,
	                            QMap<int, int>& index_mappings)
	    = 0;
};

#endif // IPAGEDWIDGET_H
