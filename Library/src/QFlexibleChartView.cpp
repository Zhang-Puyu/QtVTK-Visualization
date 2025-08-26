#include "qflexiblechartview.h"

#include <cmath>

QT_CHARTS_USE_NAMESPACE

QFlexibleChartView::QFlexibleChartView(QWidget *parent)
	: QChartView(parent)
{}

QFlexibleChartView::~QFlexibleChartView()
{}

void QFlexibleChartView::mouseMoveEvent(QMouseEvent* pEvent)
{
	if (m_bMiddleButtonPressed)
	{
		QPoint oDeltaPos = pEvent->pos() - m_oPrePos;
		this->chart()->scroll(-oDeltaPos.x(), oDeltaPos.y());
		m_oPrePos = pEvent->pos();
	}
	__super::mouseMoveEvent(pEvent);
}

void QFlexibleChartView::mousePressEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::MiddleButton)
	{
		m_bMiddleButtonPressed = true;
		m_oPrePos = pEvent->pos();
		this->setCursor(Qt::SizeAllCursor);
	}
	if (pEvent->button() == Qt::LeftButton)
	{
		m_bVerticalZoom = true;
		m_oPrePos = pEvent->pos();
		this->setCursor(Qt::SplitVCursor);
	}
	if (pEvent->button() == Qt::RightButton)
	{
		m_bHorizontalZoom = true;
		m_oPrePos = pEvent->pos();
		this->setCursor(Qt::SplitHCursor);
	}
	__super::mousePressEvent(pEvent);
}

void QFlexibleChartView::mouseReleaseEvent(QMouseEvent* pEvent)
{
	if (pEvent->button() == Qt::MiddleButton)
	{
		m_bMiddleButtonPressed = false;
		this->setCursor(Qt::ArrowCursor);
	}
	if (pEvent->button() == Qt::LeftButton)
	{
		m_bVerticalZoom = false;
		this->setCursor(Qt::ArrowCursor);
	}
	if (pEvent->button() == Qt::RightButton)
	{
		m_bHorizontalZoom = false;
		this->setCursor(Qt::ArrowCursor);
	}
	__super::mouseReleaseEvent(pEvent);
}

void QFlexibleChartView::wheelEvent(QWheelEvent* pEvent)
{
	qreal rVal = std::pow(0.999, pEvent->delta()); // ���ñ���
	// 1. ��ȡ��ͼ������Ϣ
	QRectF oPlotAreaRect = this->chart()->plotArea();
	QPointF oCenterPoint = oPlotAreaRect.center();
	// 2. ˮƽ����
	if(m_bHorizontalZoom)
		oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
	// 3. ��ֱ����
	if(m_bVerticalZoom)
		oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
	// ͬʱ����
	if ((!m_bHorizontalZoom) && (!m_bVerticalZoom))
	{
		oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
		oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
	}
	// 4.1 �����ӵ㣬�ӵ㲻�䣬Χ����������
	//QPointF oNewCenterPoint(oCenterPoint);
	// 4.2 �����ӵ㣬���������λ���ƶ�����������
	//QPointF oNewCenterPoint(pEvent->pos());
	// 4.3 �����ӵ㣬���������λ�þ������ֲ���(�ȱȻ��㣬����һ�����)
	QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
	// 5. �����ӵ�
	oPlotAreaRect.moveCenter(oNewCenterPoint);
	// 6. �ύ���ŵ���
	this->chart()->zoomIn(oPlotAreaRect);
	__super::wheelEvent(pEvent);
}
