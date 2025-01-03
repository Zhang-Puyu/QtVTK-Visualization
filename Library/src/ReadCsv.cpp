#include "Reader.h"

#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"

using namespace Eigen;

#pragma execution_character_set("utf-8")

void Reader::readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head, const bool skipFirstRow)
{
	// 打开文件
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "打开文件失败：" << fileName;
		return;
	}

	// 读取文件
	QStringList lines = QTextStream(&file).readAll().trimmed().split("\n");

	if (!skipFirstRow)
	{
		// 读取表头
		head = lines.first().trimmed().split(",");
		lines.pop_front();

		// 初始化矩阵
		// MatrixXf mat(lines.count(), head.count());
		mat.resize(lines.count(), head.count());
	}
	else
	{
		mat.resize(lines.count(), lines.first().trimmed().split(",").count());

		head.clear();
		for (int i = 0; i < mat.cols(); i++)
			head.push_back(QString("col ") + QString::number(i));
	}

	int rowCount = lines.size();
	int row = skipFirstRow ? 1 : 0;

	emit readStarted(rowCount);

	while (row < rowCount)
	{
		QStringList cells = lines[row].trimmed().split(",");
		for (int col = 0; col < cells.count(); ++col)
			mat(row, col) = cells[col].toFloat();
		emit readProgress(row++);
	}

	emit readFinished(rowCount);

	file.close();
}

void Reader::readCsv(Eigen::MatrixXf& mat, const QString& fileName)
{
	QStringList _;
	readCsv(mat, fileName, _, false);
}

void Reader::readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head)
{
	readCsv(mat, fileName, head, true);
}
