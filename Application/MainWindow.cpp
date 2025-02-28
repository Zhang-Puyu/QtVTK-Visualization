#include "MainWindow.h"

#include "qfiledialog.h"
#include "qevent.h"
#include "qmimedata.h"
#include "qcolordialog.h"
#include "qlabel.h"
#include "qmessagebox.h"

#pragma execution_character_set("utf-8")

#include "Reader.h"
#include "MatrixExtention.hpp"

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
	connect(ri->buttonOpenFile, &QToolButton::clicked, this, &MainWindow::openFile);
	connect(ri->actionOpenCsv_NotSkipFirstRow, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("打开csv文件，第一行为数据"), "", tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty()) readCsv(fileName, false);
		});
	connect(ri->actionOpenCsv_AndSkipFirstRow, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("打开csv文件，第一行为表头"), "", tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty()) readCsv(fileName, true);
		});	
	// 清空视图
	connect(ri->buttonClearView, &QToolButton::clicked, this, &MainWindow::clear);
	// 设置背景颜色
	connect(ri->buttonBackgroundColor, &QToolButton::clicked, [=]() {
		QColor color = QColorDialog::getColor();
		if (color.isValid()){
			ui->visualWidget->setBackgroundColorUp(color);
			ui->visualWidget->setBackgroundColorDown(color);
		}});
	connect(ri->actionBackgroundColorUp, &QAction::triggered,   [=]() {
		QColor color = QColorDialog::getColor();
		if (color.isValid()) ui->visualWidget->setBackgroundColorUp(color);
		});
	connect(ri->actionBackgroundColorDown, &QAction::triggered, [=]() {
		QColor color = QColorDialog::getColor();
		if (color.isValid()) ui->visualWidget->setBackgroundColorDown(color);
		});
	// 设置背景图片
	connect(ri->buttonBackgroundImage, &QToolButton::clicked, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("图片文件名和路径不要带中文"), "", tr("图片 (*.png *.jpg)"));
		if (!fileName.isEmpty()) ui->visualWidget->setBackgroundImage(fileName);
		});
	// 设置点尺寸
	connect(ri->spinPointSize, QOverload<int>::of(&QSpinBox::valueChanged), ui->visualWidget, &VisualWidget::setPointsSize);
	ui->visualWidget->setPointsSize(ri->spinPointSize->value());
	connect(ri->spinPickedPointSize, QOverload<int>::of(&QSpinBox::valueChanged), ui->visualWidget, &VisualWidget::setPickedPointSize);
	ui->visualWidget->setPickedPointSize(ri->spinPickedPointSize->value());
	// 刷新视图
	connect(ri->buttonRefreshView, &QToolButton::clicked, this, &MainWindow::refreshView);

	// 版权声明
	QLabel* copyright = new QLabel("Copyright (C) 2024 西北工业大学-张璞玉. a11 rights reserved.");
	ui->statusBar->addPermanentWidget(copyright);
	ui->statusBar->showMessage(tr("支持的文件类型：*.stl  *.csv  *.nc.  *.cls"));

	// 导出显示的点
	connect(ri->buttonExportVisualPoints, &QToolButton::clicked, this, &MainWindow::exportVisualPoints);

	// 显示拾取点
	connect(ui->visualWidget, &VisualWidget::picked, this, &MainWindow::pickedEvent);

	connect(ri->actionPickFirstPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickFirst);
	connect(ri->actionPickLastPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickLast);
	connect(ri->actionPickMaxXPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMaxX);
	connect(ri->actionPickMinXPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMinX);
	connect(ri->actionPickMaxYPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMaxY);
	connect(ri->actionPickMinYPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMinY);
	connect(ri->actionPickMaxZPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMaxZ);
	connect(ri->actionPickMinZPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMinZ);
	connect(ri->actionPickMaxFPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMaxF);
	connect(ri->actionPickMinFPoint, &QAction::triggered, ui->visualWidget, &VisualLiveWidget::pickMinF);

	// 动态刷新逐个加载点
	connect(ri->buttonStartLiveView, &QToolButton::clicked, this, &MainWindow::startLiveView);
	connect(ri->buttonStopLiveView,  &QToolButton::clicked, this, &MainWindow::stopLiveView);

	// 显示/隐藏坐标轴和标尺
	connect(ri->checkAxisVisibility,	  &QCheckBox::stateChanged, ui->visualWidget, &VisualWidget::setAxisVisibility);
	connect(ri->checkScalarbarVisibility, &QCheckBox::stateChanged, ui->visualWidget, &VisualWidget::setScalarbarVisibility);
}


void MainWindow::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
		tr("CSV Files (*.csv);;STL Files (*.stl);;CNC (*.cls *.nc)"));
	if (!fileName.isEmpty())
		readFile(fileName);
}

