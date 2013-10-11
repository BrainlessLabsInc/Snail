#ifndef __BRAINLESS_INTERFACE__
#define __BRAINLESS_INTERFACE__
/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
#include <stdio.h>
#include <stdarg.h>
#include "constNDefs.h"

#ifdef WINPLATFORM
#include <conio.h>
#endif

class IOInterface
{
public:
    IOInterface(){}
    ~IOInterface(){}

    int getChar()
#if !defined( WINPLATFORM )
    {return getchar();}
#else
    {return _getch();}
#endif
    int putStringOnScreen(const char* str)
    {return puts(str);}
    int printToScreen(const char* str, ...)
    {
        int rcode = 0;
        va_list argptr = {0};
        va_start(argptr, str);
        /*for now we know we only have 2 parameters, so no need to parse and know.*/
        rcode = printf(str, va_arg(argptr, ulong), va_arg(argptr, char));
        va_end(argptr);
        return rcode;
    }
};


#endif //__BRAINLESS_INTERFACE__