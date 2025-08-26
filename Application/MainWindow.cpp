#include "MainWindow.h"

#include "qfiledialog.h"
#include "qevent.h"
#include "qmimedata.h"
#include "qcolordialog.h"
#include "qlabel.h"
#include "qmessagebox.h"

#include "EigenExtention.hpp"
#include "StringExtension.hpp"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
	ri = ui->ribbon;
	// 允许文件拖入
	this->setAcceptDrops(true);
	// 允许文件拖入
	this->setAcceptDrops(true);
	// 打开文件
	connect(ri->buttonOpenFile, &QToolButton::clicked, this, &MainWindow::open);
	// 清空视图
	connect(ri->buttonClearView, &QToolButton::clicked, this, &MainWindow::clear);


	// 设置背景颜色
	connect(ri->buttonBackgroundColor, &QToolButton::clicked, [=]() {
		QColor color = QColorDialog::getColor();
		if (color.isValid()) {
			if (isVisualChart()) ui->visualChartWidget->setBackgroundColor(color);
			if (isVisualCloud())
			{
				ui->visualCloudWidget->setBackgroundColorUp(color);
				ui->visualCloudWidget->setBackgroundColorDown(color);
			}
		}});
	connect(ri->actionBackgroundColorUp, &QAction::triggered,   [=]() {
		QColor color = QColorDialog::getColor();
		if (color.isValid()) ui->visualCloudWidget->setBackgroundColorUp(color);
		});
	connect(ri->actionBackgroundColorDown, &QAction::triggered, [=]() {
		QColor color = QColorDialog::getColor();
		if (color.isValid()) ui->visualCloudWidget->setBackgroundColorDown(color);
		});
	ui->visualChartWidget->setBackgroundColor(QColor("#F0F0F0"));


	// 设置背景图片
	connect(ri->buttonBackgroundImage, &QToolButton::clicked, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("图片文件名和路径不要带中文"), "", tr("图片 (*.png *.jpg)"));
		if (!fileName.isEmpty()) {
			if (isVisualCloud()) ui->visualCloudWidget->setBackgroundImage(fileName);
			if (isVisualChart()) ui->visualChartWidget->setBackgroundImage(fileName);
		}});


	// 设置点尺寸
	connect(ri->spinPointSize, QOverload<int>::of(&QSpinBox::valueChanged), ui->visualCloudWidget, &VisualCloudWidget::setPointsSize);
	ui->visualCloudWidget->setPointsSize(ri->spinPointSize->value());
	connect(ri->spinPointSize, QOverload<int>::of(&QSpinBox::valueChanged), ui->visualChartWidget, &VisualChartWidget::setMarkerSize);
	ui->visualChartWidget->setMarkerSize(ri->spinPointSize->value());

	connect(ri->spinPickedPointSize, QOverload<int>::of(&QSpinBox::valueChanged), ui->visualCloudWidget, &VisualCloudWidget::setPickedPointSize);
	ui->visualCloudWidget->setPickedPointSize(ri->spinPickedPointSize->value());


	// 刷新视图
	connect(ri->buttonRefreshView, &QToolButton::clicked, [=]() {
		if (isVisualChart()) ui->visualChartWidget->clear();
		refreshView();
		});
	connect(ri->buttonAddSeries, &QToolButton::clicked, this, &MainWindow::refreshView);

	// 版权声明
	QLabel* copyright = new QLabel("Copyright (C) 2024 西北工业大学-张璞玉. a11 rights reserved.");
	ui->statusBar->addPermanentWidget(copyright);
	ui->statusBar->showMessage(tr("支持的文件类型：stl csv nc. cls"));

	// 导出显示的点
	connect(ri->buttonExportVisualPoints, &QToolButton::clicked, this, &MainWindow::exportVisualPoints);

	// 显示拾取点
	connect(ui->visualCloudWidget, &VisualCloudWidget::picked, this, &MainWindow::cloudPointPickedHandler);
	connect(ui->visualChartWidget, &VisualChartWidget::picked, this, &MainWindow::chartPointPickedHandler);
	// 拾取指定的点
	connect(ri->actionPickFirstPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickFirst);
	connect(ri->actionPickLastPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickLast);
	connect(ri->actionPickMaxXPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMaxX);
	connect(ri->actionPickMinXPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMinX);
	connect(ri->actionPickMaxYPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMaxY);
	connect(ri->actionPickMinYPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMinY);
	connect(ri->actionPickMaxZPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMaxZ);
	connect(ri->actionPickMinZPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMinZ);
	connect(ri->actionPickMaxFPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMaxF);
	connect(ri->actionPickMinFPoint, &QAction::triggered, ui->visualCloudWidget, &VisualCloudLiveWidget::pickMinF);

	// 动态刷新逐个加载点
	connect(ri->buttonStartLiveView, &QToolButton::clicked, this, &MainWindow::startLiveView);
	connect(ri->buttonStopLiveView,  &QToolButton::clicked, this, &MainWindow::stopLiveView);
	connect(ui->visualChartWidget, &VisualChartLiveWidget::liveViewStarted, this, &MainWindow::liveViewStartedHandler);
	connect(ui->visualChartWidget, &VisualChartLiveWidget::liveViewStopped, this, &MainWindow::liveViewStoppedHandler);
	connect(ui->visualCloudWidget, &VisualCloudLiveWidget::liveViewStarted, this, &MainWindow::liveViewStartedHandler);
	connect(ui->visualCloudWidget, &VisualCloudLiveWidget::liveViewStopped, this, &MainWindow::liveViewStoppedHandler);


	// 显示/隐藏坐标轴和标尺
	connect(ri->checkAxisVisibility,	  &QCheckBox::stateChanged, ui->visualCloudWidget, &VisualCloudWidget::setAxisVisibility);
	connect(ri->checkScalarbarVisibility, &QCheckBox::stateChanged, ui->visualCloudWidget, &VisualCloudWidget::setScalarbarVisibility);

	// 切换显示模式
	connect(ri->radioCloud, &QRadioButton::toggled, [=](bool check) {
		if (check) ui->stackedWidget->setCurrentWidget(ui->visualCloudWidget); 
		else       ui->stackedWidget->setCurrentWidget(ui->visualChartWidget);
		});
	emit ri->radioCloud->toggled(true); // 默认显示三维点云
}


