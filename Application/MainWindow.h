#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

    /// @brief 打开文件
    void open();
	/// @brief 刷新视图
    void refresh();
    /// @brief 清空视图
    void clear();;
    /// @brief 拾取点
	/// @param id 点的索引id
    void pick(vtkIdType id);

	void pickMax();
	void pickMin();

	/// @brief 开始动态显示
	void startLiveView();
	/// @brief 停止动态显示
	void stopLiveView();

protected:
    /// @brief 文件拖入窗口或控件时响应函数
    void dragEnterEvent(QDragEnterEvent* ev) override;
	/// @brief 文件拖入操作完成后响应函数
    void dropEvent(QDropEvent* ev)           override;
    /// @brief 是否已读取了数据
    bool hasData();

    /// @brief 读取文件
    /// @param fileName 文件路径
    void read(const QString& fileName);

    void readCsv(const QString& fileName, bool hasHead = true);
    void readCnc(const QString& fileName);

    void readFinishedHandler();

private:
    Ui::MainWindowClass* ui;

    /// @brief ribbon菜单控件
    Ribbon* ri;

    /// @brief 显示的数据
    Eigen::MatrixXf m_block;
    /// @brief 读取到的完整数据
    Eigen::MatrixXf m_mat;
	/// @brief 数据表头
    QStringList m_head;

    Reader* m_reader = new Reader();
};
