#ifndef __HIGH_RES_TIMER_H__
#define __HIGH_RES_TIMER_H__

class HighResTimer
{
public:
	HighResTimer();
	void StartTimer();
	void ResetTimer();
	double GetElapsedTimeMs();
	double GetElapsedTimeSec();
private:
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_endTime;
};

#endif