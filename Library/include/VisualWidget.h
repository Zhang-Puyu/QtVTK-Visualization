#pragma once

#include "QVTKOpenGLWidget.h"
#include "qvector.h"
#include "qthread.h"
#include "qcheckbox.h"
#include "qdebug.h"

#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h" 
#include "vtkAxesActor.h"
#include "vtkScalarBarActor.h"
#include "vtkTextProperty.h"
#include "vtkLookupTable.h"
#include "vtkProperty.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyVertex.h"
#include "vtkPointData.h"
#include "vtkPolyLine.h"
#include "vtkLineSource.h"
#include "vtkSphereSource.h"    

#include "vtkAutoInit.h"
#ifndef VTKRENDERINGOPENGL2
#define VTKRENDERINGOPENGL2
VTK_MODULE_INIT(vtkRenderingOpenGL2)
#endif 
#ifndef VTKRENDERINGFREETYPE
#define VTKRENDERINGFREETYPE
VTK_MODULE_INIT(vtkRenderingFreeType)
#endif
#ifndef VTKINTERACTIONSTYLE
#define VTKINTERACTIONSTYLE
VTK_MODULE_INIT(vtkInteractionStyle)
#endif 
#ifndef VTKRENDERINGVOLUMEOPENGL2
#define VTKRENDERINGVOLUMEOPENGL2
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
#endif 

#include <Eigen/Dense>

#include "InteractorStyle.h"

class VisualWidget : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    VisualWidget(QWidget* parent = nullptr);
    ~VisualWidget();

    /// @brief ��ʾ��ɫ����
    void visualizePoints(const Eigen::VectorXf& X, const Eigen::VectorXf& Y, const Eigen::VectorXf& Z,
        const QColor& color = Qt::white);
    /// @brief ��ʾ��ɫ����
    void visualizePoints(const Eigen::VectorXf& X, const Eigen::VectorXf& Y, const Eigen::VectorXf& Z,
        const Eigen::VectorXf& F, const QString& fName = "Scalar");

    /// @brief ��ʾstl�ļ�
    void visualizeStl(const QString& fileName);

    /// @brief �������
    void hidePointsView();
    /// @brief ���stl
    void clearStlsView();
    /// @brief ���ȫ��
    void clear();

    /// @brief �����ϰ벿�ֱ�����ɫ
    void setBackgroundColorUp(const QColor& color);
    /// @brief �����°벿�ֱ�����ɫ
    void setBackgroundColorDown(const QColor& color);

    /// @brief ���õ��С
    void setPointsSize(int size);
    /// @brief ����ʰȡ���С
    void setPickedPointSize(int size);

    void setScalarbarVisibility(bool visibility) { m_scalarbarActor->SetVisibility(visibility); this->GetRenderWindow()->Render(); }
	void setAxisVisibility(bool visibility)      { m_axesActor->SetVisibility(visibility);      this->GetRenderWindow()->Render(); }

    /// @brief ʰȡ��
    void pick(const vtkIdType& id);

    friend class LiveViewModel;

protected:
    vtkSmartPointer<vtkRenderer>		m_renderer       = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkAxesActor>		m_axesActor      = vtkSmartPointer<vtkAxesActor>::New();
    vtkSmartPointer<vtkScalarBarActor>	m_scalarbarActor = vtkSmartPointer<vtkScalarBarActor>::New();
    vtkSmartPointer<vtkLookupTable>		m_lookupTable    = vtkSmartPointer<vtkLookupTable>::New();
    vtkSmartPointer<vtkActor>			m_pointsActor    = vtkSmartPointer<vtkActor>::New();

    /// @brief �洢��ʾ�ĵ�������Ϣ
    vtkSmartPointer<vtkPoints>	                m_points          = vtkSmartPointer<vtkPoints>::New();
    /// @brief ������
    vtkSmartPointer<PointPickerInteractorStyle> m_interactorStyle = vtkSmartPointer<PointPickerInteractorStyle>::New();

    vtkSmartPointer<vtkSphereSource>   m_pickedPointSphereSource = vtkSmartPointer<vtkSphereSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> m_pickedPointMapper       = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor>		   m_pickedPointActor        = vtkSmartPointer<vtkActor>::New();

    QVector<vtkSmartPointer<vtkActor>> m_stlActors;

    const vtkIdType lastPointId() { return m_points->GetNumberOfPoints() - 1; }
    const vtkIdType firstPointId = 0;

    vtkIdType m_pickedPointId = NO_POINT_PICKED;

    // ���̰����¼�
    virtual void keyPressEvent(QKeyEvent* event)   override;
    // ����̧���¼�
    virtual void keyReleaseEvent(QKeyEvent* event) override;

signals:
    void pointPicked(vtkIdType pointId);
};
 

