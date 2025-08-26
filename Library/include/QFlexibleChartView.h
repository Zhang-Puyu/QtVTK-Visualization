#pragma once

#include <QtCharts/QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>

class QFlexibleChartView : public QtCharts::QChartView
{
	Q_OBJECT

public:
	QFlexibleChartView(QWidget* parent = nullptr);
	~QFlexibleChartView();
protected:
	virtual void mouseMoveEvent(QMouseEvent* pEvent) override;
	virtual void mousePressEvent(QMouseEvent* pEvent) override;
	virtual void mouseReleaseEvent(QMouseEvent* pEvent) override;
	virtual void wheelEvent(QWheelEvent* pEvent) override;
private:
	bool m_bMiddleButtonPressed =false;
	bool m_bHorizontalZoom = false;
	bool m_bVerticalZoom = false;
	QPoint m_oPrePos;
};
