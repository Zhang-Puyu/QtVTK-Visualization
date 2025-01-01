
#ifndef RIBBONTABCONTENT_H
#define RIBBONTABCONTENT_H

#include "qwidget.h"
#include "qtoolbutton.h"
#include "qgridlayout.h"
#include "qlayout.h"
#include "qframe.h"
#include "qlabel.h"	
#include "qscrollarea.h"

#include "RibbonGroup.h"


class RibbonTab : public QWidget
{
	Q_OBJECT

public:
	RibbonTab(QWidget* parent = nullptr);

	RibbonGroup* addGroup(const QString& groupName);
	void		 addGroup(RibbonGroup* ribbonGroup);

	void addButton(const QString& groupName, QToolButton* button);
	void addWidget(const QString& groupName, QWidget* widget);

	RibbonGroup* group(const QString& groupName);

	RibbonGroup* operator[](const QString& groupName);

private:
	QGridLayout* gridLayout;

	QScrollArea* scrollArea;
	QWidget*	 scrollAreaContent;
	QGridLayout* scrollAreaGridLayout;

	QWidget*	 spacer;
	QHBoxLayout* hLayout;
};

#endif // RIBBONTABCONTENT_H
