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

/// @brief X��Y��Z������ֵ����ֵ��Ӧ�����������
struct ValueRangeIndexes
{
    unsigned int x = 0, y = 0, z = 0, f = 0;
};

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

    /// @brief ���ñ���ͼƬ
	/// @param imageFile ͼƬ�ļ�·��
    void setBackgroundImage(const QString& imageFile);

    /// @brief ���õ��С
    void setPointsSize(int size);
    /// @brief ����ʰȡ���С
    void setPickedPointSize(int size);

    /// @brief ����ɫ���ɼ���
    void setScalarbarVisibility(bool visibility) { m_scalarbarActor->SetVisibility(visibility); this->GetRenderWindow()->Render(); }
	/// @brief ����������ɼ���
	void setAxisVisibility(bool visibility)      { m_axesActor->SetVisibility(visibility);      this->GetRenderWindow()->Render(); }

    /// @brief ʰȡ��
    void pick(const vtkIdType& id);
    
    void pickFirst() { pick(firstPointId); } void pickLast() { pick(lastPointId()); }
    void pickMaxX()  { pick(indexsMax.x); }  void pickMinX() { pick(indexsMin.x); }
	void pickMaxY()  { pick(indexsMax.y); }  void pickMinY() { pick(indexsMin.y); }
	void pickMaxZ()  { pick(indexsMax.z); }  void pickMinZ() { pick(indexsMin.z); }
	void pickMaxF()  { pick(indexsMax.f); }  void pickMinF() { pick(indexsMin.f); }

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

	/// @brief ���һ���������
    const vtkIdType lastPointId() { return m_points->GetNumberOfPoints() - 1; }
	/// @brief ��һ���������
    const vtkIdType firstPointId = 0;
	/// @brief X��Y��Z������ֵ�ֱ����ĵ����������
    ValueRangeIndexes indexsMax;
	/// @brief X��Y��Z������ֵ�ֲ���С�ĵ����������
	ValueRangeIndexes indexsMin;

    vtkIdType m_pickedPointId = NO_POINT_PICKED;

    // ���̰����¼�
    virtual void keyPressEvent(QKeyEvent* event)   override;
    // ����̧���¼�
    virtual void keyReleaseEvent(QKeyEvent* event) override;

signals:
    /// @brief ʰȡ���ź�
    void picked(vtkIdType pointId);
};
 

