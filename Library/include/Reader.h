#pragma once

#include "qstring.h"
#include "qstringlist.h"
#include "qregularexpression.h"
#include "qprogressdialog.h"

#include <Eigen/Dense>

class Reader : public QObject
{
	Q_OBJECT;

public:
	Reader(QObject* parent = nullptr)
		: QObject(parent) {
	};

	/// @brief �ж��ַ����Ƿ���������ַ�
	static bool hasChinese(const QString& str)
	{
		QRegularExpression regex("[\\u4e00-\\u9fa5]+");
		return regex.match(str).hasMatch();
	}

	/// @brief Ϊ���ݶ�ȡ������ʾ���ȴ���
	/// @param parent ���ȶԻ���ĸ�����
	void addReadProgressDialog(QWidget* parent = nullptr)
	{
		QProgressDialog* dialog = new QProgressDialog(parent);
		// �Ի�������
		dialog->setLabelText(tr("���ڶ�ȡ�ļ�..."));
		// �Ի������
		dialog->setWindowTitle(tr("��ȡ����"));
		// ģ̬�Ի���
		dialog->setWindowModality(Qt::WindowModal);
		// ȡ����ť����
		dialog->setCancelButtonText(tr("����"));
		//������������е�ʱ��С��5���������Ͳ�����ʾ��Ĭ����4000ms
		dialog->setMinimumDuration(0);

		// ��ȡ��ʼʱ�򿪽��ȶԻ���
		connect(this, &Reader::readStarted, dialog, &QProgressDialog::show);
		// ��ȡ��ʼʱ���ý��ȷ�Χ
		connect(this, &Reader::readStarted, dialog, &QProgressDialog::setMaximum);
		// ��ȡ����ʱ���½��ȶԻ���
		connect(this, &Reader::readProgress, dialog, &QProgressDialog::setValue);
		// ��ȡ����ʱ�رս��ȶԻ���
		connect(this, &Reader::readFinished, dialog, &QProgressDialog::reset);

		// ��reset()ʱ���Ի����Զ��ر�
		dialog->setAutoClose(true);
		// ������Сֵ
		dialog->setMinimum(0);
		// ���õ�ǰֵ
		dialog->setValue(0);
	}

	/// @brief ��ȡcsv�ļ�
	/// @param mat ��ȡ�ľ�������
	/// @param fileName �ļ���
	void readCsv(Eigen::MatrixXf& mat, const QString& fileName);

	/// @brief ��ȡcsv�ļ�
	/// @param mat ��ȡ�ľ�������
	/// @param fileName �ļ���
	/// @param head csv��һ����Ϊ��ͷ
	void readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head);

	/// @brief ��ȡ���ص����ļ�
	/// @param mat ��λ�����
	/// @param fileName �ļ���
	void readCnc(Eigen::MatrixXf& mat, const QString& fileName);

	/// @brief ��ȡ���ص����ļ�
	/// @param mat ��λ�����
	/// @param fileName �ļ���
	/// @param head ��λ�����
	void readCnc(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head);

private:
	/// @brief ƥ����������
	const QRegularExpression expPostiveFloat = QRegularExpression("((\\b\\d+\\.\\d+|\\b\\d+))");
	/// @brief ƥ�両����
	const QRegularExpression expFloat = QRegularExpression("-?\\b\\d+(\\.\\d+)?\\b");
	/// @brief ƥ��G����
	const QRegExp expGCodeReg = QRegExp("(([A-Z])\\s*(-?\\d+(\\.\\d+)?|\\.\\d+))");

	/// @brief ƥ����
	QRegularExpressionMatch m_match;

	/// @brief ���ٽ������
	int   m_rapidFlag = 0;
	/// @brief ���ٽ����ٶ�
	float m_rapidFeed = 6000;
	/// @brief ���ٽ���֮ǰ�����������ٶ�
	float m_cuttingFeed = 0.0f;

	/// @brief ����һ��APT����
	/// @param line APT����
	/// @param point �����õ��ĵ�λ��
	void paraseAptLine(const QString& line, Eigen::VectorXf& point);

	/// @brief ����һ��G����
	/// @param line G����
	/// @param point �����õ��ĵ�λ��
	void paraseGCodeLine(const QString& line, Eigen::VectorXf& point);

	/// @brief ��ȡcsv�ļ�
	/// @param mat ��ȡ�ľ�������
	/// @param fileName �ļ���
	/// @param head �����ͷ����skipFirstRow = trueʱ�Ż���Ч�޸�
	/// @param skipFirstRow ��ȡ�ļ�ʱ�Ƿ�������һ��
	void readCsv(Eigen::MatrixXf& mat, const QString& fileName, QStringList& head, const bool skipFirstRow);

signals:
	/// @brief ��ȡ�ļ���ʼ
	/// @param rowCount �ļ�������
	void readStarted(unsigned int rowCount);
	/// @brief ��ȡ�ļ����
	/// @param rowCount �ļ�������
	void readFinished(unsigned int rowCount);

	/// @brief �ļ���ȡ����
	/// @param row ��ǰ����
	void readProgress(unsigned int row);
};

