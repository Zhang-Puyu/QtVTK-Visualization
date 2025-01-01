
#ifndef RIBBONBUTTONGROUP_H
#define RIBBONBUTTONGROUP_H

#include "qwidget.h"
#include "qtoolbutton.h"
#include "qgridlayout.h"
#include "qlayout.h"
#include "qframe.h"
#include "qlabel.h"	

class RibbonGroup : public QWidget
{
	Q_OBJECT

public:
	RibbonGroup(const QString& title = "", QWidget* parent = nullptr);

	void	setTitle(const QString& title);
	QString title();

	void addButton(QToolButton* button);
	void addWidget(QWidget*     widget);

private:
	QGridLayout* gridLayout;

	QVBoxLayout* vLayout;
	QHBoxLayout* hLayout;

	QFrame* hLine;
	QFrame* vLine;

	QLabel* label;
};

#endif
