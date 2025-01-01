#include "DataSingleton.h"


DataSingleton*	DataSingleton::m_instance = new DataSingleton();
Eigen::MatrixXf DataSingleton::m_matrix   = Eigen::MatrixXf();
QStringList		DataSingleton::m_head	  = QStringList();

const Eigen::MatrixXf& DataSingleton::mat()
{
	return m_instance->m_matrix;
}

const QStringList& DataSingleton::head()
{
	return m_instance->m_head;
}

DataSingleton::DataSingleton(QObject* parent)
	: QObject(parent)
{

}

void DataSingleton::readCsv(const QString& fileName, bool hasHead)
{

}

void DataSingleton::readCnc(const QString& fileName)
{

}
