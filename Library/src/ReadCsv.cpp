#include "Reader.h"

#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"

using namespace Eigen;

#pragma execution_character_set("utf-8")

MatrixXf Reader::readCsv(const QString& fileName)
{
	// 打开文件
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "打开文件失败：" << fileName;
		return MatrixXf();
	}
	// 读取文件
	QStringList lines = QTextStream(&file).readAll().trimmed().split("\n");

	// 初始化矩阵
	MatrixXf mat(lines.count(), lines.first().trimmed().split(",").count());

	for (int row = 0; row < lines.count() ; ++row)
	{
		QStringList cells = lines[row].trimmed().split(",");
		for (int col = 0; col < cells.count(); ++col)
			mat(row, col) = cells[col].toFloat();
	}

	file.close();

    return mat;
}

MatrixXf Reader::readCsv(const QString& fileName, QStringList& head)
{
	// 打开文件
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "打开文件失败：" << fileName;
		return MatrixXf();
	}

	// 读取文件
	QStringList lines = QTextStream(&file).readAll().trimmed().split("\n");

	// 读取表头
	head = lines.first().trimmed().split(",");
	lines.pop_front();

	// 初始化矩阵
	MatrixXf mat(lines.count(), head.count());

	for (int row = 0; row < lines.count(); ++row)
	{
		QStringList cells = lines[row].trimmed().split(",");
		for (int col = 0; col < cells.count(); ++col)
			mat(row, col) = cells[col].toFloat();
	}

	file.close();

	qDebug() << "读取文件: " << fileName;
	qDebug() << "行数: " << mat.rows();;
	qDebug() << "列数: " << mat.cols();

	return mat;
}