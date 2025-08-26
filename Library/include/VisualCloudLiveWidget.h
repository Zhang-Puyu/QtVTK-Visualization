#pragma once

#include "VisualCloudWidget.h"
#include "StringExtension.hpp"

#include "AbstractLiveWidget.h"

class VisualCloudLiveWidget : public VisualCloudWidget, public AbstractLiveWidget
{
	Q_OBJECT

public:
	VisualCloudLiveWidget(QWidget* parent = nullptr);
	~VisualCloudLiveWidget();

	/// @brief 开始动态显示
	void startLiveView() override;

	/// @brief 设置数据
	void setData(const Eigen::VectorXf& X, const Eigen::VectorXf& Y, const Eigen::VectorXf& Z,
		const Eigen::VectorXf& F, 
		const QString& fName = "Scalar") 
	{
		m_liveX = X; m_liveY = Y; m_liveZ = Z; m_liveF = F;
		m_scalarbarActor->SetTitle(String::hasChinese(fName) ? "Scalar" : fName.toLocal8Bit());
	}

private:
	/// @brief 渲染
	void render() override { this->GetRenderWindow()->Render(); }

	vtkSmartPointer<vtkPoints>	         m_livePoints;
	vtkSmartPointer<vtkPolyVertex>       m_livePolyVertex;
	vtkSmartPointer<vtkDoubleArray>      m_liveScalars;
	vtkSmartPointer<vtkUnstructuredGrid> m_liveGrid;
	vtkSmartPointer<vtkDataSetMapper>    m_liveMapper;

	Eigen::VectorXf m_liveX, m_liveY, m_liveZ, m_liveF;

signals:
	void renderSignal();

	void liveViewStarted();
	void liveViewStopped();
};
