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
#include <QDebug>



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

	// 拾取点
	m_chart->addSeries(m_pickedPointSeries);
	m_pickedPointSeries->setMarkerSize(m_markerSize * 2);
	m_pickedPointSeries->setColor(Qt::red);
	m_pickedPointSeries->setUseOpenGL(true);
	m_pickedPointSeries->attachAxis(m_axisX);
	m_pickedPointSeries->attachAxis(m_axisY);

	connect(this, &VisualChartWidget::pointPicked, this, &VisualChartWidget::pointPickedHandler);

    this->setChart(m_chart);
}

VisualChartWidget::~VisualChartWidget()
{}

void VisualChartWidget::clear()
{
	m_pickedPointSeries->clear();

	foreach (QXYSeries* series, m_seriesMap.values())
		m_chart->removeSeries(series);

	m_chart->setTitle("散点图");

	m_seriesMap.clear();

	m_colorIndex = 0;

    this->update();
}

void VisualChartWidget::pointPickedHandler(const QPointF& point)
{
	m_pickedPointSeries->replace(m_lastPickedPoint, point);
	m_lastPickedPoint = point;
	
	this->update();
}

void VisualChartWidget::visualizePoints(const Eigen::VectorXf& X, const Eigen::VectorXf& Y,
    const QString& seriesName, QColor color)
{
	qDebug() << "inite viusal";

	color = color == Qt::white ? nextColor() : color;

	QXYSeries* series;
	if (m_seriesType == SCATTER)
	{
		series = new QtCharts::QScatterSeries();
		series->setColor(color);
		dynamic_cast<QScatterSeries*>(series)->setMarkerSize(m_markerSize);
	}
		
	else if (m_seriesType == LINE)
	{
		series = new QtCharts::QLineSeries();
		QPen pen(color);
		pen.setWidth(m_markerSize);
        series->setPen(pen);
	}
	else return;

        
    m_chart->addSeries(series);
    m_seriesMap.insert(seriesName, series);

	series->setName(seriesName.isEmpty() ? QString("series") + QString::number(m_seriesMap.size()) : seriesName);
	series->setPointLabelsVisible(m_pointLabelVisible);
    series->setPointLabelsFormat("(@xPoint, @yPoint)");
    series->setPointLabelsClipping(true);
    series->setUseOpenGL(true);
	series->attachAxis(m_axisX);
	series->attachAxis(m_axisY);

	int size = std::min(X.size(), Y.size());
	QList<QPointF> points;
    for (int i = 0; i < size; ++i)
        //series->append(X(i), Y(i));
		points << QPointF(X(i), Y(i));
	series->replace(points);

	// 连接点击信号
	connect(series, &QXYSeries::clicked, this, &VisualChartWidget::pointPicked);

	// 设置坐标轴范围
	double minX, maxX, minY, maxY;
	propRange(X, minX, maxX, 1.8);
	propRange(Y, minY, maxY, 1.8);
	minX = std::min(minX, m_axisX->min());
	maxX = std::max(maxX, m_axisX->max());
	minY = std::min(minY, m_axisY->min());
	maxY = std::max(maxY, m_axisY->max());
    m_axisX->setRange(minX, maxX);
    m_axisY->setRange(minY, maxY);

    // 更新视图
	this->update();
}

