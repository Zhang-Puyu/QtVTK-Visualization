#include "MainWindow.h"

#include "qfiledialog.h"
#include "qevent.h"
#include "qdrag.h"
#include "qmimedata.h"
#include "qcolordialog.h"
#include "qlabel.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "QtConcurrent/qtconcurrentrun.h"

#pragma execution_character_set("utf-8")

#include "Reader.h"


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
	connect(ri->actionOpenCsv_NotSkipFirstRow, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("打开csv文件，第一行为数据"), "",
			tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty()) readCsv(fileName, false);
		});
	connect(ri->actionOpenCsv_AndSkipFirstRow, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("打开csv文件，第一行为表头"), "",
			tr("CSV Files (*.csv)"));
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
	// 设置点尺寸
	connect(ri->spinPointSize, QOverload<int>::of(&QSpinBox::valueChanged),
		ui->visualWidget, &VisualWidget::setPointsSize);
	ui->visualWidget->setPointsSize(ri->spinPointSize->value());
	connect(ri->spinPickedPointSize, QOverload<int>::of(&QSpinBox::valueChanged),
		ui->visualWidget, &VisualWidget::setPickedPointSize);
	ui->visualWidget->setPickedPointSize(ri->spinPickedPointSize->value());
	// 刷新视图
	connect(ri->buttonRefreshView, &QToolButton::clicked, this, &MainWindow::refresh);

	// 版权声明
	QLabel* copyright = new QLabel("Copyright (C) 2024 西北工业大学-张璞玉. a11 rights reserved.");
	ui->statusBar->addPermanentWidget(copyright);
	ui->statusBar->showMessage(tr("支持的文件类型：*.stl  *.csv  *.nc.  *.cls"));

	// 显示拾取点
	connect(ui->visualWidget, &VisualWidget::pointPicked, this, &MainWindow::pick);

	// 动态刷新逐个加载点
	connect(ri->buttonStartLiveView, &QToolButton::clicked, this, &MainWindow::startLiveView);
	connect(ri->buttonStopLiveView,  &QToolButton::clicked, this, &MainWindow::stopLiveView);

	connect(ri->checkAxisVisibility,	  &QCheckBox::stateChanged, ui->visualWidget, &VisualWidget::setAxisVisibility);
	connect(ri->checkScalarbarVisibility, &QCheckBox::stateChanged, ui->visualWidget, &VisualWidget::setScalarbarVisibility);
}


void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
		tr("CSV Files (*.csv);;STL Files (*.stl);;CNC (*.cls *.nc)"));
	if (!fileName.isEmpty())
		read(fileName);
}

void MainWindow::read(const QString& fileName)
{
	if (fileName.toLower().endsWith(".stl"))	readStl(fileName);
	if (fileName.toLower().endsWith(".csv"))	readCsv(fileName);
	if (fileName.toLower().endsWith(".nc") 
		|| fileName.toLower().endsWith(".cls")) readCnc(fileName);
}

void MainWindow::readCsv(const QString& fileName, bool hasHead)
{
	QStringList head;
	if (hasHead)
	{
		mat = Reader::readCsv(fileName, head);
	}
	else
	{
		mat = Reader::readCsv(fileName);
		for (int i = 0; i < mat.cols(); i++)
			head.push_back(QString("col ") + QString::number(i));
	}

	ri->comboX->clear(); ri->comboX->addItems(head);
	ri->comboY->clear(); ri->comboY->addItems(head);
	ri->comboZ->clear(); ri->comboZ->addItems(head);
	ri->comboF->clear(); ri->comboF->addItems(head);

	ri->comboX->setCurrentIndex(0 < head.length() - 1 ? 0 : head.length() - 1);
	ri->comboY->setCurrentIndex(1 < head.length() - 1 ? 1 : head.length() - 1);
	ri->comboZ->setCurrentIndex(2 < head.length() - 1 ? 2 : head.length() - 1);
	ri->comboF->setCurrentIndex(3 < head.length() - 1 ? 3 : head.length() - 1);

	// 获取不包含路径的文件名
	ui->statusBar->showMessage(QFileInfo(fileName).fileName());
	QMessageBox::information(this, "读取成功", "读取点数量：" + QString::number(mat.rows()));

	ri->spinFirstVisualPointNo->setRange(0, mat.rows());
	ri->spinFirstVisualPointNo->setValue(0);
	ri->spinLastVisualPointNo->setRange(0, mat.rows() - 1);
	ri->spinLastVisualPointNo->setValue(mat.rows() - 1);

	refresh();
}

void MainWindow::readStl(const QString& fileName)
{
	ui->visualWidget->visualizeStl(fileName);
}

void MainWindow::readCnc(const QString& fileName)
{
	QStringList head;
	mat = Reader::readCnc(fileName, head);
	ri->comboX->clear(); ri->comboX->addItems(head);
	ri->comboY->clear(); ri->comboY->addItems(head);
	ri->comboZ->clear(); ri->comboZ->addItems(head);
	ri->comboF->clear(); ri->comboF->addItems(head);

	ri->comboX->setCurrentIndex(0);
	ri->comboY->setCurrentIndex(1);
	ri->comboZ->setCurrentIndex(2);
	ri->comboF->setCurrentIndex(7);

	ui->statusBar->showMessage(fileName);
	QMessageBox::information(this, "读取成功", "读取点数量：" + QString::number(mat.rows()));

	ri->spinFirstVisualPointNo->setRange(0, mat.rows());
	ri->spinFirstVisualPointNo->setValue(0);
	ri->spinLastVisualPointNo->setRange(0, mat.rows() - 1);
	ri->spinLastVisualPointNo->setValue(mat.rows() - 1);

	refresh();
}

void MainWindow::refresh()
{
	if (hasData())
	{
		int startRow = ri->spinFirstVisualPointNo->value();
		int startCol = 0;
		int rowCount = ri->spinLastVisualPointNo->value() - startRow + 1;
		int colCount = mat.cols();
		visualMat = mat.block(startRow, startCol, rowCount, colCount);

		ui->visualWidget->visualizePoints(
			visualMat.col(ri->comboX->currentIndex()),
			visualMat.col(ri->comboY->currentIndex()),
			visualMat.col(ri->comboZ->currentIndex()),
			visualMat.col(ri->comboF->currentIndex()),
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
}

void MainWindow::pick(vtkIdType id)
{
	if (id < 0 || id >= visualMat.rows())
	{
		ui->labelPickedX->setText("-");
		ui->labelPickedY->setText("-");
		ui->labelPickedZ->setText("-");
		ui->labelPickedF->setText("-");
		ui->labelPointId->setText("-");
		return;
	}

	float x = visualMat(id, ri->comboX->currentIndex());
	float y = visualMat(id, ri->comboY->currentIndex());
	float z = visualMat(id, ri->comboZ->currentIndex());
	float f = visualMat(id, ri->comboF->currentIndex());
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
			mat.col(ri->comboX->currentIndex()),
			mat.col(ri->comboY->currentIndex()),
			mat.col(ri->comboZ->currentIndex()),
			mat.col(ri->comboF->currentIndex()),
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
			read(url.toLocalFile());
		ev->acceptProposedAction();
	}
}

bool MainWindow::hasData()
{
	return ri->comboX->currentIndex() >= 0 && ri->comboY->currentIndex() >= 0
		&& ri->comboZ->currentIndex() >= 0 && ri->comboF->currentIndex() >= 0
		&& mat.rows();
}