void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
		tr("CSV Files (*.csv);;STL Files (*.stl);;CNC (*.cls *.nc *.mpf)"));
	if (!fileName.isEmpty())
		read(fileName);
}

void MainWindow::read(const QString& fileName)
{
	ui->statusBar->showMessage(QFileInfo(fileName).fileName());

	if (fileName.toLower().endsWith(".stl"))	
		ui->visualCloudWidget->visualizeStl(fileName);
	if (fileName.toLower().endsWith(".csv"))	
		readCSV(fileName);
	if (fileName.toLower().endsWith(".nc") || fileName.toLower().endsWith(".mpf"))
	{
		m_ncReader = new NC::Toolpath::ReadGCode();
		readNC(fileName);
	}
	if (fileName.toLower().endsWith(".cls"))
	{
		m_ncReader = new NC::Toolpath::ReadAPT();
		readNC(fileName);
	}
}

void MainWindow::readCSV(const QString& fileName, bool hasHead)
{
	ui->visualChartWidget->setTitle(QFileInfo(fileName).baseName());

    Eigen::readCSV(m_oriMat, fileName, m_head, ",", ri->comboCodec->currentText());

	readFinishHandler();
}

void MainWindow::readNC(const QString& fileName)
{
	ui->visualChartWidget->setTitle(QFileInfo(fileName).baseName());

	m_ncReader->read(m_oriMat, fileName);
	m_head = m_ncReader->head().split(',');

	readFinishHandler();
}

void MainWindow::readFinishHandler()
{
	if (m_head.length() == 0)
	{
		QMessageBox::warning(this, "读取失败", "没有读取到任何数据");
		return;
	}

	// 添加一列，值为行号
	m_head.append("index");
	Eigen::addMatrixColumn(m_oriMat);

	ri->comboX->clear(); ri->comboX->addItems(m_head);
	ri->comboY->clear(); ri->comboY->addItems(m_head);
	ri->comboZ->clear(); ri->comboZ->addItems(m_head);
	ri->comboF->clear(); ri->comboF->addItems(m_head);

	ri->comboX->setCurrentIndex(0);
	ri->comboY->setCurrentIndex(m_head.length() > 2 ? 1 : m_head.length() - 1);
	ri->comboZ->setCurrentIndex(m_head.length() > 3 ? 2 : m_head.length() - 1);
	ri->comboF->setCurrentIndex(m_head.length() > 4 ? 3 : m_head.length() - 1);

	ri->spinFirstVisualPointNo->setRange(0, m_oriMat.rows());
	ri->spinFirstVisualPointNo->setValue(0);
	ri->spinLastVisualPointNo->setRange(0, m_oriMat.rows() - 1);
	ri->spinLastVisualPointNo->setValue(m_oriMat.rows() - 1);

	refreshView();

	QMessageBox::information(this, "读取成功", "读取到 " + QString::number(m_oriMat.rows()) + " 个点");
}

void MainWindow::liveViewStartedHandler()
{
	ri->tabFile->setEnabled(false);
	ri->tabView->setEnabled(false);
	ri->tabSet->setEnabled(false);

	ri->buttonStartLiveView->setEnabled(false);
	ri->buttonStopLiveView->setEnabled(true);
}

void MainWindow::liveViewStoppedHandler()
{
	ri->tabFile->setEnabled(true);
	ri->tabView->setEnabled(true);
	ri->tabSet->setEnabled(true);

	ri->buttonStartLiveView->setEnabled(true);
	ri->buttonStopLiveView->setEnabled(false);
}

