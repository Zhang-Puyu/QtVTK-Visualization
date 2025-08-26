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

	/*文件**************************************************************/

	tabFile = ribbonTabWidget->addTab(QIcon(":/icons/icons/file.svg"), "文件");
	tabView = ribbonTabWidget->addTab(QIcon(":/icons/icons/measure.svg"), "显示");
	tabSet  = ribbonTabWidget->addTab(QIcon(":/icons/icons/set.svg"),  "设置");
	tabLiveView = ribbonTabWidget->addTab(QIcon(":/icons/icons/info.svg"), "动态显示");

	buttonOpenFile->setText(tr("打开"));
	buttonOpenFile->setIcon(QIcon(":/icons/icons/打开.svg"));

	ribbonTabWidget->addButton("文件", "打开", buttonOpenFile);

	QWidget* widget = new QWidget;
	QGridLayout* gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel("文件编码格式"), 0, 0);
	gridLayout->addWidget(comboCodec, 1, 0);

	comboCodec->addItem("UTF-8");
	comboCodec->addItem("GB18030");
	comboCodec->addItem("UTF-16");
	comboCodec->addItem("UTF-16BE");
	comboCodec->addItem("UTF-16LE");
	comboCodec->addItem("UTF-32");
	comboCodec->addItem("UTF-32BE");
	comboCodec->addItem("UTF-32LE");

	ribbonTabWidget->addWidget("文件", "打开", widget);

	buttonClearView->setText(tr("清空"));
	buttonClearView->setIcon(QIcon(":/icons/icons/清空.svg"));
	ribbonTabWidget->addButton("文件", "", buttonClearView);

	/*显示**************************************************************/

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

	ribbonTabWidget->addWidget("显示", "显示类型", widget);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 4, 3, 4);
	gridLayout->addWidget(new QLabel(tr("X")), 0, 0); gridLayout->addWidget(comboX, 0, 1);
	gridLayout->addWidget(new QLabel(tr("Y")), 1, 0); gridLayout->addWidget(comboY, 1, 1);
	gridLayout->addWidget(new QLabel(tr(" Z")), 0, 3); gridLayout->addWidget(comboZ, 0, 4);
	gridLayout->addWidget(new QLabel(tr(" F")), 1, 3); gridLayout->addWidget(comboF, 1, 4);

	// 仅显示三维点云时
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		comboZ->setEnabled(checked);
		comboF->setEnabled(checked);
		});

	ribbonTabWidget->addWidget("显示", "显示对象", widget);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel(tr("起始")), 0, 0); gridLayout->addWidget(spinFirstVisualPointNo, 0, 1);
	gridLayout->addWidget(new QLabel(tr("截止")), 1, 0); gridLayout->addWidget(spinLastVisualPointNo, 1, 1);

	spinFirstVisualPointNo->setRange(0, 1);
	spinFirstVisualPointNo->setValue(0);
	spinFirstVisualPointNo->setMinimumWidth(60);
	spinLastVisualPointNo->setRange(1, 100);
	spinLastVisualPointNo->setValue(1);
	spinLastVisualPointNo->setMinimumWidth(60);

	auto valueChanged = QOverload<int>::of(&QSpinBox::valueChanged);
	connect(spinFirstVisualPointNo, valueChanged, [=](int value) { spinLastVisualPointNo->setMinimum(++value); });
	connect(spinLastVisualPointNo,  valueChanged, [=](int value) { spinFirstVisualPointNo->setMaximum(--value); });

	ribbonTabWidget->addWidget("显示", "裁剪数据", widget);

	buttonExportVisualPoints->setText(tr("导出"));
	buttonExportVisualPoints->setIcon(QIcon(":/icons/icons/导出.svg"));
	buttonExportVisualPoints->setToolTip(tr("导出当前显示的点到csv文件"));

	ribbonTabWidget->addButton("显示", "裁剪数据", buttonExportVisualPoints);

	buttonRefreshView->setText(tr("刷新"));
	buttonRefreshView->setIcon(QIcon(":/icons/icons/刷新.svg"));
	ribbonTabWidget->addButton("显示", "显示", buttonRefreshView);

	buttonAddSeries->setText(tr("添加"));
	buttonAddSeries->setIcon(QIcon(":/icons/icons/添加.svg"));
	ribbonTabWidget->addButton("显示", "显示", buttonAddSeries);

	// 仅显示二维图像时
	connect(radioChart, &QRadioButton::toggled, [=](bool checked) {
		buttonAddSeries->setEnabled(checked);
		});
	buttonAddSeries->setEnabled(false);

	/*设置**************************************************************/

	buttonBackgroundColor->setText(tr("颜色"));
	buttonBackgroundColor->setIcon(QIcon(":/icons/icons/颜色.svg"));

	QMenu* menuColor = new QMenu("打开");
	actionBackgroundColorUp->setText(tr("上半部分背景色"));
	actionBackgroundColorDown->setText(tr("下半部分背景色"));
	menuColor->addAction(actionBackgroundColorUp);
	menuColor->addAction(actionBackgroundColorDown);

	buttonBackgroundColor->setPopupMode(QToolButton::MenuButtonPopup);
	buttonBackgroundColor->setMenu(menuColor);

	ribbonTabWidget->addButton("设置", "背景", buttonBackgroundColor);

	// 仅显示三维点云时
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		menuColor->setEnabled(checked);
		});

	buttonBackgroundImage->setText(tr("图片"));
	buttonBackgroundImage->setIcon(QIcon(":/icons/icons/图片.svg"));

	ribbonTabWidget->addButton("设置", "背景", buttonBackgroundImage);

	checkAxisVisibility->setText(tr("坐标轴"));
	checkAxisVisibility->setChecked(true);
	checkScalarbarVisibility->setText(tr("标量条"));
	checkScalarbarVisibility->setChecked(true);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 7, 3, 9);
	gridLayout->addWidget(checkAxisVisibility, 0, 0);
	gridLayout->addWidget(checkScalarbarVisibility, 1, 0);

	// 仅显示三维点云时
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		checkAxisVisibility->setEnabled(checked);
		checkScalarbarVisibility->setEnabled(checked);
		});

	ribbonTabWidget->addWidget("设置", "辅助", widget);

	widget		= new QWidget;
	gridLayout  = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel(tr("离散点大小")), 0, 0);
	gridLayout->addWidget(spinPointSize, 0, 1);
	gridLayout->addWidget(new QLabel(tr("拾取点大小")), 1, 0);
	gridLayout->addWidget(spinPickedPointSize, 1, 1);

	spinPointSize->setRange(1, 20);
	spinPointSize->setValue(3);
	spinPickedPointSize->setRange(1, 20);
	spinPickedPointSize->setValue(5);

	ribbonTabWidget->addWidget("设置", "点尺寸", widget);

	// 仅显示三维点云时
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		spinPickedPointSize->setEnabled(checked);
		});

	buttonPickPoint->setText(tr("拾取"));
	buttonPickPoint->setIcon(QIcon(":/icons/icons/跳转.svg"));
	buttonPickPoint->setToolTip(tr("按左右方向键可跳转选中到前后点，按上下方向键可跳转选中到首尾点"));

	QMenu* menuPick = new QMenu("跳转");
	menuPick->addAction(actionPickFirstPoint); menuPick->addAction(actionPickLastPoint);
	menuPick->addAction(actionPickMaxXPoint); menuPick->addAction(actionPickMinXPoint);
	menuPick->addAction(actionPickMaxYPoint); menuPick->addAction(actionPickMinYPoint);
	menuPick->addAction(actionPickMaxZPoint); menuPick->addAction(actionPickMinZPoint);
	menuPick->addAction(actionPickMaxFPoint); menuPick->addAction(actionPickMinFPoint);

	buttonPickPoint->setPopupMode(QToolButton::MenuButtonPopup);
	buttonPickPoint->setMenu(menuPick);

	ribbonTabWidget->addButton("设置", "跳转", buttonPickPoint);

	// 仅显示三维点云时
	connect(radioCloud, &QRadioButton::toggled, [=](bool checked) {
		menuPick->setEnabled(checked);
		});

	/*动态显示**************************************************************/

	buttonStartLiveView->setText(tr("开始"));
	buttonStartLiveView->setIcon(QIcon(":/icons/icons/开始.svg"));

	buttonStopLiveView->setText(tr("停止"));
	buttonStopLiveView->setIcon(QIcon(":/icons/icons/停止.svg"));
	buttonStopLiveView->setEnabled(false);

	ribbonTabWidget->addButton("动态显示", "动态显示", buttonStartLiveView);
	ribbonTabWidget->addButton("动态显示", "动态显示", buttonStopLiveView);

	widget     = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 6, 3, 6);
	gridLayout->addWidget(new QLabel(tr("刷新间隔")), 0, 0);
	gridLayout->addWidget(spinLiveInterval, 0, 1);
	gridLayout->addWidget(new QLabel(tr("持续时间")), 1, 0);
	gridLayout->addWidget(spinLiveDuration, 1, 1);

	spinLiveInterval->setRange(2, 10 * 1000);
	spinLiveInterval->setValue(50);
	spinLiveInterval->setSuffix("ms");
	spinLiveDuration->setRange(1, 10 * 60);
	spinLiveDuration->setValue(10);
	spinLiveDuration->setSuffix("s");

	ribbonTabWidget->addWidget("动态显示", "设置", widget);
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
