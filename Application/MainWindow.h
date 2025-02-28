#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

    /// @brief 打开文件
    void openFile();
	/// @brief 刷新视图
    void refreshView();
    /// @brief 清空视图
    void clear();

	/// @brief 导出显示的点
	void exportVisualPoints();

	/// @brief 开始动态显示
	void startLiveView();
	/// @brief 停止动态显示
	void stopLiveView();

protected:
    /// @brief 文件拖入窗口或控件时响应函数
    void dragEnterEvent(QDragEnterEvent* ev) override;
	/// @brief 文件拖入操作完成后响应函数
    void dropEvent(QDropEvent* ev)           override;

    /// @brief 当前是否存在有效数据
    bool hasData();

    /// @brief 打开文件
    /// @param fileName 文件路径
    void readFile(const QString& fileName);

    /// @brief 读取csv文件
    /// @param fileName 文件名
	/// @param hasHead 第一行是否为表头
    void readCsv(const QString& fileName, bool hasHead = true);
	/// @brief 读取cnc文件
	/// @param fileName 文件名
    void readCnc(const QString& fileName);

    /// @brief 拾取到点后UI处理事件
    /// @param id 点的索引id
    void pickedEvent(vtkIdType id);

	/// @brief 读取完成后的UI处理事件
    void readEvent();

private:
    Ui::MainWindowClass* ui;

    /// @brief ribbon菜单控件
    Ribbon* ri;

    /// @brief 显示的数据
    Eigen::MatrixXf m_visualMat;
    /// @brief 读取到的完整数据
    Eigen::MatrixXf m_mat;
	/// @brief 数据表头
    QStringList     m_head;

	/// @brief 读取器
    Reader* m_reader = new Reader();
};
