#include "Timer.h"



Timer::Timer():
	m_secondsPerCount(0.0),m_deltaTime(-1.0),
	m_stopped(false),m_currTime(0),m_prevTime(0),
	m_pausedTime(0),m_baseTime(0)
{
	//Get cycle time
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_secondsPerCount = 1.0 / (double)countsPerSec;
}

Timer::~Timer()
{
}

float Timer::GetDeltaTime() const
{
	return static_cast<float>(m_deltaTime);
}

float Timer::GetTotalTime() const
{
	if (m_stopped)
	{
		return ((m_stopTime - m_pausedTime) - m_baseTime)*m_secondsPerCount;
	}
	else
	{
		return ((m_currTime - m_pausedTime) - m_baseTime)*m_secondsPerCount;
	}
}

void Timer::Start()
{
	if (m_stopped)
	{
		m_stopped = false;
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		m_pausedTime += (startTime-m_stopTime);
		m_stopTime = 0;
		m_prevTime = startTime;
		m_currTime = startTime;
	}
}

void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_baseTime = currTime;
	m_currTime = currTime;
	m_prevTime = currTime;
	m_stopTime = 0;
	m_stopped = false;
}

void Timer::Stop()
{
	if (!m_stopped)
	{
		m_stopped = true;
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		m_stopTime = currTime;
	}
}

void Timer::Tick()
{
	if (m_stopped)
	{
		m_deltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_currTime = currTime;
	m_deltaTime = (m_currTime - m_prevTime)*m_secondsPerCount;
	m_prevTime = currTime;

	if (m_deltaTime < 0.0)
		m_deltaTime = 0.0;
}
