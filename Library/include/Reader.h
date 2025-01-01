#pragma once

#include "qstring"
#include "qstringlist.h"
#include "qregularexpression.h"
#include "qdebug.h"

#include <Eigen/Dense>

namespace Reader
{
	/// @brief qDebug�������
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

	/// @brief �ж��ַ����Ƿ���������ַ�
	static bool hasChinese(const QString& str)
	{
		QRegularExpression regex("[\\u4e00-\\u9fa5]+");
		return regex.match(str).hasMatch();
	}

    /// @brief ��ȡcsv�ļ�
    /// @param fileName �ļ���
    /// @return ��ȡ�ľ�������
    Eigen::MatrixXf readCsv(const QString& fileName);
    /// @brief ��ȡcsv�ļ�
    /// @param fileName �ļ���
	/// @param head csv��һ����Ϊ��ͷ
    /// @return ��ȡ�ľ�������
    Eigen::MatrixXf readCsv(const QString& fileName, QStringList& head);

	/// @brief ��ȡ���ص����ļ�
	/// @param fileName �ļ���
	/// @return ��λ�����
	Eigen::MatrixXf readCnc(const QString& fileName);
	/// @brief ��ȡ���ص����ļ�
	/// @param fileName �ļ���
	/// @param head �����ļ���ͷ
	/// @return ��λ�����
	Eigen::MatrixXf readCnc(const QString& fileName, QStringList& head);
}