void MainWindow::readFile(const QString& fileName)
{
	ui->statusBar->showMessage(QFileInfo(fileName).fileName());

	if (fileName.toLower().endsWith(".stl"))	
		ui->visualWidget->visualizeStl(fileName);
	if (fileName.toLower().endsWith(".csv"))	
		readCsv(fileName);
	if (fileName.toLower().endsWith(".nc") || fileName.toLower().endsWith(".cls")) 
		readCnc(fileName);
}

void MainWindow::readCsv(const QString& fileName, bool hasHead)
{
	if (hasHead)
	{
		m_reader->readCsv(m_mat, fileName, m_head, ri->comboCodec->currentText());
	}
	else
	{
		m_reader->readCsv(m_mat, fileName, ri->comboCodec->currentText());

		m_head.clear();
		for (int i = 0; i < m_mat.cols(); i++)
			m_head.push_back(QString("col ") + QString::number(i));
	}

	readEvent();
}

void MainWindow::readCnc(const QString& fileName)
{
	m_reader->readCnc(m_mat, fileName, m_head);

	readEvent();
}

void MainWindow::readEvent()
{
	ri->comboX->clear(); ri->comboX->addItems(m_head);
	ri->comboY->clear(); ri->comboY->addItems(m_head);
	ri->comboZ->clear(); ri->comboZ->addItems(m_head);
	ri->comboF->clear(); ri->comboF->addItems(m_head);

	ri->comboX->setCurrentIndex(0);
	ri->comboY->setCurrentIndex(1);
	ri->comboZ->setCurrentIndex(2);
	ri->comboF->setCurrentIndex(m_head.length() - 1);

	ri->spinFirstVisualPointNo->setRange(0, m_mat.rows());
	ri->spinFirstVisualPointNo->setValue(0);
	ri->spinLastVisualPointNo->setRange(0, m_mat.rows() - 1);
	ri->spinLastVisualPointNo->setValue(m_mat.rows() - 1);

	refreshView();

	QMessageBox::information(this, "读取成功", "读取到 " + QString::number(m_mat.rows()) + " 个点");
}

void MainWindow::refreshView()
{
	if (hasData())
	{
		int startRow = ri->spinFirstVisualPointNo->value();
		int startCol = 0;
		int rowCount = ri->spinLastVisualPointNo->value() - startRow + 1;
		int colCount = m_mat.cols();
		m_visualMat = m_mat.block(startRow, startCol, rowCount, colCount);

		ui->visualWidget->visualizePoints(
			m_visualMat.col(ri->comboX->currentIndex()),
			m_visualMat.col(ri->comboY->currentIndex()),
			m_visualMat.col(ri->comboZ->currentIndex()),
			m_visualMat.col(ri->comboF->currentIndex()),
			ri->comboF->currentText());
		ui->labelFeatureName->setText(ri->comboF->currentText() + ": ");
	}
}

void MainWindow::clear()
{
	ui->visualWidget->clear();
	ri->comboX->clear();
	ri->comboY->clear();
	ri->comboZ->clear();
	ri->comboF->clear();
	ui->statusBar->showMessage(tr("支持的文件类型：*.stl  *.csv  *.nc.  *.cls"));

	m_visualMat.resize(0, 0);
	m_mat.resize(0, 0);

	ri->resetSpinVisualPointNo();
}

void MainWindow::exportVisualPoints()
{
	if (hasData())
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("导出所显示的点"), "", tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty())
		{
			m_reader->writeCsv(m_visualMat, fileName, m_head, ri->comboCodec->currentText());
			QMessageBox::information(this, "导出成功", "导出到 " + fileName);
		}
	}
}

void MainWindow::pickedEvent(vtkIdType id)
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

void MainWindow::startLiveView()
{
	if (hasData())
	{
		ri->buttonStartLiveView->setEnabled(false);
		ri->buttonStopLiveView->setEnabled(true);

		ui->visualWidget->setData(
			m_mat.col(ri->comboX->currentIndex()),
			m_mat.col(ri->comboY->currentIndex()),
			m_mat.col(ri->comboZ->currentIndex()),
			m_mat.col(ri->comboF->currentIndex()),
			ri->comboF->currentText());
		ui->visualWidget->setInterval(ri->spinLiveInterval->value());
		ui->visualWidget->startLiveView();
	}
}

void MainWindow::stopLiveView()
{
	ri->buttonStartLiveView->setEnabled(true);
	ri->buttonStopLiveView->setEnabled(false);

	ui->visualWidget->stopLiveView();
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
			readFile(url.toLocalFile());
		ev->acceptProposedAction();
	}
}

bool MainWindow::hasData()
{
	return ri->comboX->currentIndex() >= 0 && ri->comboY->currentIndex() >= 0
		&& ri->comboZ->currentIndex() >= 0 && ri->comboF->currentIndex() >= 0
		&& m_mat.rows();
}
