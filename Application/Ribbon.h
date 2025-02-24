#pragma once

#include "qwidget.h"
#include "qtoolbutton.h"
#include "qaction.h"
#include "qspinbox.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qcheckbox.h"

#pragma execution_character_set("utf-8")

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

	QToolButton* buttonPickPoint = new QToolButton;
	QAction* actionPickFirstPoint = new QAction(tr("��һ����")); QAction* actionPickLastPoint = new QAction("���һ����");
	QAction* actionPickMaxXPoint = new QAction("X����"); QAction* actionPickMinXPoint = new QAction("X��С��");
	QAction* actionPickMaxYPoint = new QAction("Y����"); QAction* actionPickMinYPoint = new QAction("Y��С��");
	QAction* actionPickMaxZPoint = new QAction("Z����"); QAction* actionPickMinZPoint = new QAction("Z��С��");
	QAction* actionPickMaxFPoint = new QAction("����ֵ����"); QAction* actionPickMinFPoint = new QAction("����ֵ��С��");

	QToolButton* buttonStartLiveView = new QToolButton;
	QToolButton* buttonStopLiveView  = new QToolButton;

	QCheckBox* checkScalarbarVisibility = new QCheckBox;
	QCheckBox* checkAxisVisibility		= new QCheckBox;

	QToolButton* buttonBackgroundImage = new QToolButton;

	QComboBox* comboCodec = new QComboBox;

	void resetSpinVisualPointNo();
};
