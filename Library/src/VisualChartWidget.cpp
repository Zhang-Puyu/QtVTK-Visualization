#include "VisualChartWidget.h"

#include <QtCharts/QValueAxis>
#include <QSlider>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>
#include <QGraphicsLayout>

#include "EigenExtention.hpp"

QT_CHARTS_USE_NAMESPACE

#pragma execution_character_set("utf-8")

VisualChartWidget::VisualChartWidget(QWidget* parent)
	: QFlexibleChartView(parent)
{
	// 设置标题格式
	m_chart->setTitle("散点图");
	m_chart->setTitleFont(QFont("SimHei", 14));

    // 创建坐标轴
	m_chart->addAxis(m_axisX, Qt::AlignBottom);
	m_chart->addAxis(m_axisY, Qt::AlignLeft);

	m_chart->layout()->setContentsMargins(0, 0, 0, 0); // 设置外边距为 0
	m_chart->setMargins(QMargins(0, 10, 0, 5)); // 设置内边距为 10 像素
	m_chart->setBackgroundRoundness(0); // 设置背景无圆角

    // 设置坐标轴范围
    m_axisX->setRange(0, 1);
    m_axisY->setRange(0, 1);

    // 设置网格
	m_axisX->setGridLineVisible(true);
	m_axisY->setGridLineVisible(true);

    // 在标签和轴之间加空格
    Q_ASSERT(m_axisY);
    m_axisY->setLabelFormat("%.1f  ");

    this->setChart(m_chart);
}

VisualChartWidget::~VisualChartWidget()
{}

void VisualChartWidget::clear()
{
	m_chart->removeAllSeries();
	m_chart->setTitle("散点图");
	m_seriesMap.clear();

	m_colorIndex = 0;

    this->update();
}

void VisualChartWidget::visualizePoints(const Eigen::VectorXf& X, const Eigen::VectorXf& Y,
    const QString& seriesName, const QColor& color)
{
	QScatterSeries* m_liveSeries = new QtCharts::QScatterSeries();
    m_chart->addSeries(m_liveSeries);
    m_seriesMap.insert(seriesName, m_liveSeries);

	m_liveSeries->setName(seriesName.isEmpty() ? QString("series") + QString::number(m_seriesMap.size()) : seriesName);
    m_liveSeries->setMarkerSize(m_markerSize);

    if (color == Qt::white)
        m_liveSeries->setColor(nextColor());
    else
		m_liveSeries->setColor(color);

	m_liveSeries->setPointLabelsVisible(m_pointLabelVisible);
    m_liveSeries->setPointLabelsFormat("(@xPoint, @yPoint)");
    m_liveSeries->setPointLabelsClipping(true);
    m_liveSeries->setUseOpenGL(true);
	m_liveSeries->attachAxis(m_axisX);
	m_liveSeries->attachAxis(m_axisY);

	int size = std::min(X.size(), Y.size());
    for (int i = 0; i < size; ++i)
        m_liveSeries->append(X(i), Y(i));

	// 连接点击信号
	connect(m_liveSeries, &QScatterSeries::clicked, this, &VisualChartWidget::picked);

	// 设置坐标轴范围
	double minX, maxX, minY, maxY;
	Eigen::coeffRange(X, minX, maxX, 1.8);
	Eigen::coeffRange(Y, minY, maxY, 1.8);
	minX = std::min(minX, m_axisX->min());
	maxX = std::max(maxX, m_axisX->max());
	minY = std::min(minY, m_axisY->min());
	maxY = std::max(maxY, m_axisY->max());
    m_axisX->setRange(minX, maxX);
    m_axisY->setRange(minY, maxY);

    // 更新视图
	this->update();
}

