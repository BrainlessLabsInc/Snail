#ifndef __BRAINLESS_HELPER__
#define __BRAINLESS_HELPER__
/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
#include <stdio.h>
#include <string>
#include <map>
#include "constNDefs.h"

using namespace std;

class FileRead
{
private:
    int m_iCount;
    int m_iIndex;
    char m_szBuffer[BUFFER_SIZE];
    const string m_sFileName;
    FILE* m_pFile;
    int m_cCurChar;
    bool m_bEOFReached;

    map<long, long int> m_restorePoints;
    long m_iNextNum;

private:
    int getNextCharInternal();
    bool unreadCharInternal(const int);
    long getFileIndexInternal();
	void setFileIndexInternal(long index);
    long setRestorePointInternal();
    bool positionToRestorePointInternal(const long index, const bool remove = false);

public:
    FileRead(string sFileName):
          m_sFileName(sFileName),
          m_iCount(0),
          m_iIndex(-1),
          m_cCurChar(HEX_0),
          m_bEOFReached(false),
          m_pFile(NULL),
          m_iNextNum(0)
    {
        if(false == m_sFileName.empty())
        {
            m_pFile = fopen(m_sFileName.c_str(), "r+");
            if(NULL == m_pFile)
            {
                puts("Cannot open file\n");
                exit(0);
            }
        }

        memset(m_szBuffer, HEX_0, DIM(m_szBuffer));
    }

    ~FileRead()
    {
        if(NULL != m_pFile)
        {
            fclose(m_pFile);
            m_pFile = NULL;
        }
    }

    int getNextChar()
    {
        int ch('\0');

        do
        {
            ch = getNextCharInternal();
        }while( isWhiteSpace(ch) );

        return ch;
    }

    bool isWhiteSpace(const int ch)
    {
        bool rcode = false;//false till its proved to be true

        switch(ch)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\v':
            rcode = true;
            break;
        }

        return rcode;
    }

    int getCurrentChar()
    {return m_cCurChar;}

    int getNextNumber();

    BOOL isEOF()
    {return m_bEOFReached;}

    bool unreadChar(const int ch)
    {return unreadCharInternal(ch);}

    void setFileIndex(const long index)
	{setFileIndexInternal(index);}
	long getFileIndex()
    {return getFileIndexInternal();}
    long setRestorePoint()
    {return setRestorePointInternal();}
    bool positionToRestorePoint(const long index, const bool remove)
    {return positionToRestorePointInternal(index, remove);}
};

#endif //__BRAINLESS_HELPER__

