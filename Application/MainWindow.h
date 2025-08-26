#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "ReadAPT.h"
#include "ReadGCode.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

    /// @brief ���ļ�
    void open();
	/// @brief ˢ����ͼ
    void refreshView();
    /// @brief �����ͼ
    void clear();

	/// @brief ������ʾ�ĵ�
	void exportVisualPoints();

	/// @brief ��ʼ��̬��ʾ
	void startLiveView();
	/// @brief ֹͣ��̬��ʾ
	void stopLiveView();

protected:
    /// @brief �ļ����봰�ڻ�ؼ�ʱ��Ӧ����
    void dragEnterEvent(QDragEnterEvent* ev) override;
	/// @brief �ļ����������ɺ���Ӧ����
    void dropEvent(QDropEvent* ev) override;

    /// @brief ��ǰ�Ƿ������Ч����
    bool hasData();

    /// @brief ���ļ�
    /// @param fileName �ļ�·��
    void read(const QString& fileName);

    /// @brief ��ȡcsv�ļ�
    /// @param fileName �ļ���
	/// @param hasHead ��һ���Ƿ�Ϊ��ͷ
    void readCSV(const QString& fileName, bool hasHead = true);
	/// @brief ��ȡ���س���
	/// @param fileName �ļ���
    void readNC(const QString& fileName);

    /// @brief ��ά����ʰȡ�㴦���¼�
    /// @param id �������id
    void cloudPointPickedHandler(vtkIdType id);

	/// @brief ��άͼ��ʰȡ�㴦���¼�
    /// @param point ������
    void chartPointPickedHandler(const QPointF& point);

	/// @brief ��ȡ��ɺ��UI�����¼�
    void readFinishHandler();

    /// @brief ��̬��ʾ��ʼUI�����¼�
    void liveViewStartedHandler();
    /// @brief ��̬��ʾ����UI�����¼�
    void liveViewStoppedHandler();

private:
    Ui::MainWindowClass* ui;

    /// @brief ribbon�˵��ؼ�
    Ribbon* ri;

    bool isVisualChart() const {
        return ui->stackedWidget->currentWidget() == ui->visualChartWidget;
	}
    bool isVisualCloud() const {
        return ui->stackedWidget->currentWidget() == ui->visualCloudWidget;
    }

    /// @brief ��ʾ������
    Eigen::MatrixXf m_visualMat;
    /// @brief ��ȡ������������
    Eigen::MatrixXf m_oriMat;
	/// @brief ���ݱ�ͷ
    QStringList m_head;

	/// @brief ��ȡ��
    NC::Toolpath::AbstractrReader* m_ncReader;
};
