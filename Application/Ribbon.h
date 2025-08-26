#pragma once

#include "qwidget.h"
#include "qtoolbutton.h"
#include "qaction.h"
#include "qspinbox.h"
#include "qcombobox.h"
#include "qcheckbox.h"
#include "qradiobutton.h"
#include "qbuttongroup.h"

#include "RibbonTab.h"

#pragma execution_character_set("utf-8")

class Ribbon : public QWidget
{
	Q_OBJECT

public:
	Ribbon(QWidget* parent = nullptr);

	RibbonTab* tabFile; 
	RibbonTab* tabView; 
	RibbonTab* tabSet; 
	RibbonTab* tabLiveView;

	QToolButton* buttonOpenFile	= new QToolButton;

	QRadioButton* radioChart = new QRadioButton("二维图像");
	QRadioButton* radioCloud = new QRadioButton("三维点云");

	QToolButton* buttonClearView   = new QToolButton;
	QToolButton* buttonRefreshView = new QToolButton;

	QToolButton* buttonAddSeries = new QToolButton;

	QToolButton* buttonBackgroundColor = new QToolButton;
	QAction* actionBackgroundColorUp   = new QAction;
	QAction* actionBackgroundColorDown = new QAction;

	QToolButton* buttonBackgroundImage = new QToolButton;

	QSpinBox* spinPointSize		  = new QSpinBox;
	QSpinBox* spinPickedPointSize = new QSpinBox;

	QSpinBox* spinFirstVisualPointNo = new QSpinBox;
	QSpinBox* spinLastVisualPointNo  = new QSpinBox;

	QToolButton* buttonExportVisualPoints = new QToolButton;

	QComboBox* comboX = new QComboBox;
	QComboBox* comboY = new QComboBox;
	QComboBox* comboZ = new QComboBox;
	QComboBox* comboF = new QComboBox;

	QSpinBox* spinLiveInterval = new QSpinBox;
	QSpinBox* spinLiveDuration = new QSpinBox;

	QToolButton* buttonPickPoint = new QToolButton;
	QAction* actionPickFirstPoint = new QAction("第一个点");   QAction* actionPickLastPoint = new QAction("最后一个点");
	QAction* actionPickMaxXPoint = new QAction("X最大点");     QAction* actionPickMinXPoint = new QAction("X最小点");
	QAction* actionPickMaxYPoint = new QAction("Y最大点");	  QAction* actionPickMinYPoint = new QAction("Y最小点");
	QAction* actionPickMaxZPoint = new QAction("Z最大点");     QAction* actionPickMinZPoint = new QAction("Z最小点");
	QAction* actionPickMaxFPoint = new QAction("特征值最大点"); QAction* actionPickMinFPoint = new QAction("特征值最小点");

	QToolButton* buttonStartLiveView = new QToolButton;
	QToolButton* buttonStopLiveView  = new QToolButton;

	QCheckBox* checkScalarbarVisibility = new QCheckBox;
	QCheckBox* checkAxisVisibility		= new QCheckBox;

	QComboBox* comboCodec = new QComboBox;

	/// @brief 重置显示的点的编号
	void resetSpinVisualPointNo();
};
