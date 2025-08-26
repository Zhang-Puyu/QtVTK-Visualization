#pragma once

class AbstractLiveWidget
{
public:
	/// @brief ��ʼ��̬��ʾ
	virtual void startLiveView() = 0;
	/// @brief ֹͣ��̬��ʾ
	void stopLiveView() { m_liveContinueFlag = false; }

	/// @brief ����ˢ�¼��
	/// @param interval ˢ�¼��: ms
	void setInterval(unsigned int interval) { m_liveInterval = interval; }
	/// @brief ���ó���ʱ��
	/// @param duration ����ʱ��: s
	void setDuration(unsigned int duration) { m_liveDuration = duration; }

protected:
	virtual void render() = 0;

	/// @brief ˢ�¼�� ms
	unsigned int m_liveInterval = 50; // ms
	/// @brief ����ʱ�� s
	unsigned int m_liveDuration = 10; // s

	/// @brief �Ƿ������̬��ʾ
	bool m_liveContinueFlag = true;
};
