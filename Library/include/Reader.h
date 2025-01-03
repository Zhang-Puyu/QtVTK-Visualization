#pragma once

#include "qstring.h"
#include "qstringlist.h"
#include "qregularexpression.h"
#include "qprogressdialog.h"

#include <Eigen/Dense>

class Reader : public QObject
{
	Q_OBJECT;

public:
	Reader(QObject* parent = nullptr)
		: QObject(parent) {
	};

	/// @brief 判断字符串是否包含中文字符
	static bool hasChinese(const QString& str)
	{
		QRegularExpression regex("[\\u4e00-\\u9fa5]+");
		return regex.match(str).hasMatch();
	}

	/// @brief 为数据读取过程显示进度窗口
	/// @param parent 进度对话框的父窗口
	void addReadProgressDialog(QWidget* parent = nullptr)
	{
		QProgressDialog* dialog = new QProgressDialog(parent);
		// 对话框正文
		dialog->setLabelText(tr("正在读取文件..."));
		// 对话框标题
		dialog->setWindowTitle(tr("读取进度"));
		// 模态对话框
		dialog->setWindowModality(Qt::WindowModal);
		// 取消按钮文字
		dialog->setCancelButtonText(tr("隐藏"));
		//如果进度条运行的时间小于5，进度条就不会显示，默认是4000ms
		dialog->setMinimumDuration(0);

		// 读取开始时打开进度对话框
		connect(this, &Reader::readStarted, dialog, &QProgressDialog::show);
		// 读取开始时设置进度范围
		connect(this, &Reader::readStarted, dialog, &QProgressDialog::setMaximum);
		// 读取进度时更新进度对话框
		connect(this, &Reader::readProgress, dialog, &QProgressDialog::setValue);
		// 读取结束时关闭进度对话框
		connect(this, &Reader::readFinished, dialog, &QProgressDialog::reset);

		// 当reset()时，对话框自动关闭
		dialog->setAutoClose(true);
		// 设置最小值
		dialog->setMinimum(0);
		// 设置当前值
		dialog->setValue(0);
	}

	/// @brief 读取csv文件
	/// @param mat 读取的矩阵数据
	/// @param fileName 文件名
	void readCsv(Eigen::MatrixXf& mat, const QString& fileName);

	/// @brief 读取csv文件
	/// @param mat 读取的矩阵数据
	/// @param fileName 文件名
	/// @param head csv第一行作为表头
	void readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head);

	/// @brief 读取数控刀轨文件
	/// @param mat 刀位点矩阵
	/// @param fileName 文件名
	void readCnc(Eigen::MatrixXf& mat, const QString& fileName);

	/// @brief 读取数控刀轨文件
	/// @param mat 刀位点矩阵
	/// @param fileName 文件名
	/// @param head 刀位点矩阵
	void readCnc(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head);

private:
	/// @brief 匹配正浮点数
	const QRegularExpression expPostiveFloat = QRegularExpression("((\\b\\d+\\.\\d+|\\b\\d+))");
	/// @brief 匹配浮点数
	const QRegularExpression expFloat = QRegularExpression("-?\\b\\d+(\\.\\d+)?\\b");
	/// @brief 匹配G代码
	const QRegExp expGCodeReg = QRegExp("(([A-Z])\\s*(-?\\d+(\\.\\d+)?|\\.\\d+))");

	/// @brief 匹配结果
	QRegularExpressionMatch m_match;

	/// @brief 快速进给标记
	int   m_rapidFlag = 0;
	/// @brief 快速进给速度
	float m_rapidFeed = 6000;
	/// @brief 快速进给之前的切削进给速度
	float m_cuttingFeed = 0.0f;

	/// @brief 解析一行APT代码
	/// @param line APT代码
	/// @param point 解析得到的刀位点
	void paraseAptLine(const QString& line, Eigen::VectorXf& point);

	/// @brief 解析一行G代码
	/// @param line G代码
	/// @param point 解析得到的刀位点
	void paraseGCodeLine(const QString& line, Eigen::VectorXf& point);

	/// @brief 读取csv文件
	/// @param mat 读取的矩阵数据
	/// @param fileName 文件名
	/// @param head 矩阵表头，当skipFirstRow = true时才会生效修改
	/// @param skipFirstRow 读取文件时是否跳过第一行
	void readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head, const bool skipFirstRow);

signals:
	/// @brief 读取文件开始
	/// @param rowCount 文件总行数
	void readStarted(unsigned int rowCount);
	/// @brief 读取文件完成
	/// @param rowCount 文件总行数
	void readFinished(unsigned int rowCount);

	/// @brief 文件读取进度
	/// @param row 当前行数
	void readProgress(unsigned int row);
};

