#pragma once

#include <Eigen/Dense>

#include "qstring.h"
#include "qdebug.h"

namespace MatrixExtension
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

	/// @brief 两个刀位点是否重合
	/// @param p1 (x, y, z, a, b, c)
	/// @param p2 (x, y, z, a, b, c)
	static bool isCoincident(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2)
	{
		return (p1 - p2).segment(0, 3).norm() < 1e-6;
	}

	/// @brief 两个刀位点刀轴是否平行
	/// @param p1 (x, y, z, a, b, c)
	/// @param p2 (x, y, z, a, b, c)
	static bool isParalleled(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2)
	{
		return (p1.segment(3, 3) - p2.segment(3, 3)).norm() < 1e-6;
	}

	/// @brief 矩阵删除一行
	/// @param matrix 矩阵
	/// @param rowToRemove 要删除的行号
	static void removeMatrixRow(Eigen::MatrixXf& matrix, unsigned int rowToRemove) {

		unsigned int numRows = matrix.rows() - 1;
		unsigned int numCols = matrix.cols();

		if (rowToRemove < numRows) {
			matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) =
				matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);
		}
		matrix.conservativeResize(numRows, numCols);
	}

	/// @brief 矩阵删除一列
	/// @param matrix 矩阵
	/// @param colToRemove 要删除的列号
	static void removeMatrixColumn(Eigen::MatrixXf& matrix, unsigned int colToRemove) {

		unsigned int numRows = matrix.rows();
		unsigned int numCols = matrix.cols() - 1;

		if (colToRemove < numCols) {
			matrix.block(0, colToRemove, numRows, numCols - colToRemove) =
				matrix.block(0, colToRemove + 1, numRows, numCols - colToRemove);
		}

		matrix.conservativeResize(numRows, numCols);
	}
}