#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

    /// @brief ���ļ�
    void openFile();
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
    void dropEvent(QDropEvent* ev)           override;

    /// @brief ��ǰ�Ƿ������Ч����
    bool hasData();

    /// @brief ���ļ�
    /// @param fileName �ļ�·��
    void readFile(const QString& fileName);

    /// @brief ��ȡcsv�ļ�
    /// @param fileName �ļ���
	/// @param hasHead ��һ���Ƿ�Ϊ��ͷ
    void readCsv(const QString& fileName, bool hasHead = true);
	/// @brief ��ȡcnc�ļ�
	/// @param fileName �ļ���
    void readCnc(const QString& fileName);

    /// @brief ʰȡ�����UI�����¼�
    /// @param id �������id
    void pickedEvent(vtkIdType id);

	/// @brief ��ȡ��ɺ��UI�����¼�
    void readEvent();

private:
    Ui::MainWindowClass* ui;

    /// @brief ribbon�˵��ؼ�
    Ribbon* ri;

    /// @brief ��ʾ������
    Eigen::MatrixXf m_visualMat;
    /// @brief ��ȡ������������
    Eigen::MatrixXf m_mat;
	/// @brief ���ݱ�ͷ
    QStringList     m_head;

	/// @brief ��ȡ��
    Reader* m_reader = new Reader();
};
