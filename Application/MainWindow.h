#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include <Eigen/Dense>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

    /// @brief ���ļ�
    void open();
	/// @brief ˢ����ͼ
    void refresh();
    /// @brief �����ͼ
    void clear();;
    /// @brief ʰȡ��
	/// @param id �������id
    void pick(vtkIdType id);

	/// @brief ��ʼ��̬��ʾ
	void startLiveView();
	/// @brief ֹͣ��̬��ʾ
	void stopLiveView();

protected:
    /// @brief �ļ����봰�ڻ�ؼ�ʱ��Ӧ����
    void dragEnterEvent(QDragEnterEvent* ev) override;
	/// @brief �ļ����������ɺ���Ӧ����
    void dropEvent(QDropEvent* ev)           override;
    /// @brief �Ƿ��Ѷ�ȡ������
    bool hasData();

    /// @brief ��ȡ�ļ�
    /// @param fileName �ļ�·��
    void read(const QString& fileName);

    void readCsv(const QString& fileName, bool hasHead = true);
    void readStl(const QString& fileName);
    void readCnc(const QString& fileName);

private:
    Ui::MainWindowClass* ui;

    /// @brief ribbon�˵��ؼ�
    Ribbon*              ri;
    /// @brief ��ʾ������
    Eigen::MatrixXf      visualMat;
    /// @brief ��ȡ������������
    Eigen::MatrixXf      mat;
};