#include "vtkCaptionActor2D.h"
#include "vtkScalarBarActor.h"
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
#include "vtkPNGReader.h"

#include <vtkCamera.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>
#include <vtkNew.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include "qmessagebox.h"
#include "qevent.h"

#include "VisualWidget.h"
#include "Reader.h"

#pragma execution_character_set("utf-8")

using namespace Eigen;

VisualWidget::VisualWidget(QWidget* parent)
	: QVTKOpenGLWidget(parent)
{
	// ����������
	m_axesActor->SetTotalLength(20, 20, 20);
	m_axesActor->SetTipTypeToCone();
	m_axesActor->AxisLabelsOn();

	m_axesActor->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	m_axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
	m_axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);

	m_axesActor->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	m_axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
	m_axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 1, 0);

	m_axesActor->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	m_axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
	m_axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 1);

	m_renderer->AddActor(m_axesActor);

	// ���ý��䱳��
	m_renderer->SetBackground(0.1, 0.2, 0.4);
	m_renderer->SetBackground2(1, 1, 1);
	m_renderer->SetGradientBackground(1);

	// ������ɫӳ��
	m_lookupTable->SetNumberOfColors(256);
	m_lookupTable->SetHueRange(0.7, 0);       // ��ɫɫ��
	m_lookupTable->SetAlphaRange(1.0, 1.0);   // ��ɫ���Ͷ�
	m_lookupTable->SetValueRange(1.0, 1.0);
	m_lookupTable->SetSaturationRange(0.8, 0.8);  // ��ɫ����
	m_lookupTable->Build();

	// ������ɫ������
	m_scalarbarActor->GetTitleTextProperty()->SetColor(0.1, 0.1, 0.1);
	m_scalarbarActor->GetTitleTextProperty()->SetFontSize(3);
	m_scalarbarActor->GetTitleTextProperty()->ItalicOff();
	m_scalarbarActor->SetNumberOfLabels(5); // ����5����ǩ
	m_scalarbarActor->SetHeight(0.70);
	m_scalarbarActor->SetWidth(0.10);
	m_scalarbarActor->SetLookupTable(m_lookupTable);

	m_renderer->AddActor2D(m_scalarbarActor);

	m_pointsActor->GetProperty()->SetRepresentationToPoints();
	m_pointsActor->GetProperty()->SetPointSize(5);
	m_pointsActor->SetVisibility(false);

	m_renderer->AddActor(m_pointsActor);

	// ��ʰȡ����б��
	m_pickedPointSphereSource->Update();
	m_pickedPointMapper->SetInputConnection(m_pickedPointSphereSource->GetOutputPort());
	m_pickedPointActor->SetMapper(m_pickedPointMapper);
	m_pickedPointActor->SetScale(1);
	m_pickedPointActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
	m_pickedPointActor->SetVisibility(false);

	m_renderer->AddActor(m_pickedPointActor);

	// ����һ�����������������Զ����ʰȡ��ʽ
	m_interactorStyle->SetDefaultRenderer(m_renderer);
	this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(m_interactorStyle);
	connect(m_interactorStyle->GetSignalSender(), &PointPickerSignalSender::sendPickedPointIdSignal, this, &VisualWidget::pick);

	this->GetRenderWindow()->AddRenderer(m_renderer);
	this->GetRenderWindow()->Render();
}

VisualWidget::~VisualWidget()
{
}

void VisualWidget::visualizePoints(const VectorXf& X, const VectorXf& Y, const VectorXf& Z, 
	const QColor& color)
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

	for (int i = 0; i < X.size(); ++i)
	{
		vtkIdType vtkId = points->InsertNextPoint(X(i), Y(i), Z(i));
		cells->InsertNextCell(1, &vtkId);
	};
	// ��¼������
	m_points = points;
	// ȡ��ѡ�еĵ�
	pick(NO_POINT_PICKED);

	//����vtkPolyData����
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(m_points);
	polyData->SetVerts(cells);

	m_scalarbarActor->SetTitle("");

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);

	m_pointsActor->SetMapper(mapper);
	m_pointsActor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
	m_pointsActor->SetVisibility(true);

	this->GetRenderWindow()->Render();
}

void VisualWidget::visualizePoints(const VectorXf& X, const VectorXf& Y, const VectorXf& Z, 
	const VectorXf& F, const QString& fName)
{
	// ��������  ��������  ��������
	vtkSmartPointer<vtkPoints>	    points	   = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPolyVertex>  polyVertex = vtkSmartPointer<vtkPolyVertex>::New();
	vtkSmartPointer<vtkDoubleArray> scalars    = vtkSmartPointer<vtkDoubleArray>::New();

	for (int i = 0; i < X.size(); i++)
	{
		vtkIdType vtkId = points->InsertNextPoint(X(i), Y(i), Z(i));
		polyVertex->GetPointIds()->InsertNextId(vtkId);
		scalars->InsertNextTuple1(F(i));
	}
	// ��¼������
	m_points = points;
	// ȡ��ѡ�еĵ�
	pick(NO_POINT_PICKED);

	float minScalar = F.minCoeff(); 
	float maxScalar = F.maxCoeff();

	// ������������������ϳ�һ���ṹΪvtkUnstructureGrid���ݼ�
	vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	grid->Allocate(1, 1);
	grid->SetPoints(m_points);
	grid->InsertNextCell(polyVertex->GetCellType(), polyVertex->GetPointIds());
	grid->GetPointData()->SetScalars(scalars);

	m_lookupTable->SetNumberOfTableValues(X.size());
	m_lookupTable->SetRange(minScalar, maxScalar);
	m_lookupTable->Build();

	if (Reader::hasChinese(fName))
	{
		QMessageBox::warning(this, "����", "ScalarBar���ⲻ֧����ʾ�����ַ�", QMessageBox::Ok);
		m_scalarbarActor->SetTitle("Scalar");
	}
	else
	{
		m_scalarbarActor->SetTitle(fName.toLocal8Bit());
	}

	// �����������ݵ������Сֵ������ɫ����������ݹ���
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(grid);
	mapper->ScalarVisibilityOn();
	mapper->SetScalarRange(minScalar, maxScalar);
	mapper->SetLookupTable(m_lookupTable);
	mapper->SetColorModeToDefault();

	m_pointsActor->SetMapper(mapper);
	m_pointsActor->SetVisibility(true);

	this->GetRenderWindow()->Render();
}

