#pragma once

class AbstractLiveWidget
{
public:
	/// @brief 开始动态显示
	virtual void startLiveView() = 0;
	/// @brief 停止动态显示
	void stopLiveView() { m_liveContinueFlag = false; }

	/// @brief 设置刷新间隔
	/// @param interval 刷新间隔: ms
	void setInterval(unsigned int interval) { m_liveInterval = interval; }
	/// @brief 设置持续时间
	/// @param duration 持续时间: s
	void setDuration(unsigned int duration) { m_liveDuration = duration; }

protected:
	virtual void render() = 0;

	/// @brief 刷新间隔 ms
	unsigned int m_liveInterval = 50; // ms
	/// @brief 持续时间 s
	unsigned int m_liveDuration = 10; // s

	/// @brief 是否继续动态显示
	bool m_liveContinueFlag = true;
};
