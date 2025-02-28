#pragma once

#include "qobject.h"
#include "qstringlist.h"

#include <Eigen/Dense>

class DataSingleton : public QObject
{
	Q_OBJECT

public:
	/// @brief 禁止拷贝操作
	DataSingleton(const DataSingleton&) = delete;
	/// @brief 禁止赋值操作
	DataSingleton& operator=(const DataSingleton&) = delete;

	static const Eigen::MatrixXf& m_mat();
	static const QStringList&     head();

	static void	readCsv(const QString& filename, bool hasHead);
	static void	readCnc(const QString& fileName);

private:
	/// @brief 单例模式
	DataSingleton(QObject* parent = nullptr);

	static DataSingleton*  m_instance;
	static Eigen::MatrixXf m_matrix;
	static QStringList     m_head;

signals:
	void dataChanged();

};

