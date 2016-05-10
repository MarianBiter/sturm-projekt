#include <Windows.h>
#include "HighResTimer.h"

HighResTimer::HighResTimer()
{
	QueryPerformanceFrequency(&m_frequency);
}

void HighResTimer::StartTimer()
{
	QueryPerformanceCounter(&m_startTime);
}

void HighResTimer::ResetTimer()
{
	QueryPerformanceCounter(&m_startTime);
}

double HighResTimer::GetElapsedTimeMs()
{
	QueryPerformanceCounter(&m_endTime);
	
	LARGE_INTEGER elapsedTime;
	elapsedTime.QuadPart = m_endTime.QuadPart - m_startTime.QuadPart;

	m_startTime = m_endTime;

	elapsedTime.QuadPart *= 1000;
	elapsedTime.QuadPart /= m_frequency.QuadPart;

	return elapsedTime.QuadPart;
}

double HighResTimer::GetElapsedTimeSec()
{
	QueryPerformanceCounter(&m_endTime);
	
	LARGE_INTEGER elapsedTime;

	elapsedTime.QuadPart = m_endTime.QuadPart - m_startTime.QuadPart;

	m_startTime = m_endTime;

	elapsedTime.QuadPart *= 1000;
	elapsedTime.QuadPart /= m_frequency.QuadPart;

	return elapsedTime.QuadPart;
}