void VisualWidget::visualizeStl(const QString& fileName)
{
	vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	stlReader->SetFileName(fileName.toLocal8Bit());
	stlReader->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(stlReader->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	m_renderer->AddActor(actor);

	m_stlActors.push_back(actor);

	this->GetRenderWindow()->Render();
}

void VisualWidget::hidePointsView()
{
	m_pointsActor->SetVisibility(false);
	m_scalarbarActor->SetTitle("");
	pick(NO_POINT_PICKED);

	this->GetRenderWindow()->Render();
}

void VisualWidget::clearStlsView()
{
	for (auto& actor : m_stlActors)
		m_renderer->RemoveActor(actor);
	m_stlActors.clear();

	this->GetRenderWindow()->Render();
}

void VisualWidget::clear()
{
	hidePointsView();
	clearStlsView();

	this->GetRenderWindow()->Render();
}

void VisualWidget::setBackgroundColorUp(const QColor& color)
{
	if (color.isValid())
	{
		m_renderer->SetBackground2(color.redF(), color.greenF(), color.blueF());
		this->GetRenderWindow()->Render();
	}
}

void VisualWidget::setBackgroundColorDown(const QColor& color)
{
	if (color.isValid())
	{
		m_renderer->SetBackground(color.redF(), color.greenF(), color.blueF());
		this->GetRenderWindow()->Render();
	}
}


void VisualWidget::setBackgroundImage(const QString& imageFile)
{
	if (Reader::hasChinese(imageFile))
	{
		QMessageBox::critical(this, "����", "����VTK�����������ļ�", QMessageBox::Ok);
		return;
	}

	// ��ȡͼƬ
	vtkNew<vtkImageReader2Factory> readerFactory;
	vtkSmartPointer<vtkImageReader2> imageReader;
	imageReader.TakeReference(readerFactory->CreateImageReader2(imageFile.toLocal8Bit()));
	imageReader->SetFileName(imageFile.toLocal8Bit());
	imageReader->Update();

	vtkSmartPointer<vtkImageData> imageData = imageReader->GetOutput();

	// ����һ��vtkImageActor
	vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
	imageActor->SetInputData(imageData);

	// ����һ���µ���Ⱦ�����ڱ���
	vtkSmartPointer<vtkRenderer> backgroundRenderer = vtkSmartPointer<vtkRenderer>::New();
	backgroundRenderer->SetLayer(0);
	backgroundRenderer->InteractiveOff();

	// ������ͼƬ��ӵ�������Ⱦ����
	backgroundRenderer->AddActor(imageActor);

	// ������Ⱦ���ڵĲ������������Ⱦ��
	this->GetRenderWindow()->SetNumberOfLayers(2);
	this->GetRenderWindow()->AddRenderer(backgroundRenderer);
	m_renderer->SetLayer(1);

	// ���ñ�������������Ⱦ��
	double origin[3];
	double spacing[3];
	int extent[6];
	imageData->GetOrigin(origin);
	imageData->GetSpacing(spacing);
	imageData->GetExtent(extent);

	vtkSmartPointer<vtkCamera> camera = backgroundRenderer->GetActiveCamera();
	camera->ParallelProjectionOn();

	double xc = origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0];
	double yc = origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1];
	double yd = (extent[3] - extent[2] + 1) * spacing[1];
	double d = camera->GetDistance();
	camera->SetParallelScale(0.5 * yd);
	camera->SetFocalPoint(xc, yc, 0.0);
	camera->SetPosition(xc, yc, d);

	// ��Ⱦ����
	this->GetRenderWindow()->Render();
}

void VisualWidget::setPointsSize(int size)
{
	m_pointsActor->GetProperty()->SetPointSize(size > 0 ? size : 1);
	this->GetRenderWindow()->Render();
}

void VisualWidget::setPickedPointSize(int size)
{
	m_pickedPointActor->SetScale(size > 0 ? size : 1);
	this->GetRenderWindow()->Render();
}

void VisualWidget::pick(const vtkIdType& id)
{
	if (m_points->GetNumberOfPoints() > 0)
	{
		m_pickedPointId = id;

		if (m_pointsActor->GetVisibility() && id >= 0)
		{
			double* pos = m_points->GetPoint(id);
			m_pickedPointActor->SetPosition(pos[0], pos[1], pos[2]);
			m_pickedPointActor->SetVisibility(true);
			this->GetRenderWindow()->Render();

			emit pointPicked(id);
		}
		else // id == -1
		{
			m_pickedPointActor->SetVisibility(false);
			this->GetRenderWindow()->Render();

			emit pointPicked(NO_POINT_PICKED);
		}
	}
}

void VisualWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Left)
		pick(m_pickedPointId <= firstPointId  ? 0			  : m_pickedPointId - 1);
	if (event->key() == Qt::Key_Right)
		pick(m_pickedPointId >= lastPointId() ? lastPointId() : m_pickedPointId + 1);

	__super::keyPressEvent(event);
}

void VisualWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Up)
		pick(firstPointId);
	if (event->key() == Qt::Key_Down)
		pick(lastPointId());

	__super::keyReleaseEvent(event);
}
