#include <cstdio>
#include <time.h>
using namespace std;

#include "TimeLogger.h"
const double CTimer::_frequency = getFrequency();
LARGE_INTEGER CTimer::_start = {0};
LARGE_INTEGER CTimer::_end = {0};
double CTimer::_timeElapsedInSec = {0};
LARGE_INTEGER TimeLogger::_start;
LARGE_INTEGER TimeLogger::_end;

ofstream TimeLogger::_file;

void TimeLogger::init(const string& fileName)
{
	_file.open(fileName.c_str());
	if(!_file.is_open() || !_file.good())
		cerr<<"Cannot open file"<<endl;
}

void TimeLogger::functionStart(const std::string& funName)
{
	time_t rawtime;
	string timeS;
	getTime(timeS);

	ST(_start);
	char str[1024] = {0};
	sprintf(str, "%s:%I64d %s Start{",  timeS.c_str(), _start.QuadPart, funName.c_str());
	_file<<str<<endl;
}

void TimeLogger::functionEnd(const std::string& funName)
{
	string timeS;
	getTime(timeS);
	ET(_end);
	float diff = DIFF(_start, _end);
	char str[1024] = {0};
	sprintf(str, "%s:%I64d %s End}",  timeS.c_str(), _end.QuadPart, funName.c_str());
	_file<<str<<endl;
	sprintf(str, "============%s execution time = %f seconds============%",  funName.c_str(), diff);
	_file<<str<<endl;
}


void TimeLogger::getTime(string& timeS)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	timeS = asctime (timeinfo);
	timeS[timeS.length()-2] = '\0';
}