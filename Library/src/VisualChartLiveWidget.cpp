#include "VisualChartLiveWidget.h"

#include "QtConcurrent/qtconcurrentrun.h"

#include "qdebug.h"

#include "EigenExtention.hpp"

QT_CHARTS_USE_NAMESPACE

#pragma execution_character_set("utf-8")

VisualChartLiveWidget::VisualChartLiveWidget(QWidget* parent) 
	: VisualChartWidget(parent),
	  AbstractLiveWidget()
{
	connect(this, &VisualChartLiveWidget::renderSignal, 
			this, &VisualChartLiveWidget::render,
			Qt::BlockingQueuedConnection);
}

VisualChartLiveWidget::~VisualChartLiveWidget()
{}

void VisualChartLiveWidget::startLiveView()
{
	this->clear();

	m_liveSeries = new QtCharts::QScatterSeries();
	m_chart->addSeries(m_liveSeries);
	m_seriesMap.insert(m_liveSeriesName, m_liveSeries);

	m_liveSeries->setName(m_liveSeriesName);
	m_liveSeries->setMarkerSize(m_markerSize);
	m_liveSeries->setColor(nextColor());

	m_liveSeries->setPointLabelsVisible(m_pointLabelVisible);
	m_liveSeries->setPointLabelsFormat("(@xPoint, @yPoint)");
	m_liveSeries->setPointLabelsClipping(true);
	m_liveSeries->setUseOpenGL(true);
	m_liveSeries->attachAxis(m_axisX);
	m_liveSeries->attachAxis(m_axisY);

	// 设置坐标轴范围
	double minX, maxX, minY, maxY;
	Eigen::coeffRange(m_liveX, minX, maxX, 1.8);
	Eigen::coeffRange(m_liveY, minY, maxY, 1.8);
	m_axisX->setRange(minX, maxX);
	m_axisY->setRange(minY, maxY);

	// 更新视图
	this->update();

	QtConcurrent::run([&]()
		{
			emit liveViewStarted();

			int size = std::min(m_liveX.size(), m_liveY.size());
			for (int i = 0; m_liveContinueFlag && i < size; ++i)
			{
				m_liveSeries->append(m_liveX(i), m_liveY(i));

				emit renderSignal();

				QThread::msleep(m_liveInterval);
			}

			qDebug() << "live finish";

			emit liveViewStopped();
		});
}
