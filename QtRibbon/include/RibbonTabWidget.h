
#ifndef RIBBONTABWIDGET_H
#define RIBBONTABWIDGET_H

#include <QTabWidget>
#include <QToolButton>

#include "RibbonTab.h"

class RibbonTabWidget : public QTabWidget
{
	Q_OBJECT
public:
	explicit RibbonTabWidget(QWidget* parent = 0);

	RibbonTab* addTab(const QString& tabName);
	RibbonTab* addTab(const QIcon& tabIcon, const QString& tabName);

	void addButton(const QString& tabName, const QString& groupName, QToolButton* button);
	void addWidget(const QString& tabName, const QString& groupName, QWidget* widget);
	
	RibbonTab*   tab(const QString& tabName);
	RibbonGroup* group(const QString& tabName, const QString& groupName);

	RibbonTab* operator[](const QString& tabName);
};

#endif
