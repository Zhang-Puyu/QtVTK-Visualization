#pragma once

#include "qwidget.h"
#include "qtoolbutton.h"
#include "qaction.h"
#include "qspinbox.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qcheckbox.h"

class Ribbon : public QWidget
{
	Q_OBJECT

public:
	Ribbon(QWidget* parent = nullptr);

	QToolButton* buttonOpenFile				 = new QToolButton;
	QAction*     actionOpenCsv_AndSkipFirstRow	 = new QAction;
	QAction*     actionOpenCsv_NotSkipFirstRow   = new QAction;

	QToolButton* buttonClearView   = new QToolButton;
	QToolButton* buttonRefreshView = new QToolButton;

	QToolButton* buttonBackgroundColor = new QToolButton;
	QAction* actionBackgroundColorUp   = new QAction;
	QAction* actionBackgroundColorDown = new QAction;

	QSpinBox* spinPointSize		  = new QSpinBox;
	QSpinBox* spinPickedPointSize = new QSpinBox;

	QSpinBox* spinFirstVisualPointNo = new QSpinBox;
	QSpinBox* spinLastVisualPointNo  = new QSpinBox;

	QComboBox* comboX = new QComboBox;
	QComboBox* comboY = new QComboBox;
	QComboBox* comboZ = new QComboBox;
	QComboBox* comboF = new QComboBox;

	QSpinBox* spinLiveInterval = new QSpinBox;
	QSpinBox* spinLiveDuration = new QSpinBox;

	QToolButton* buttonStartLiveView = new QToolButton;
	QToolButton* buttonStopLiveView  = new QToolButton;

	QCheckBox* checkScalarbarVisibility = new QCheckBox;
	QCheckBox* checkAxisVisibility		= new QCheckBox;

	QToolButton* buttonBackgroundImage = new QToolButton;
};
