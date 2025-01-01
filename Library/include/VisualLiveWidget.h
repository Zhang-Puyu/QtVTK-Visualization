#pragma once

#include "VisualWidget.h"
#include "Reader.h"

class VisualLiveWidget : public VisualWidget
{
	Q_OBJECT

public:
	VisualLiveWidget(QWidget* parent = nullptr);
	~VisualLiveWidget();

	/// @brief 开始动态显示
	void startLiveView();
	/// @brief 停止动态显示
	void stopLiveView() { m_liveContinueFlag = false; }

	/// @brief 设置刷新间隔
	/// @param interval 刷新间隔: ms
	void setInterval(unsigned int interval) { m_liveInterval = interval; }
	/// @brief 设置持续时间
	/// @param duration 持续时间: s
	void setDuration(unsigned int duration) { m_liveDuration = duration; }

	/// @brief 设置数据
	void setData(const Eigen::VectorXf& X, const Eigen::VectorXf& Y, const Eigen::VectorXf& Z,
		const Eigen::VectorXf& F, const QString& fName = "Scalar") {
		m_liveX = X; m_liveY = Y; m_liveZ = Z; m_liveF = F;
		m_scalarbarActor->SetTitle(Reader::hasChinese(fName) ? "Scalar" : fName.toLocal8Bit());
	}

private:

	/// @brief 渲染
	void render() { this->GetRenderWindow()->Render(); }

	vtkSmartPointer<vtkPoints>	         m_livePoints;
	vtkSmartPointer<vtkPolyVertex>       m_livePolyVertex;
	vtkSmartPointer<vtkDoubleArray>      m_liveScalars;
	vtkSmartPointer<vtkUnstructuredGrid> m_liveGrid;
	vtkSmartPointer<vtkDataSetMapper>    m_liveMapper;

	unsigned int m_liveInterval = 50; // ms
	unsigned int m_liveDuration = 10; // s

	/// @brief 是否继续动态显示
	bool m_liveContinueFlag	= true;

	Eigen::VectorXf m_liveX, m_liveY, m_liveZ, m_liveF;

signals:
	void renderSignal();

	void liveViewStarted();
	void liveViewStopped();
};
