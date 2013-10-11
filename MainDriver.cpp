/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
#include "Interpreter.h"
#include <stdio.h>
#include "TimeLogger.h"

void help()
{
    printf("brainless <fileName>\n");
}

int main(int numArgs, char* ArgVectors[])
{
    if(numArgs < 2)
    {
        help();
        return 0;
    }

	LOGTIME_INIT("Logger.log");
    string fileName(ArgVectors[1]);
	printf("Interperting file [ %s ]\n", ArgVectors[1]);
	FUNCTION_BEGIN("Interprete Start");
    Interpreter interpreter( fileName );
    interpreter.interpreteScript();
	FUNCTION_END;
    getchar();

    return 0;
}
