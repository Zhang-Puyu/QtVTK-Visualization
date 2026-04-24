#pragma once

#include <QWidget>
#include <QBrush>
#include <QColor>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <Eigen/Dense>

#include "QFlexibleChartView.h"

enum SeriesType { LINE, SCATTER };

class VisualChartWidget : public QFlexibleChartView
{
	Q_OBJECT

public:
	VisualChartWidget(QWidget* parent = nullptr);
	~VisualChartWidget();

    /// @brief 新建Series显示单色点云
    void visualizePoints(const Eigen::VectorXf& X, const Eigen::VectorXf& Y,
		const QString& seriesName = "",
        QColor color = Qt::white);
	
	/// @brief 设置标题
	void setTitle(const QString& title) { 
		m_chart->setTitle(title); 
	}

	/// @brief 设置坐标值可见
	void setPointLabelVisible(bool visible) {
		m_pointLabelVisible = visible;
		for (QtCharts::QXYSeries* series : m_seriesMap)
			series->setPointLabelsVisible(visible);
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
		for (QtCharts::QXYSeries* series : m_seriesMap) {
			if (QtCharts::QScatterSeries* scatterSeries =
				dynamic_cast<QtCharts::QScatterSeries*>(series)) {
				scatterSeries->setMarkerSize(size);
			}
			else {
				QPen pen = series->pen();
				pen.setWidth(size);
				series->setPen(pen);
			}
		};
		this->update();
	}

	/// @brief 设置拾取点大小
	void setPickedPointSize(int size) {
		m_pickedPointSeries->setMarkerSize(size);
		this->update();
	}

	void setSeriesType(SeriesType seriesType) {
		m_seriesType = seriesType;
	}

	/// @brief 清空全部
	void clear();

protected:
    QtCharts::QChart* m_chart = new QtCharts::QChart;

	SeriesType m_seriesType = SCATTER;

	/// @brief 横坐标轴
	QtCharts::QValueAxis* m_axisX = new QtCharts::QValueAxis;
    /// @brief 纵坐标轴
	QtCharts::QValueAxis* m_axisY = new QtCharts::QValueAxis;

	QMap<QString, QtCharts::QXYSeries*> m_seriesMap;

	QtCharts::QScatterSeries* m_pickedPointSeries 
		= new QtCharts::QScatterSeries;
	QPointF m_lastPickedPoint;

	/// @brief 点尺寸
	int m_markerSize = 10;
	bool m_pointLabelVisible = true;

	QColor nextColor(){
		if (m_colorIndex >= m_colorList.size()) 
			m_colorIndex = 0;
		return m_colorList[++m_colorIndex];
	}

	static void propRange(const Eigen::VectorXf& vector, 
		double& propMin, double& propMax, double prop) {
		double oriMin = vector.minCoeff();
		double oriMax = vector.maxCoeff();		
		propMin = oriMin - (oriMax - oriMin) * (prop - 1);
		propMax = oriMax + (oriMax - oriMin) * (prop - 1);
	}

	/// @brief 颜色表
	const QList<QColor> m_colorList = {
		Qt::blue, Qt::magenta, Qt::darkRed, Qt::darkGreen, Qt::darkBlue,
		Qt::darkYellow, Qt::darkCyan, Qt::darkMagenta
	};

	/// @brief 当前颜色索引
	int m_colorIndex = 0;

	/// @brief 拾取点处理事件
	/// @param point 拾取点坐标
	void pointPickedHandler(const QPointF& point);

signals:
	/// @brief 拾取点信号
	void pointPicked(const QPointF& point);
};

