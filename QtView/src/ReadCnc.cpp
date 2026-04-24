#include "Reader.h"
#include "MatrixExtention.hpp"

#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qregexp.h"
#include "qregularexpression.h"

using namespace MatrixExtension;
using namespace Eigen;

#pragma execution_character_set("utf-8")

const int x = 0, y = 1, z = 2, u = 3, v = 4, w = 5, s = 6, f = 7;

void Reader::paraseAptLine(const QString& line, VectorXf& point)
{
	if (line.trimmed().startsWith("GOTO/"))
	{
		auto i = expFloat.globalMatch(line);
		if (i.hasNext()) {
			m_match = i.next();
			point(x) = m_match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			m_match = i.next();
			point(y) = m_match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			m_match = i.next();
			point(z) = m_match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			m_match = i.next();
			point(u) = m_match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			m_match = i.next();
			point(v) = m_match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			m_match = i.next();
			point(w) = m_match.captured(0).toFloat();
		}

		if (m_rapidFlag == 2) {
			point(f) = m_rapidFeed;
			--m_rapidFlag;
		}
		else if (m_rapidFlag == 1) {
			point(f) = m_cuttingFeed;
			--m_rapidFlag;
		}
	}
	if (line.trimmed().startsWith("FEDRAT/"))
	{
		m_match = expPostiveFloat.match(line);
		if (m_match.hasMatch())
			point(f) = m_match.captured(0).toFloat();

	}
	if (line.trimmed().startsWith("SPINDL/"))
	{
		m_match = expPostiveFloat.match(line);
		if (m_match.hasMatch())
			point(s) = m_match.captured(0).toFloat();

	}
	if (line.trimmed().startsWith("RAPID/"))
	{
		m_rapidFlag = 2;
		m_cuttingFeed = point(f);
	}

}

void Reader::paraseGCodeLine(const QString& line, VectorXf& point)
{
	int pos = 0;
	while ((pos = expGCodeReg.indexIn(line, pos)) != -1)
	{
		QString capture = expGCodeReg.cap(1);
		QString letter = capture.at(0);
		QString number = capture.mid(1);

		     if (letter == "X") point(x) = number.toFloat();
		else if (letter == "Y") point(y) = number.toFloat();
		else if (letter == "Z") point(z) = number.toFloat();
		else if (letter == "A") point(u) = number.toFloat();
		else if (letter == "B") point(v) = number.toFloat();
		else if (letter == "C") point(w) = number.toFloat();
		else if (letter == "S") point(s) = number.toFloat();
		else if (letter == "F") point(f) = number.toFloat();

		else if (letter.toLower() == "g" && number.toInt() == 0)
			point(f) = m_rapidFeed;

		pos += expGCodeReg.matchedLength(); // 移动到下一个匹配this
	}
}

void Reader::readCnc(Eigen::MatrixXf& mat, const QString& fileName, const QString& codec)
{
	QStringList _;
	readCnc(mat, fileName, _, codec);
}

void Reader::readCnc(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head, const QString& codec)
{
	// 打开文件
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "打开文件失败：" << fileName;
		return;
	}

	head.clear();
	head << "X" << "Y" << "Z" << "A" << "B" << "C" << "Rev" << "Feed";

	// MatrixXf mat(1, head.length()); mat.setZero();
	mat.resize(1, head.length()); mat.setZero();
	VectorXf lastPoint(head.length()); lastPoint.setZero();
	VectorXf point;

	//// 逐行读取文件
	//QTextStream in(&file);
	//if (fileName.toLower().endsWith(".cls"))
	//{
	//	while (!in.atEnd())
	//	{
	//		point = lastPoint;
	//		paraseAptLine(in.readLine(), point);
	//		if (!isCoincident(lastPoint, point) && !isParalleled(lastPoint, point))
	//		{
	//			mat.conservativeResize(mat.rows() + 1, mat.cols());
	//			mat.row(mat.rows() - 1) = point;
	//			lastPoint = point;
	//		}
	//	}
	//	return mat.block(1, 0, mat.rows() - 1, mat.cols());
	//}
	//if (fileName.toLower().endsWith(".nc"))
	//{
	//	while (!in.atEnd())
	//	{
	//		point = lastPoint;
	//		paraseGCodeLine(in.readLine(), point);
	//		if (!isCoincident(lastPoint, point) && !isParalleled(lastPoint, point))
	//		{
	//			mat.conservativeResize(mat.rows() + 1, mat.cols());
	//			mat.row(mat.rows() - 1) = point;
	//			lastPoint = point;
	//		}
	//	}
	//	return mat.block(1, 0, mat.rows() - 1, mat.cols());
	//}
	//return mat;

	// 一次性读取文件
	// 读取文件
	QTextStream in(&file);
	// 指定文件编码格式
	in.setCodec(codec.toLocal8Bit());
	QStringList lines = in.readAll().trimmed().split("\n");

	int rowCount = lines.size();
	int row = 0;

	void (Reader::*paraseMethodPtr)(const QString& line, VectorXf& point) = nullptr;

	if (fileName.toLower().endsWith(".cls"))
		paraseMethodPtr = &Reader::paraseAptLine;
	if (fileName.toLower().endsWith(".nc"))
		paraseMethodPtr = &Reader::paraseGCodeLine;
		
	emit readStarted(rowCount);

	while (row < rowCount)
	{
		point = lastPoint;
		(this->*paraseMethodPtr)(lines[row], point);
		if (!isCoincident(lastPoint, point) && !isParalleled(lastPoint, point))
		{
			mat.conservativeResize(mat.rows() + 1, mat.cols());
			mat.row(mat.rows() - 1) = point;
			lastPoint = point;
		}
		emit readProgress(row++);
	}

	emit readFinished(rowCount);

	removeMatrixRow(mat, 0);
}