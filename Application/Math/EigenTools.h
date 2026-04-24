#pragma once

#include <Eigen/Dense>

#include "qstring.h"
#include "qstringlist.h"
#include "qdebug.h"

/// @brief qDebug输出矩阵
inline void qDebugEigenMatrix(const Eigen::MatrixXf& mat)
{
	for (int row = 0; row < mat.rows(); ++row)
	{
		QString line;
		for (int col = 0; col < mat.cols(); ++col)
			line += QString::number(mat(row, col)) + ", ";
		qDebug() << line;
	}
}
/// @brief 读取csv文件到矩阵
/// @param matrix 矩阵
/// @param fileName 文件路径
/// @param skipFirstRow 是否跳过第一行（表头）
/// @param separator 分隔符
/// @param codec 编码格式
void readCSV(Eigen::MatrixXf& matrix, const QString& fileName,
	QStringList& head,
	const QString& separator = ",",
	const QString& codec = "UTF-8");
	

/// @brief 将矩阵写入csv文件
/// @param matrix 矩阵
/// @param fileName 文件路径
/// @param head 表头
/// @param separator 分隔符
/// @param codec 编码格式
void writeCSV(const Eigen::MatrixXf& matrix, const QString& fileName,
	const QString& head = "",
	const QString& separator = ",",
	const QString& codec = "UTF-8");

/// @brief 矩阵删除一列
void removeColumn(Eigen::MatrixXf& matrix, unsigned int col);

/// @brief 矩阵删除一行
void removeRow(Eigen::MatrixXf& matrix, unsigned int row);

/// @brief Eigen矩阵添加一列，各元素的值为行号索引
void addColumn(Eigen::MatrixXf& matrix);

/// @brief Eigen矩阵添加一列，各元素的值为给定值
void addColumn(Eigen::MatrixXf& matrix, double value);

/// @brief 按比例缩放向量的上下界
void propRange(const Eigen::VectorXf& vector, double& propMin, double& propMax, double prop);
	