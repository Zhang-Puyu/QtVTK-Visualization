#pragma once

#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

#include <Eigen/Dense>

#include "QFlexibleChartView.h"

class VisualChartWidget : public QFlexibleChartView
{
	Q_OBJECT

public:
	VisualChartWidget(QWidget* parent = nullptr);
	~VisualChartWidget();

    /// @brief 新建Series显示单色点云
    void visualizePoints(const Eigen::VectorXf& X, const Eigen::VectorXf& Y,
		const QString& seriesName = "",
        const QColor& color = Qt::white);
	
	/// @brief 设置标题
	void setTitle(const QString& title) { 
		m_chart->setTitle(title); 
	}

	/// @brief 设置坐标值可见
	void setPointLabelVisible(bool visible) {
		m_pointLabelVisible = visible;
		for (QtCharts::QScatterSeries* m_liveSeries : m_seriesMap)
			m_liveSeries->setPointLabelsVisible(visible);
		this->update();
	}

	/// @brief 设置背景颜色
	void setBackgroundColor(const QColor& color) {
		m_chart->setBackgroundBrush(QBrush(color));
		this->update();
	}

	/// @brief 设置背景图片
	void setBackgroundImage(const QString& imageFile) {
		m_chart->setBackgroundVisible(false);
		setStyleSheet(QString("QGraphicsView { background-image: url(%1); }").arg(imageFile));
		this->update();
	}

	/// @brief 设置离散点大小
	void setMarkerSize(int size) {
		m_markerSize = size;
		for (QtCharts::QScatterSeries* m_liveSeries : m_seriesMap)
			m_liveSeries->setMarkerSize(size);
		this->update();
	}

	/// @brief 清空全部
	void clear();

protected:
    QtCharts::QChart* m_chart = new QtCharts::QChart;

	QtCharts::QValueAxis* m_axisX = new QtCharts::QValueAxis;
	QtCharts::QValueAxis* m_axisY = new QtCharts::QValueAxis;

	QMap<QString, QtCharts::QScatterSeries*> m_seriesMap;

	int m_markerSize = 10;
	bool m_pointLabelVisible = true;

	QColor nextColor()
	{
		if (m_colorIndex >= m_colorList.size()) 
			m_colorIndex = 0;
		return m_colorList[m_colorIndex++];
	}

	const QList<QColor> m_colorList = {
		Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan,
		Qt::magenta, Qt::darkRed, Qt::darkGreen, Qt::darkBlue,
		Qt::darkYellow, Qt::darkCyan, Qt::darkMagenta };
	int m_colorIndex = 0;

signals:
	void picked(const QPointF& point);
};

