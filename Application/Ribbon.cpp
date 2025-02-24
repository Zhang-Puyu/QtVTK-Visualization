#include "Ribbon.h"

#include "RibbonTab.h"
#include "RibbonTabWidget.h"
#include "RibbonGroup.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QLabel>

#pragma execution_character_set("utf-8")

Ribbon::Ribbon(QWidget* parent)
	: QWidget(parent)
{
	RibbonTabWidget* ribbonTabWidget = new RibbonTabWidget(this);
	QVBoxLayout*     layout			 = new QVBoxLayout(this);
	layout->addWidget(ribbonTabWidget);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);

	/***************************************************************/

	ribbonTabWidget->addTab(QIcon(":/icons/icons/file.svg"), "显示");
	ribbonTabWidget->addTab(QIcon(":/icons/icons/set.svg"),  "设置");
	ribbonTabWidget->addTab(QIcon(":/icons/icons/info.svg"), "动态显示");

	buttonOpenFile->setText(tr("打开"));
	buttonOpenFile->setIcon(QIcon(":/icons/icons/打开.svg"));

	QMenu* menuOpen = new QMenu("打开");
	actionOpenCsv_AndSkipFirstRow->setText(tr("打开csv，第一行为表头"));
	actionOpenCsv_NotSkipFirstRow->setText(tr("打开csv，第一行为数据"));
	menuOpen->addAction(actionOpenCsv_AndSkipFirstRow);
	menuOpen->addAction(actionOpenCsv_NotSkipFirstRow);

	buttonOpenFile->setPopupMode(QToolButton::MenuButtonPopup);
	buttonOpenFile->setMenu(menuOpen);

	ribbonTabWidget->addButton("显示", "文件", buttonOpenFile);

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

	ribbonTabWidget->addWidget("显示", "文件", widget);

	widget = new QWidget;
	gridLayout = new QGridLayout(widget);
	gridLayout->setContentsMargins(3, 4, 3, 4);
	gridLayout->addWidget(new QLabel(tr("X")), 0, 0); gridLayout->addWidget(comboX, 0, 1);
	gridLayout->addWidget(new QLabel(tr("Y")), 1, 0); gridLayout->addWidget(comboY, 1, 1);
	gridLayout->addWidget(new QLabel(tr(" Z")), 0, 3); gridLayout->addWidget(comboZ, 0, 4);
	gridLayout->addWidget(new QLabel(tr(" F")), 1, 3); gridLayout->addWidget(comboF, 1, 4);

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

	buttonRefreshView->setText(tr("刷新"));
	buttonRefreshView->setIcon(QIcon(":/icons/icons/刷新.svg"));
	ribbonTabWidget->addButton("显示", "显示", buttonRefreshView);

	buttonClearView->setText(tr("清空"));
	buttonClearView->setIcon(QIcon(":/icons/icons/清空.svg"));
	ribbonTabWidget->addButton("显示", "显示", buttonClearView);

	/***************************************************************/

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

	ribbonTabWidget->addWidget("设置", "显示", widget);

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

	/***************************************************************/

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
	spinLiveDuration->setEnabled(false);

	ribbonTabWidget->addWidget("动态显示", "设置", widget);
}

void Ribbon::resetSpinVisualPointNo()
{
	spinFirstVisualPointNo->setMaximum(0);
	spinFirstVisualPointNo->setValue(0);
	spinFirstVisualPointNo->setMaximum(0);

	spinLastVisualPointNo->setMinimum(0);
	spinLastVisualPointNo->setValue(0);
	spinLastVisualPointNo->setMinimum(0);
}
