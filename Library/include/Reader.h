#pragma once

#include "qstring"
#include "qstringlist.h"
#include "qregularexpression.h"
#include "qdebug.h"

#include <Eigen/Dense>

namespace Reader
{
	/// @brief qDebug输出矩阵
	static void qDebugEigenMatrix(const Eigen::MatrixXf& mat)
	{
		for (int row = 0; row < mat.rows(); ++row)
		{
			QString line;
			for (int col = 0; col < mat.cols(); ++col)
				line += QString::number(mat(row, col)) + ", ";
			qDebug() << line;
		}
	}

	/// @brief 判断字符串是否包含中文字符
	static bool hasChinese(const QString& str)
	{
		QRegularExpression regex("[\\u4e00-\\u9fa5]+");
		return regex.match(str).hasMatch();
	}

    /// @brief 读取csv文件
    /// @param fileName 文件名
    /// @return 读取的矩阵数据
    Eigen::MatrixXf readCsv(const QString& fileName);
    /// @brief 读取csv文件
    /// @param fileName 文件名
	/// @param head csv第一行作为表头
    /// @return 读取的矩阵数据
    Eigen::MatrixXf readCsv(const QString& fileName, QStringList& head);

	/// @brief 读取数控刀轨文件
	/// @param fileName 文件名
	/// @return 刀位点矩阵
	Eigen::MatrixXf readCnc(const QString& fileName);
	/// @brief 读取数控刀轨文件
	/// @param fileName 文件名
	/// @param head 矩阵文件表头
	/// @return 刀位点矩阵
	Eigen::MatrixXf readCnc(const QString& fileName, QStringList& head);
}

