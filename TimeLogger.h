#ifndef _TIMELOGGER_bb056fff_a11f_46e8_9b95_df6f32fe2fcf_
#define _TIMELOGGER_bb056fff_a11f_46e8_9b95_df6f32fe2fcf_

/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
//#include <stdio.h>
#ifndef _IOSTREAM_INC
#define _IOSTREAM_INC
#include <iostream>
#endif //_IOSTREAM_INC

#ifndef _FSTREAM_INC
#include<fstream>
#endif //_FSTREAM_INC
#include "CTimer.h"

class TimeLogger
{
public:
	static ofstream _file;
	static LARGE_INTEGER _start;
	static LARGE_INTEGER _end;

private:
	TimeLogger()
	{}

	TimeLogger(const TimeLogger& rhs)
	{}

	TimeLogger& operator=(TimeLogger&)
	{return *this;}

	void getTime(std::string&);

public:
	~TimeLogger()
	{
		if(_file.is_open())
			_file.close();
	}

	static TimeLogger& instance()
	{
		static TimeLogger loggerInstance;
		return loggerInstance;
	}

	void init(const std::string& fileName);

	void functionStart(const std::string& funName);
	void functionEnd(const std::string& funName);
};

#ifdef _LOG_TIME
#define LOGTIME_INIT(name) TimeLogger::instance().init(name);

#define FUNCTION_BEGIN(name) std::string bb056ffffunName(name);\
TimeLogger::instance().functionStart(bb056ffffunName);

#define FUNCTION_END TimeLogger::instance().functionEnd(bb056ffffunName);
#else
#define LOGTIME_INIT(name) ;
#define FUNCTION_BEGIN(name) ;
#define FUNCTION_END ;
#endif//_LOG_TIME

#endif //_TIMELOGGER_bb056fff_a11f_46e8_9b95_df6f32fe2fcf_
