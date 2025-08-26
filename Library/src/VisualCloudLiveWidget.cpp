

#include "vtkCaptionActor2D.h"
#include "vtkScalarBarActor.h"
#include "vtkActor2DCollection.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkSTLReader.h"
#include "vtkActor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkLookupTable.h"
#include "vtkPolyVertex.h"
#include "vtkPointData.h"
#include "vtkPolyLine.h"
#include "vtkLineSource.h"

#include "VisualCloudLiveWidget.h"

#include "EigenExtention.hpp"
#include "StringExtension.hpp"

#include "QtConcurrent/qtconcurrentrun.h"

#pragma execution_character_set("utf-8")

using namespace Eigen;

VisualCloudLiveWidget::VisualCloudLiveWidget(QWidget* parent)
	: VisualCloudWidget(parent),
	  AbstractLiveWidget()
{
	connect(this, &VisualCloudLiveWidget::renderSignal, 
			this, &VisualCloudLiveWidget::render,
			Qt::BlockingQueuedConnection);
}

VisualCloudLiveWidget::~VisualCloudLiveWidget()
{
	stopLiveView();
}

void VisualCloudLiveWidget::startLiveView()
{
	pick(NO_POINT_PICKED);

	float minScalar = m_liveF.minCoeff();
	float maxScalar = m_liveF.maxCoeff();

	m_livePoints	 = vtkSmartPointer<vtkPoints>::New();
	m_livePolyVertex = vtkSmartPointer<vtkPolyVertex>::New();
	m_liveScalars	 = vtkSmartPointer<vtkDoubleArray>::New();
	m_liveGrid	     = vtkSmartPointer<vtkUnstructuredGrid>::New();
	m_liveMapper	 = vtkSmartPointer<vtkDataSetMapper>::New();

	m_liveGrid->Allocate(1, 1);
	m_liveGrid->SetPoints(m_livePoints);
	m_liveGrid->GetPointData()->SetScalars(m_liveScalars);

	m_lookupTable->SetNumberOfTableValues(m_liveX.size());
	m_lookupTable->SetRange(minScalar, maxScalar);
	m_lookupTable->Build();

	m_liveMapper->SetInputData(m_liveGrid);
	m_liveMapper->ScalarVisibilityOn();
	m_liveMapper->SetScalarRange(minScalar, maxScalar);
	m_liveMapper->SetLookupTable(m_lookupTable);
	m_liveMapper->SetColorModeToDefault();

	m_pointsActor->SetMapper(m_liveMapper);
	m_pointsActor->SetVisibility(true);

	m_liveContinueFlag = true;

	QtConcurrent::run([&]()
		{
			emit liveViewStarted();

			for (int i = 0; m_liveContinueFlag && i < m_liveX.size(); ++i)
			{
				m_livePolyVertex->GetPointIds()->InsertNextId(
					m_livePoints->InsertNextPoint(m_liveX(i), m_liveY(i), m_liveZ(i)));
				m_liveScalars->InsertNextTuple1(m_liveF(i));

				m_liveGrid->Reset();
				m_liveGrid->InsertNextCell(m_livePolyVertex->GetCellType(), m_livePolyVertex->GetPointIds());

				m_liveGrid->UpdateCellGhostArrayCache();
				m_liveGrid->Modified();
				m_liveMapper->Update();
				m_liveMapper->Modified();

				emit renderSignal();

				QThread::msleep(m_liveInterval);
			}

			emit liveViewStopped();

			m_points = m_livePoints;
		});
}
