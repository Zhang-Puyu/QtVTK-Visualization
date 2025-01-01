#include "InteractorStyle.h"

#include <vtkObjectFactory.h>  
#include <vtkPointPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>

#ifndef VTKSTANDARDNEWMACRO_POINTPICKERINTERACTORSTYLE
#define VTKSTANDARDNEWMACRO_POINTPICKERINTERACTORSTYLE
vtkStandardNewMacro(PointPickerInteractorStyle);
#endif 

PointPickerInteractorStyle::PointPickerInteractorStyle() 
    : vtkInteractorStyleTrackballCamera()
{
	// 设置拾取容差
	m_picker->SetTolerance(0.005);
}

void PointPickerInteractorStyle::OnLeftButtonDown()
{
    int* clickPos = this->GetInteractor()->GetEventPosition();
    // 获取拾取信息
    m_picker->Pick(clickPos[0], clickPos[1], 0, 
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());

    // 检查是否拾取到了点
    //if (m_picker->GetPointId() != -1)
    //{
    //    double pickedPos[3];
    //    m_picker->GetPickPosition(pickedPos);
    //}

    m_signalSender->sendPickedPointId(m_picker->GetPointId());
    __super::OnLeftButtonDown();
}

void PointPickerInteractorStyle::SetPickerTolerance(double tol)
{
    if (tol > 0) m_picker->SetTolerance(tol);
}

double PointPickerInteractorStyle::GetPickerTolerance()
{
	return m_picker->GetTolerance();
}

PointPickerSignalSender::PointPickerSignalSender(QObject* parent)
    : QObject(parent)
{

}
