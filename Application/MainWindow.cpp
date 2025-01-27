#include "MainWindow.h"

#include "qfiledialog.h"
#include "qevent.h"
#include "qmimedata.h"
#include "qcolordialog.h"
#include "qlabel.h"
#include "qmessagebox.h"

#pragma execution_character_set("utf-8")

#include "Reader.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
{
    ui->setupUi(this);
	ri = ui->ribbon;
	// �����ļ�����
	this->setAcceptDrops(true);
	// �����ļ�����
	this->setAcceptDrops(true);
	// ���ļ�
	connect(ri->buttonOpenFile, &QToolButton::clicked, this, &MainWindow::open);
	connect(ri->actionOpenCsv_NotSkipFirstRow, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("��csv�ļ�����һ��Ϊ����"), "",
			tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty()) readCsv(fileName, false);
		});
	connect(ri->actionOpenCsv_AndSkipFirstRow, &QAction::triggered, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("��csv�ļ�����һ��Ϊ��ͷ"), "",
			tr("CSV Files (*.csv)"));
		if (!fileName.isEmpty()) readCsv(fileName, true);
		});	
	// �����ͼ
	connect(ri->buttonClearView, &QToolButton::clicked, this, &MainWindow::clear);
	// ���ñ�����ɫ
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
	// ���ñ���ͼƬ
	connect(ri->buttonBackgroundImage, &QToolButton::clicked, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("ͼƬ�ļ�����·����Ҫ������"), "",
			tr("ͼƬ (*.png *.jpg)"));
		if (!fileName.isEmpty())
			ui->visualWidget->setBackgroundImage(fileName);
		});
	// ���õ�ߴ�
	connect(ri->spinPointSize, QOverload<int>::of(&QSpinBox::valueChanged),
		ui->visualWidget, &VisualWidget::setPointsSize);
	ui->visualWidget->setPointsSize(ri->spinPointSize->value());
	connect(ri->spinPickedPointSize, QOverload<int>::of(&QSpinBox::valueChanged),
		ui->visualWidget, &VisualWidget::setPickedPointSize);
	ui->visualWidget->setPickedPointSize(ri->spinPickedPointSize->value());
	// ˢ����ͼ
	connect(ri->buttonRefreshView, &QToolButton::clicked, this, &MainWindow::refresh);

	// ��Ȩ����
	QLabel* copyright = new QLabel("Copyright (C) 2024 ������ҵ��ѧ-�����. a11 rights reserved.");
	ui->statusBar->addPermanentWidget(copyright);
	ui->statusBar->showMessage(tr("֧�ֵ��ļ����ͣ�*.stl  *.csv  *.nc.  *.cls"));

	// ��ʾʰȡ��
	connect(ui->visualWidget, &VisualWidget::pointPicked, this, &MainWindow::pick);

	// ��̬ˢ��������ص�
	connect(ri->buttonStartLiveView, &QToolButton::clicked, this, &MainWindow::startLiveView);
	connect(ri->buttonStopLiveView,  &QToolButton::clicked, this, &MainWindow::stopLiveView);

	// ��ʾ/����������ͱ��
	connect(ri->checkAxisVisibility,	  &QCheckBox::stateChanged, ui->visualWidget, &VisualWidget::setAxisVisibility);
	connect(ri->checkScalarbarVisibility, &QCheckBox::stateChanged, ui->visualWidget, &VisualWidget::setScalarbarVisibility);
}


void MainWindow::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("���ļ�"), "",
		tr("CSV Files (*.csv);;STL Files (*.stl);;CNC (*.cls *.nc)"));
	if (!fileName.isEmpty())
		read(fileName);
}

void MainWindow::read(const QString& fileName)
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
		m_reader->readCsv(m_mat, fileName, m_head);
	}
	else
	{
		for (int i = 0; i < m_mat.cols(); i++)
			m_head.push_back(QString("col ") + QString::number(i));

		m_reader->readCsv(m_mat, fileName);
	}

	readFinishedHandler();
}


void MainWindow::readCnc(const QString& fileName)
{
	m_reader->readCnc(m_mat, fileName, m_head);

	readFinishedHandler();
}

void MainWindow::readFinishedHandler()
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

	refresh();

	QMessageBox::information(this, "��ȡ�ɹ�", "��ȡ�� " + QString::number(m_mat.rows()) + " ����");
}

void MainWindow::refresh()
{
	if (hasData())
	{
		int startRow = ri->spinFirstVisualPointNo->value();
		int startCol = 0;
		int rowCount = ri->spinLastVisualPointNo->value() - startRow + 1;
		int colCount = m_mat.cols();
		m_block = m_mat.block(startRow, startCol, rowCount, colCount);

		ui->visualWidget->visualizePoints(
			m_block.col(ri->comboX->currentIndex()),
			m_block.col(ri->comboY->currentIndex()),
			m_block.col(ri->comboZ->currentIndex()),
			m_block.col(ri->comboF->currentIndex()),
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
	ui->statusBar->showMessage(tr("֧�ֵ��ļ����ͣ�*.stl  *.csv  *.nc.  *.cls"));
}

void MainWindow::pick(vtkIdType id)
{
	if (id < 0 || id >= m_block.rows())
	{
		ui->labelPickedX->setText("-");
		ui->labelPickedY->setText("-");
		ui->labelPickedZ->setText("-");
		ui->labelPickedF->setText("-");
		ui->labelPointId->setText("-");
		return;
	}

	float x = m_block(id, ri->comboX->currentIndex());
	float y = m_block(id, ri->comboY->currentIndex());
	float z = m_block(id, ri->comboZ->currentIndex());
	float f = m_block(id, ri->comboF->currentIndex());
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
			read(url.toLocalFile());
		ev->acceptProposedAction();
	}
}

bool MainWindow::hasData()
{
	return ri->comboX->currentIndex() >= 0 && ri->comboY->currentIndex() >= 0
		&& ri->comboZ->currentIndex() >= 0 && ri->comboF->currentIndex() >= 0
		&& m_mat.rows();
}
