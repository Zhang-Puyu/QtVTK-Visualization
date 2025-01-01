#include "Reader.h"

#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"

using namespace Eigen;

#pragma execution_character_set("utf-8")

MatrixXf Reader::readCsv(const QString& fileName)
{
	// ���ļ�
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "���ļ�ʧ�ܣ�" << fileName;
		return MatrixXf();
	}
	// ��ȡ�ļ�
	QStringList lines = QTextStream(&file).readAll().trimmed().split("\n");

	// ��ʼ������
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
	// ���ļ�
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "���ļ�ʧ�ܣ�" << fileName;
		return MatrixXf();
	}

	// ��ȡ�ļ�
	QStringList lines = QTextStream(&file).readAll().trimmed().split("\n");

	// ��ȡ��ͷ
	head = lines.first().trimmed().split(",");
	lines.pop_front();

	// ��ʼ������
	MatrixXf mat(lines.count(), head.count());

	for (int row = 0; row < lines.count(); ++row)
	{
		QStringList cells = lines[row].trimmed().split(",");
		for (int col = 0; col < cells.count(); ++col)
			mat(row, col) = cells[col].toFloat();
	}

	file.close();

	qDebug() << "��ȡ�ļ�: " << fileName;
	qDebug() << "����: " << mat.rows();;
	qDebug() << "����: " << mat.cols();

	return mat;
}