#include "EigenTools.h"

#include "qfile.h"
#include "qtextstream.h"
#include "qregexp.h"
#include "qregularexpression.h"

/// @brief 判断字符串是否为表头（只包含数字、小数点、加号、减号、分隔符）
inline bool hasOnlyNumber(QString str, const QString& sep = ",")
{
	// 使用正则表达式检查是否只包含数字、小数点、加号、减号
	QRegularExpression regex("^[0-9\\+\\-\\.]*$");
	QRegularExpressionMatch match = regex.match(str.remove(sep));

	return !match.hasMatch();
}

void readCSV(Eigen::MatrixXf& matrix, const QString& fileName, 
	QStringList& head, const QString& separator, const QString& codec)
{
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			// 一次性读取文件
			// 文本流
			QTextStream stream(&file);
			stream.setCodec(codec.toLocal8Bit());
			QStringList lines = stream.readAll().trimmed().split("\n");

			// 解析表头
			head.clear();
			QString firstLine = lines.first();
			if (hasOnlyNumber(firstLine, separator)) {
				for (const QString& cell : firstLine.trimmed().split(separator))
					head.append(cell.trimmed());
				lines.removeFirst();
			}
			else {
				for (int i = 0; i < firstLine.split(separator).size(); i++)
					head.append(QString("col ") + QString::number(i));
			}

			int rows = lines.size();
			int cols = lines.first().split(separator).size();
			matrix.resize(rows, cols);

			for (int row = 0; row < rows; ++row) {
				QStringList values = lines.at(row).split(separator);
				for (int col = 0; col < cols; ++col)
					matrix(row, col) = values.at(col).toDouble();
			}
			file.close();
		}
		else
		{
			qDebug() << "Failed to open file:" << fileName;
		}
	}
}

void writeCSV(const Eigen::MatrixXf& matrix, const QString& fileName,
	const QString& head, const QString& separator, const QString& codec)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream.setCodec(codec.toLocal8Bit());

		if (!head.isEmpty())
			stream << head << "\n";

		for (int row = 0; row < matrix.rows(); ++row) {
			for (int col = 0; col < matrix.cols(); ++col) {
				stream << matrix(row, col);
				if (col < matrix.cols() - 1) {
					stream << separator;
				}
			}
			stream << "\n";
		}
		file.close();
	}
}

void removeColumn(Eigen::MatrixXf& matrix, unsigned int col)
{
	unsigned int numRows = matrix.rows();
	unsigned int numCols = matrix.cols() - 1;

	if (col < numCols) {
		matrix.block(0, col, numRows, numCols - col) =
			matrix.block(0, col + 1, numRows, numCols - col);
	}

	matrix.conservativeResize(numRows, numCols);
}

void removeRow(Eigen::MatrixXf& matrix, unsigned int row)
{
	if (row < 0 || row >= matrix.rows()) {
		// 文件打开失败时输出错误信息并返回
		qDebug() << "错误：矩阵行索引超出范围，无法删除第 " << row << " 行 ";
		return;
	}

	int rows = matrix.rows() - 1;
	int cols = matrix.cols();

	if (row < rows) {
		matrix.block(row, 0, rows - row, cols) = matrix.block(row + 1, 0, rows - row, cols);
	}
	matrix.conservativeResize(rows, cols);
}

void addColumn(Eigen::MatrixXf& matrix)
{
	// 获取矩阵行数
	int rows = matrix.rows();

	// 添加新列并赋值行号
	matrix.conservativeResize(rows, matrix.cols() + 1);
	// 给新列赋值行号（0,1,2,...）
	for (int i = 0; i < rows; ++i)
		matrix(i, matrix.cols() - 1) = i;
}

void addColumn(Eigen::MatrixXf& matrix, double value)
{
	// 调整矩阵大小，增加一列
	matrix.conservativeResize(matrix.rows(), matrix.cols() + 1);
	// 为新列设置常量值
	matrix.col(matrix.cols() - 1).setConstant(value);
}

void propRange(const Eigen::VectorXf& vector, double& propMin, double& propMax, double prop)
{
	double oriMin = vector.minCoeff();
	double oriMax = vector.maxCoeff();
	propMax = oriMax + (oriMax - oriMin) * (prop - 1);
	propMin = oriMin - (oriMax - oriMin) * (prop - 1);
}