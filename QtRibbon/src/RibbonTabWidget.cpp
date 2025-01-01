/*
 * Copyright (C) Martijn Koopman
 * All Rights Reserved
 *
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
 */

#include "RibbonTabWidget.h"
#include "RibbonTab.h"
#include "RibbonGroup.h"

#include <QApplication>
#include <QStyleOption>
#include <QPainter>

RibbonTabWidget::RibbonTabWidget(QWidget *parent)
  : QTabWidget(parent)
{
  // Determine default colors
  QColor bg = qApp->palette().color(QPalette::Background);
  QColor mid = qApp->palette().color(QPalette::Mid);

  // Note: the order in which the background/palette/stylesheet functions are
  // called does matter. Should be same as in Qt designer.
  setAutoFillBackground(true);

  // Set stylesheet
  QString styleSheetText = QString(
    "QTabWidget::pane {"
    "  border-top: 1px solid rgb(%4, %5, %6);"
    "  position: absolute;"
    "  top: -1px;"
    "}"
    ""
    "QTabBar::tab {"
    "  padding-top: 5px;"
    "  padding-bottom: 5px;"
    "  padding-left: 8px;"
    "  padding-right: 8px;"
    "  margin-top: 1px;"
    "}"
    ""
    "QTabBar::tab::!selected {"
    "  border-bottom: 1px solid rgb(%4, %5, %6);"
    "  background-color:#ffffff;"
    "}"
    ""
    "QTabBar::tab:selected {"
    "  background-color: rgb(%1, %2, %3);"
    "  border-top: 1px solid rgb(%4, %5, %6);"
    "  border-right: 1px solid rgb(%4, %5, %6);"
    "  border-left: 1px solid rgb(%4, %5, %6);"
    "  border-bottom: 1px solid rgb(%1, %2, %3);"
    "}"
    ""
    "QTabBar::tab:hover"
    "{"
    "  background-color: rgb(205, 232, 255);"
    "}"
    ""
    "QTabBar::tab:selected:hover {"
    "  background-color: rgb(%1, %2, %3);"
    "}"
    ).arg(bg.red()).arg(bg.green()).arg(bg.blue())
     .arg(mid.red()).arg(mid.green()).arg(mid.blue());

  setStyleSheet(styleSheetText);

  // Set background color
  QPalette pal = palette();
  pal.setColor(QPalette::Background, Qt::white);
  setPalette(pal);
}

RibbonTab* RibbonTabWidget::addTab(const QString& tabName)
{
    RibbonTab* ribbonTab = new RibbonTab;
    QTabWidget::addTab(ribbonTab, tabName);
	return ribbonTab;
}

RibbonTab* RibbonTabWidget::addTab(const QIcon& tabIcon, const QString& tabName)
{
    RibbonTab* ribbonTab = new RibbonTab;
    QTabWidget::addTab(ribbonTab, tabIcon, tabName);
    return ribbonTab;
}

void RibbonTabWidget::addButton(const QString& tabName, const QString& groupName, QToolButton* button)
{
    RibbonTab* tab = this->tab(tabName);
	tab = tab ? tab : addTab(tabName);

	RibbonGroup* group = tab->group(groupName);
	group = group ? group : tab->addGroup(groupName);

	group->addButton(button);
}

void RibbonTabWidget::addWidget(const QString& tabName, const QString& groupName, QWidget* widget)
{
	RibbonTab* tab = this->tab(tabName);
	tab = tab ? tab : addTab(tabName);

	RibbonGroup* group = tab->group(groupName);

	group = group ? group : tab->addGroup(groupName);
	group->addWidget(widget);
}

RibbonTab* RibbonTabWidget::tab(const QString& tabName)
{
	// Find ribbon tab
	QWidget* tab = nullptr;
	for (int i = 0; i < count(); i++)
	{
		if (tabText(i).toLower() == tabName.toLower())
		{
			tab = QTabWidget::widget(i);
			break;
		}
	}
	return static_cast<RibbonTab*>(tab);
}

RibbonGroup* RibbonTabWidget::group(const QString& tabName, const QString& groupName)
{
	RibbonTab* tab = this->tab(tabName);
	return tab ? tab->group(groupName) : nullptr;
}

RibbonTab* RibbonTabWidget::operator[](const QString& tabName)
{
	return this->tab(tabName);
}
