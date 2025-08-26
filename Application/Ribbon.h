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

	QRadioButton* radioChart = new QRadioButton("��άͼ��");
	QRadioButton* radioCloud = new QRadioButton("��ά����");

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
	QAction* actionPickFirstPoint = new QAction("��һ����");   QAction* actionPickLastPoint = new QAction("���һ����");
	QAction* actionPickMaxXPoint = new QAction("X����");     QAction* actionPickMinXPoint = new QAction("X��С��");
	QAction* actionPickMaxYPoint = new QAction("Y����");	  QAction* actionPickMinYPoint = new QAction("Y��С��");
	QAction* actionPickMaxZPoint = new QAction("Z����");     QAction* actionPickMinZPoint = new QAction("Z��С��");
	QAction* actionPickMaxFPoint = new QAction("����ֵ����"); QAction* actionPickMinFPoint = new QAction("����ֵ��С��");

	QToolButton* buttonStartLiveView = new QToolButton;
	QToolButton* buttonStopLiveView  = new QToolButton;

	QCheckBox* checkScalarbarVisibility = new QCheckBox;
	QCheckBox* checkAxisVisibility		= new QCheckBox;

	QComboBox* comboCodec = new QComboBox;

	/// @brief ������ʾ�ĵ�ı��
	void resetSpinVisualPointNo();
};
