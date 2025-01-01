#pragma once

#include <vtkAutoInit.h>
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

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPointPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>    
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

#include "qobject.h"

const static int NO_POINT_PICKED = -1;

class PointPickerSignalSender : public QObject
{
    Q_OBJECT

    friend class PointPickerInteractorStyle;

private:
    void sendPickedPointId(const vtkIdType& id) {
        emit sendPickedPointIdSignal(id);
    }

public:
    PointPickerSignalSender(QObject* parent = nullptr);

signals:
    void sendPickedPointIdSignal(const vtkIdType& id);
};

class PointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    PointPickerInteractorStyle();

    static PointPickerInteractorStyle* New();
    vtkTypeMacro(PointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown() override;
    void SetPickerTolerance(double tol);
    double GetPickerTolerance();
    const PointPickerSignalSender* GetSignalSender() const { return m_signalSender; }

private:
    vtkSmartPointer<vtkPointPicker> m_picker = vtkSmartPointer<vtkPointPicker>::New();
    PointPickerSignalSender* m_signalSender = new PointPickerSignalSender();
};