void MainWindow::refreshView()
{
	if (hasData())
	{
		int startRow = ri->spinFirstVisualPointNo->value();
		int startCol = 0;
		int rowCount = ri->spinLastVisualPointNo->value() - startRow + 1;
		int colCount = m_oriMat.cols();
		m_visualMat = m_oriMat.block(startRow, startCol, rowCount, colCount);

		if (isVisualChart())
		{
			ui->visualChartWidget->visualizePoints(
				m_visualMat.col(ri->comboX->currentIndex()),
				m_visualMat.col(ri->comboY->currentIndex()),
				ri->comboX->currentText() + "-" + ri->comboY->currentText());
			ui->labelFeatureName->setText(ri->comboF->currentText() + ": ");
		}
		else
		{
			ui->visualCloudWidget->visualizePoints(
				m_visualMat.col(ri->comboX->currentIndex()),
				m_visualMat.col(ri->comboY->currentIndex()),
				m_visualMat.col(ri->comboZ->currentIndex()),
				m_visualMat.col(ri->comboF->currentIndex()),
				ri->comboF->currentText());
			ui->labelFeatureName->setText(ri->comboF->currentText() + ": ");
		}
	}
}

void MainWindow::clear()
{
	ui->visualCloudWidget->clear();
	ui->visualChartWidget->clear();
	ri->comboX->clear();
	ri->comboY->clear();
	ri->comboZ->clear();
	ri->comboF->clear();
	ui->statusBar->showMessage(tr("支持的文件类型：*.stl  *.csv  *.nc.  *.cls  *mpf"));

	m_visualMat.resize(0, 0);
	m_oriMat.resize(0, 0);

	ri->resetSpinVisualPointNo();
}

void MainWindow::exportVisualPoints()
{
	if (hasData())
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("导出所显示的点"), "", tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty())
		{
			Eigen::writeCSV(m_visualMat, fileName, m_head.join(','), ",", ri->comboCodec->currentText());
			QMessageBox::information(this, "导出成功", "导出到 " + fileName);
		}
	}
}

void MainWindow::cloudPointPickedHandler(vtkIdType id)
{
	if (id < 0 || id >= m_visualMat.rows())
	{
		ui->labelPickedX->setText("-");
		ui->labelPickedY->setText("-");
		ui->labelPickedZ->setText("-");
		ui->labelPickedF->setText("-");
		ui->labelPointId->setText("-");
		return;
	}

	float x = m_visualMat(id, ri->comboX->currentIndex());
	float y = m_visualMat(id, ri->comboY->currentIndex());
	float z = m_visualMat(id, ri->comboZ->currentIndex());
	float f = m_visualMat(id, ri->comboF->currentIndex());
	ui->labelPickedX->setText(QString::number(x));
	ui->labelPickedY->setText(QString::number(y));
	ui->labelPickedZ->setText(QString::number(z));
	ui->labelPickedF->setText(QString::number(f));
	ui->labelPointId->setText(QString::number(id));
}


void MainWindow::chartPointPickedHandler(const QPointF& point)
{
	ui->labelPickedX->setText(QString::number(point.x()));
	ui->labelPickedY->setText(QString::number(point.y()));
	ui->labelPickedZ->setText("-");
	ui->labelPickedF->setText("-");
	ui->labelPointId->setText("-");
}

void MainWindow::startLiveView()
{
	if (hasData())
	{
		if (isVisualCloud())
		{
			ui->visualCloudWidget->setData(
				m_oriMat.col(ri->comboX->currentIndex()),
				m_oriMat.col(ri->comboY->currentIndex()),
				m_oriMat.col(ri->comboZ->currentIndex()),
				m_oriMat.col(ri->comboF->currentIndex()),
				ri->comboF->currentText());
			ui->visualCloudWidget->setInterval(ri->spinLiveInterval->value());
			ui->visualCloudWidget->startLiveView();
		}
		else
		{
			ui->visualChartWidget->setData(
				m_oriMat.col(ri->comboX->currentIndex()),
				m_oriMat.col(ri->comboY->currentIndex()),
				ri->comboX->currentText() + "-" + ri->comboY->currentText());
			ui->visualChartWidget->setInterval(ri->spinLiveInterval->value());
			ui->visualChartWidget->startLiveView();
		}
	}
}

void MainWindow::stopLiveView()
{
	ui->visualCloudWidget->stopLiveView();
	ui->visualChartWidget->stopLiveView();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* ev)
{
	__super::dragEnterEvent(ev);
	if (ev->mimeData()->hasUrls())
		ev->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* ev)
{
	__super::dropEvent(ev);
	if (ev->mimeData()->hasUrls())
	{
		for (auto& url : ev->mimeData()->urls())
			read(url.toLocalFile());
		ev->acceptProposedAction();
	}
}

bool MainWindow::hasData()
{
	if (isVisualChart())
		return ri->comboX->currentIndex() >= 0 && ri->comboY->currentIndex() >= 0
			&& m_oriMat.rows();
	if (isVisualCloud())
		return ri->comboX->currentIndex() >= 0 && ri->comboY->currentIndex() >= 0
			&& ri->comboZ->currentIndex() >= 0 && ri->comboF->currentIndex() >= 0
			&& m_oriMat.rows();
    return false;
}
