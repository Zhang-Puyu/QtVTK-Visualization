#pragma once

#include "VisualChartWidget.h"
#include "AbstractLiveWidget.h"

class VisualChartLiveWidget : public VisualChartWidget, public AbstractLiveWidget
{
	Q_OBJECT

public:
	VisualChartLiveWidget(QWidget* parent = nullptr);
	~VisualChartLiveWidget();

	/// @brief 开始动态显示
	void startLiveView() override;

	/// @brief 设置数据
	void setData(const Eigen::VectorXf& X, const Eigen::VectorXf& Y, 
		const QString& seriesName)
	{
		m_liveX = X; m_liveY = Y;
		m_liveSeriesName = seriesName;
	}

private:
	/// @brief 渲染
	void render() override { this->update(); }

	Eigen::VectorXf m_liveX, m_liveY;

	QString m_liveSeriesName;
	QtCharts::QScatterSeries* m_liveSeries;

signals:
	void renderSignal();

	void liveViewStarted();
	void liveViewStopped();
};

