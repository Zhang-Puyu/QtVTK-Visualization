#include "Reader.h"

#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qregexp.h"
#include "qregularexpression.h"

using namespace Eigen;

#pragma execution_character_set("utf-8")

const int x = 0, y = 1, z = 2, u = 3, v = 4, w = 5, s = 6, f = 7;

/// @brief 两个刀位点是否重合
static bool isCoincident(const VectorXf& p1, const VectorXf& p2)
{
	return (p1 - p2).segment(0, 3).norm() < 1e-6;
}
/// @brief 两个刀位点刀轴是否平行
static bool isParalleled(const VectorXf& p1, const VectorXf& p2)
{
	return (p1.segment(3, 3) - p2.segment(3, 3)).norm() < 1e-6;
}

/// @brief 匹配正浮点数
const QRegularExpression g_PostiveFloatExp = QRegularExpression("((\\b\\d+\\.\\d+|\\b\\d+))");
/// @brief 匹配浮点数
const QRegularExpression g_FloatExp = QRegularExpression("-?\\b\\d+(\\.\\d+)?\\b");
/// @brief 匹配结果
QRegularExpressionMatch g_Match;

/// @brief 快速进给标记
int   g_RapidFlag = 0;
/// @brief 快速进给速度
float g_RapidFeed = 6000;
/// @brief 快速进给之前的进给速度
float g_CuttingFeedBefore = 0.0f;

/// @brief 解析一行APT代码
/// @param line APT代码
/// @param point 解析得到的刀位点
static void paraseAptLine(const QString& line, VectorXf& point)
{
	if (line.trimmed().startsWith("GOTO/"))
	{
		auto i = g_FloatExp.globalMatch(line);
		if (i.hasNext()) {
			g_Match = i.next();
			point(x) = g_Match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			g_Match = i.next();
			point(y) = g_Match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			g_Match = i.next();
			point(z) = g_Match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			g_Match = i.next();
			point(u) = g_Match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			g_Match = i.next();
			point(v) = g_Match.captured(0).toFloat();
		}
		if (i.hasNext()) {
			g_Match = i.next();
			point(w) = g_Match.captured(0).toFloat();
		}

		if (g_RapidFlag == 2) {
			point(f) = g_RapidFeed;
			--g_RapidFlag;
		}
		else if (g_RapidFlag == 1) {
			point(f) = g_CuttingFeedBefore;
			--g_RapidFlag;
		}
	}
	if (line.trimmed().startsWith("FEDRAT/"))
	{
		g_Match = g_PostiveFloatExp.match(line);
		if (g_Match.hasMatch())
			point(f) = g_Match.captured(0).toFloat();

	}
	if (line.trimmed().startsWith("SPINDL/"))
	{
		g_Match = g_PostiveFloatExp.match(line);
		if (g_Match.hasMatch())
			point(s) = g_Match.captured(0).toFloat();

	}
	if (line.trimmed().startsWith("RAPID/"))
	{
		g_RapidFlag = 2;
		g_CuttingFeedBefore = point(f);
	}

}

/// @brief 匹配G代码
const QRegExp g_GCodeRegExp = QRegExp("(([A-Z])\\s*(-?\\d+(\\.\\d+)?|\\.\\d+))");

/// @brief 解析一行G代码
/// @param line G代码
/// @param point 解析得到的刀位点
static void paraseGCodeLine(const QString& line, VectorXf& point)
{
	int pos = 0;
	qDebug() << line;
	while ((pos = g_GCodeRegExp.indexIn(line, pos)) != -1)
	{
		QString capture = g_GCodeRegExp.cap(1);
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
			point(f) = g_RapidFeed;

		pos += g_GCodeRegExp.matchedLength(); // 移动到下一个匹配this
	}
}

Eigen::MatrixXf Reader::readCnc(const QString& fileName)
{
	QStringList head;
	return readCnc(fileName, head);
}

Eigen::MatrixXf Reader::readCnc(const QString& fileName, QStringList& head)
{
	// 打开文件
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "打开文件失败：" << fileName;
		return MatrixXf();
	}

	head.clear();
	head << "X" << "Y" << "Z" << "A" << "B" << "C" << "Rev" << "Feed";

	MatrixXf mat(1, head.length()); mat.setZero();
	VectorXf lastPoint(head.length()); lastPoint.setZero();
	VectorXf point;

	// 逐行读取文件
	QTextStream in(&file);

	if (fileName.toLower().endsWith(".cls"))
	{
		while (!in.atEnd())
		{
			point = lastPoint;
			paraseAptLine(in.readLine(), point);
			if (!isCoincident(lastPoint, point) && !isParalleled(lastPoint, point))
			{
				mat.conservativeResize(mat.rows() + 1, mat.cols());
				mat.row(mat.rows() - 1) = point;
				lastPoint = point;
			}
		}
		return mat.block(1, 0, mat.rows() - 1, mat.cols());
	}
	if (fileName.toLower().endsWith(".nc"))
	{
		while (!in.atEnd())
		{
			point = lastPoint;
			paraseGCodeLine(in.readLine(), point);
			if (!isCoincident(lastPoint, point) && !isParalleled(lastPoint, point))
			{
				mat.conservativeResize(mat.rows() + 1, mat.cols());
				mat.row(mat.rows() - 1) = point;
				lastPoint = point;
			}
		}
		return mat.block(1, 0, mat.rows() - 1, mat.cols());
	}
	return mat;
}
