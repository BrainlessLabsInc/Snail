#ifndef _CTIMER_bb056fff_a11f_46e8_9b95_df6f32fe2fcf_
#define _CTIMER_bb056fff_a11f_46e8_9b95_df6f32fe2fcf_
/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/

#ifndef _IOSTREAM_INC
#define _IOSTREAM_INC
#include <iostream>
#endif //_IOSTREAM_INC

#ifndef _WIN_HEADER_
#define _WIN_HEADER_
#include <windows.h>
#endif //_WIN_HEADER_

typedef LARGE_INTEGER tInt;

class CTimer
{
private:
	static LARGE_INTEGER _start;
	static LARGE_INTEGER _end;
	static double _timeElapsedInSec;
	static const double _frequency;
public:
public:
	typedef LARGE_INTEGER tInt;
	static double getFrequency()
	{
		LARGE_INTEGER proc_freq;
		if (!::QueryPerformanceFrequency(&proc_freq))
			std::cout<<"QueryPerformanceFrequency() failed"<<std::endl;
		//else
		//std::cout<<"QueryPerformanceFrequency() called"<<std::endl;
		return proc_freq.QuadPart;
	}

public:
	static void startTimer()
	{
		DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
		::QueryPerformanceCounter(&_start);
		::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	}

	static void startTimer(tInt& start)
	{
		DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
		::QueryPerformanceCounter(&start);
		::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	}

	static void stopTimer()
	{
		DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
		::QueryPerformanceCounter(&_end);
		::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
		_timeElapsedInSec = ((_end.QuadPart - _start.QuadPart) / _frequency);
		// Shouldn't you divide by frequency instead of multiply?
		// Yes, indeed I was. Old was: frequency = 1.0 / GetFrequency()
		// Still, I think directly dividing is clearer, so I changed it.
	}

	static void stopTimer(tInt& end)
	{
		DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
		::QueryPerformanceCounter(&end);
		::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
		//_timeElapsedInSec = ((_end.QuadPart - _start.QuadPart)/ _frequency);
		// Shouldn't you divide by frequency instead of multiply?
		// Yes, indeed I was. Old was: frequency = 1.0 / GetFrequency()
		// Still, I think directly dividing is clearer, so I changed it.
	}

	static float getTimeElapsed(const tInt start, const tInt end)
	{
		return ((end.QuadPart - start.QuadPart) / getFrequency());
	}

	static double getTimeElapsed()
	{
		return _timeElapsedInSec;
	}
};

/*
const double CTimer::_frequency = getFrequency();
LARGE_INTEGER CTimer::_start = {0};
LARGE_INTEGER CTimer::_end = {0};
double CTimer::_timeElapsedInSec = {0};
*/


#define ST(start) /*Shakti ST - Start*/CTimer::startTimer(start)/*Shakti ST - End*/

#define ET(end) /*Shakti ET - Start*/CTimer::stopTimer(end)/*Shakti ET - end*/

#define DIFF(start, end) /*Shakti DIFF - Start*/CTimer::getTimeElapsed(start, end)/*Shakti DIFF - End*/

#endif //_CTIMER_bb056fff_a11f_46e8_9b95_df6f32fe2fcf_
