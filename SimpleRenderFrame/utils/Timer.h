#pragma once

#include<Windows.h>

class Timer
{
public:
	Timer();
	~Timer();
	float GetDeltaTime() const;
	//<Return application running time>
	float GetTotalTime() const;

	void Start();
	void Reset();
	void Stop();
	void Tick();

private:
	double m_secondsPerCount;
	double m_deltaTime;

	__int64 m_baseTime;
	__int64 m_pausedTime;
	__int64 m_stopTime;
	__int64 m_prevTime;
	__int64 m_currTime;

	bool m_stopped;
};

