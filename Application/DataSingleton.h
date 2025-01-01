#pragma once

#include "qobject.h"
#include "qstringlist.h"

#include <Eigen/Dense>

class DataSingleton : public QObject
{
	Q_OBJECT

public:
	// ½ûÖ¹¿½±´ºÍ¸³Öµ²Ù×÷
	DataSingleton(const DataSingleton&) = delete;
	DataSingleton& operator=(const DataSingleton&) = delete;

	static const Eigen::MatrixXf& mat();
	static const QStringList&     head();

	static void	readCsv(const QString& filename, bool hasHead);
	static void	readCnc(const QString& fileName);

private:
	DataSingleton(QObject* parent = nullptr);

	static DataSingleton*  m_instance;
	static Eigen::MatrixXf m_matrix;
	static QStringList     m_head;

signals:
	void dataChanged();

};

