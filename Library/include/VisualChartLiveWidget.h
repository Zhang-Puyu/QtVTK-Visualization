#pragma once

#include "VisualChartWidget.h"
#include "AbstractLiveWidget.h"

class VisualChartLiveWidget : public VisualChartWidget, public AbstractLiveWidget
{
	Q_OBJECT

public:
	VisualChartLiveWidget(QWidget* parent = nullptr);
	~VisualChartLiveWidget();

	/// @brief ��ʼ��̬��ʾ
	void startLiveView() override;

	/// @brief ��������
	void setData(const Eigen::VectorXf& X, const Eigen::VectorXf& Y, 
		const QString& seriesName)
	{
		m_liveX = X; m_liveY = Y;
		m_liveSeriesName = seriesName;
	}

private:
	/// @brief ��Ⱦ
	void render() override { this->update(); }

	Eigen::VectorXf m_liveX, m_liveY;

	QString m_liveSeriesName;
	QtCharts::QScatterSeries* m_liveSeries;

signals:
	void renderSignal();

	void liveViewStarted();
	void liveViewStopped();
};

