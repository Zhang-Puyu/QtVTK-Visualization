#include "Reader.h"

#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"

using namespace Eigen;

#pragma execution_character_set("utf-8")

void Reader::readCsv(Eigen::MatrixXf& mat, 
	const QString& fileName, const QString& codec, QStringList& head,
	const bool firstRowIsHead)
{
	// ���ļ�
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "���ļ�ʧ�ܣ�" << fileName;
		return;
	}

	// ��ȡ�ļ�
    QTextStream in(&file);
	// ָ���ļ������ʽ
    in.setCodec(codec.toLocal8Bit());
    QStringList lines = in.readAll().trimmed().split("\n");

	if (firstRowIsHead)
	{
		// ��ȡ��ͷ
		head = lines.first().trimmed().split(",");
		lines.pop_front();

		// ��ʼ������
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
	int row = 0;

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

void Reader::readCsv(Eigen::MatrixXf& mat, const QString& fileName, const QString& codec)
{
	QStringList _;
	readCsv(mat, fileName, codec, _, false);
}

void Reader::readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head, const QString& codec)
{
	readCsv(mat, fileName, codec, head, true);
}

void Reader::writeCsv(const Eigen::MatrixXf& mat, const QString& fileName, QStringList& head, const QString& codec)
{
	// ���ļ�
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "���ļ�ʧ�ܣ�" << fileName;
		return;
	}
	// ָ���ļ������ʽ
	QTextStream out(&file);
	out.setCodec(codec.toLocal8Bit());
	// д���ͷ
	for (int i = 0; i < head.length(); ++i)
	{
		out << head[i];
		if (i < head.length() - 1)
			out << ",";
	}
	out << "\n";
	// д������
	for (int row = 0; row < mat.rows(); ++row)
	{
		for (int col = 0; col < mat.cols(); ++col)
		{
			out << mat(row, col);
			if (col < mat.cols() - 1)
				out << ",";
		}
		out << "\n";
	}
	file.close();
}
