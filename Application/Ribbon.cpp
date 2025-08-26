#include "Ribbon.h"

#include "RibbonTab.h"
#include "RibbonTabWidget.h"

#include "qmenu.h"
#include "qlabel.h"
#include "qdebug.h"

#pragma execution_character_set("utf-8")

Ribbon::Ribbon(QWidget* parent)
	: QWidget(parent)
{
	RibbonTabWidget* ribbonTabWidget = new RibbonTabWidget(this);
	QVBoxLayout*     layout			 = new QVBoxLayout(this);
	layout->addWidget(ribbonTabWidget);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);

	setMinimumHeight(115);
	setMaximumHeight(115);

	/*�ļ�**************************************************************/

	tabFile = ribbonTabWidget->addTab(QIcon(":/icons/icons/file.svg"), "�ļ�");
	tabView = ribbonTabWidget->addTab(QIcon(":/icons/icons/measure.svg"), "��ʾ");
	tabSet  = ribbonTabWidget->addTab(QIcon(":/icons/icons/set.svg"),  "����");
	tabLiveView = ribbonTabWidget->addTab(QIcon(":/icons/icons/info.svg"), "��̬��ʾ");

	buttonOpenFile->setText(tr("��"));
	buttonOpenFile->setIcon(QIcon(":/icons/icons/��.svg"));

	ribbonTabWidget->addButton("�ļ�", "��", buttonOpenFile);

	QWidget* widget = new QWidget;
	QGridLayout* gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel("�ļ������ʽ"), 0, 0);
	gridLayout->addWidget(comboCodec, 1, 0);

	comboCodec->addItem("UTF-8");
	comboCodec->addItem("GB18030");
	comboCodec->addItem("UTF-16");
	comboCodec->addItem("UTF-16BE");
	comboCodec->addItem("UTF-16LE");
	comboCodec->addItem("UTF-32");
	comboCodec->addItem("UTF-32BE");
	comboCodec->addItem("UTF-32LE");

	ribbonTabWidget->addWidget("�ļ�", "��", widget);

	buttonClearView->setText(tr("���"));
	buttonClearView->setIcon(QIcon(":/icons/icons/���.svg"));
	ribbonTabWidget->addButton("�ļ�", "", buttonClearView);

	/*��ʾ**************************************************************/

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 7, 3, 9);
	QButtonGroup* radioGroup = new QButtonGroup();
	radioGroup->addButton(radioChart, 0);
	radioGroup->addButton(radioCloud, 1);
	radioGroup->setExclusive(true);

	gridLayout->addWidget(radioChart, 0, 0);
	gridLayout->addWidget(radioCloud, 1, 0);

	radioCloud->setChecked(true);

	ribbonTabWidget->addWidget("��ʾ", "��ʾ����", widget);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 4, 3, 4);
	gridLayout->addWidget(new QLabel(tr("X")), 0, 0); gridLayout->addWidget(comboX, 0, 1);
	gridLayout->addWidget(new QLabel(tr("Y")), 1, 0); gridLayout->addWidget(comboY, 1, 1);
	gridLayout->addWidget(new QLabel(tr(" Z")), 0, 3); gridLayout->addWidget(comboZ, 0, 4);
	gridLayout->addWidget(new QLabel(tr(" F")), 1, 3); gridLayout->addWidget(comboF, 1, 4);

	// ����ʾ��ά����ʱ
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		comboZ->setEnabled(checked);
		comboF->setEnabled(checked);
		});

	ribbonTabWidget->addWidget("��ʾ", "��ʾ����", widget);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel(tr("��ʼ")), 0, 0); gridLayout->addWidget(spinFirstVisualPointNo, 0, 1);
	gridLayout->addWidget(new QLabel(tr("��ֹ")), 1, 0); gridLayout->addWidget(spinLastVisualPointNo, 1, 1);

	spinFirstVisualPointNo->setRange(0, 1);
	spinFirstVisualPointNo->setValue(0);
	spinFirstVisualPointNo->setMinimumWidth(60);
	spinLastVisualPointNo->setRange(1, 100);
	spinLastVisualPointNo->setValue(1);
	spinLastVisualPointNo->setMinimumWidth(60);

	auto valueChanged = QOverload<int>::of(&QSpinBox::valueChanged);
	connect(spinFirstVisualPointNo, valueChanged, [=](int value) { spinLastVisualPointNo->setMinimum(++value); });
	connect(spinLastVisualPointNo,  valueChanged, [=](int value) { spinFirstVisualPointNo->setMaximum(--value); });

	ribbonTabWidget->addWidget("��ʾ", "�ü�����", widget);

	buttonExportVisualPoints->setText(tr("����"));
	buttonExportVisualPoints->setIcon(QIcon(":/icons/icons/����.svg"));
	buttonExportVisualPoints->setToolTip(tr("������ǰ��ʾ�ĵ㵽csv�ļ�"));

	ribbonTabWidget->addButton("��ʾ", "�ü�����", buttonExportVisualPoints);

	buttonRefreshView->setText(tr("ˢ��"));
	buttonRefreshView->setIcon(QIcon(":/icons/icons/ˢ��.svg"));
	ribbonTabWidget->addButton("��ʾ", "��ʾ", buttonRefreshView);

	buttonAddSeries->setText(tr("���"));
	buttonAddSeries->setIcon(QIcon(":/icons/icons/���.svg"));
	ribbonTabWidget->addButton("��ʾ", "��ʾ", buttonAddSeries);

	// ����ʾ��άͼ��ʱ
	connect(radioChart, &QRadioButton::toggled, [=](bool checked) {
		buttonAddSeries->setEnabled(checked);
		});
	buttonAddSeries->setEnabled(false);

	/*����**************************************************************/

	buttonBackgroundColor->setText(tr("��ɫ"));
	buttonBackgroundColor->setIcon(QIcon(":/icons/icons/��ɫ.svg"));

	QMenu* menuColor = new QMenu("��");
	actionBackgroundColorUp->setText(tr("�ϰ벿�ֱ���ɫ"));
	actionBackgroundColorDown->setText(tr("�°벿�ֱ���ɫ"));
	menuColor->addAction(actionBackgroundColorUp);
	menuColor->addAction(actionBackgroundColorDown);

	buttonBackgroundColor->setPopupMode(QToolButton::MenuButtonPopup);
	buttonBackgroundColor->setMenu(menuColor);

	ribbonTabWidget->addButton("����", "����", buttonBackgroundColor);

	// ����ʾ��ά����ʱ
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		menuColor->setEnabled(checked);
		});

	buttonBackgroundImage->setText(tr("ͼƬ"));
	buttonBackgroundImage->setIcon(QIcon(":/icons/icons/ͼƬ.svg"));

	ribbonTabWidget->addButton("����", "����", buttonBackgroundImage);

	checkAxisVisibility->setText(tr("������"));
	checkAxisVisibility->setChecked(true);
	checkScalarbarVisibility->setText(tr("������"));
	checkScalarbarVisibility->setChecked(true);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 7, 3, 9);
	gridLayout->addWidget(checkAxisVisibility, 0, 0);
	gridLayout->addWidget(checkScalarbarVisibility, 1, 0);

	// ����ʾ��ά����ʱ
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		checkAxisVisibility->setEnabled(checked);
		checkScalarbarVisibility->setEnabled(checked);
		});

	ribbonTabWidget->addWidget("����", "����", widget);

	widget		= new QWidget;
	gridLayout  = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel(tr("��ɢ���С")), 0, 0);
	gridLayout->addWidget(spinPointSize, 0, 1);
	gridLayout->addWidget(new QLabel(tr("ʰȡ���С")), 1, 0);
	gridLayout->addWidget(spinPickedPointSize, 1, 1);

	spinPointSize->setRange(1, 20);
	spinPointSize->setValue(3);
	spinPickedPointSize->setRange(1, 20);
	spinPickedPointSize->setValue(5);

	ribbonTabWidget->addWidget("����", "��ߴ�", widget);

	// ����ʾ��ά����ʱ
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		spinPickedPointSize->setEnabled(checked);
		});

	buttonPickPoint->setText(tr("ʰȡ"));
	buttonPickPoint->setIcon(QIcon(":/icons/icons/��ת.svg"));
	buttonPickPoint->setToolTip(tr("�����ҷ��������תѡ�е�ǰ��㣬�����·��������תѡ�е���β��"));

	QMenu* menuPick = new QMenu("��ת");
	menuPick->addAction(actionPickFirstPoint); menuPick->addAction(actionPickLastPoint);
	menuPick->addAction(actionPickMaxXPoint); menuPick->addAction(actionPickMinXPoint);
	menuPick->addAction(actionPickMaxYPoint); menuPick->addAction(actionPickMinYPoint);
	menuPick->addAction(actionPickMaxZPoint); menuPick->addAction(actionPickMinZPoint);
	menuPick->addAction(actionPickMaxFPoint); menuPick->addAction(actionPickMinFPoint);

	buttonPickPoint->setPopupMode(QToolButton::MenuButtonPopup);
	buttonPickPoint->setMenu(menuPick);

	ribbonTabWidget->addButton("����", "��ת", buttonPickPoint);

	// ����ʾ��ά����ʱ
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		menuPick->setEnabled(checked);
		});

	/*��̬��ʾ**************************************************************/

	buttonStartLiveView->setText(tr("��ʼ"));
	buttonStartLiveView->setIcon(QIcon(":/icons/icons/��ʼ.svg"));

	buttonStopLiveView->setText(tr("ֹͣ"));
	buttonStopLiveView->setIcon(QIcon(":/icons/icons/ֹͣ.svg"));
	buttonStopLiveView->setEnabled(false);

	ribbonTabWidget->addButton("��̬��ʾ", "��̬��ʾ", buttonStartLiveView);
	ribbonTabWidget->addButton("��̬��ʾ", "��̬��ʾ", buttonStopLiveView);

	widget     = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel(tr("ˢ�¼��")), 0, 0);
	gridLayout->addWidget(spinLiveInterval, 0, 1);
	gridLayout->addWidget(new QLabel(tr("����ʱ��")), 1, 0);
	gridLayout->addWidget(spinLiveDuration, 1, 1);

	spinLiveInterval->setRange(2, 10 * 1000);
	spinLiveInterval->setValue(50);
	spinLiveInterval->setSuffix("ms");
	spinLiveDuration->setRange(1, 10 * 60);
	spinLiveDuration->setValue(10);
	spinLiveDuration->setSuffix("s");

	ribbonTabWidget->addWidget("��̬��ʾ", "����", widget);
}

void Ribbon::resetSpinVisualPointNo()
{
	spinFirstVisualPointNo->setMinimum(0);
	spinFirstVisualPointNo->setValue(0);
	spinFirstVisualPointNo->setMaximum(0);

	spinLastVisualPointNo->setMinimum(1);
	spinLastVisualPointNo->setValue(1);
	spinLastVisualPointNo->setMaximum(1);
}
