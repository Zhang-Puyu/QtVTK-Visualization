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

    /// @brief 打开文件
    void open();
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
    void dropEvent(QDropEvent* ev) override;

    /// @brief 当前是否存在有效数据
    bool hasData();

    /// @brief 打开文件
    /// @param fileName 文件路径
    void read(const QString& fileName);

    /// @brief 读取csv文件
    /// @param fileName 文件名
	/// @param hasHead 第一行是否为表头
    void readCSV(const QString& fileName, bool hasHead = true);
	/// @brief 读取数控程序
	/// @param fileName 文件名
    void readNC(const QString& fileName);

    /// @brief 三维点云拾取点处理事件
    /// @param id 点的索引id
    void cloudPointPickedHandler(vtkIdType id);

	/// @brief 二维图像拾取点处理事件
    /// @param point 点坐标
    void chartPointPickedHandler(const QPointF& point);

	/// @brief 读取完成后的UI处理事件
    void readFinishHandler();

    /// @brief 动态显示开始UI处理事件
    void liveViewStartedHandler();
    /// @brief 动态显示结束UI处理事件
    void liveViewStoppedHandler();

private:
    Ui::MainWindowClass* ui;

    /// @brief ribbon菜单控件
    Ribbon* ri;

    bool isVisualChart() const {
        return ui->stackedWidget->currentWidget() == ui->visualChartWidget;
	}
    bool isVisualCloud() const {
        return ui->stackedWidget->currentWidget() == ui->visualCloudWidget;
    }

    /// @brief 显示的数据
    Eigen::MatrixXf m_visualMat;
    /// @brief 读取到的完整数据
    Eigen::MatrixXf m_oriMat;
	/// @brief 数据表头
    QStringList m_head;

	/// @brief 读取器
    NC::Toolpath::AbstractrReader* m_ncReader;
};
