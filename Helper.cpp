/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
#include "Helper.h"
#include <ctype.h>
#include <stdlib.h>

int FileRead::getNextCharInternal()
{
    if(NULL == m_pFile)
    {
        m_cCurChar = '\0';
        return m_cCurChar;
    }

    if(false == m_bEOFReached)
    {
        m_cCurChar = getc(m_pFile);
        if(EOF == m_cCurChar)
        {
            m_bEOFReached = (feof(m_pFile) >= 1) ? true : false;
            m_cCurChar = '\0';
        }
    }

    return (char)m_cCurChar;
}

int FileRead::getNextNumber()
{
    char numStr[20] = {0};
    int index = 0;

    while( isdigit( numStr[index++] = (char)getNextCharInternal() ) )
        ;//Empty while loop to get numbers
    numStr[--index] = '\0';

    unreadChar(m_cCurChar);

    return atoi(numStr);
}

bool FileRead::unreadCharInternal(const int ch)
{
/*    if(0 > --m_iIndex)
        return false;

    m_szBuffer[++m_iCount, m_iIndex] = ch;
*/ 
    return (ungetc(ch, m_pFile) ==  ch) ? true : false;
}

long FileRead::getFileIndexInternal()
{
    if(NULL == m_pFile)
        return -1L;// IF the file is not open then no need to add another function call

    return ftell(m_pFile);
}

void FileRead::setFileIndexInternal(const long index)
{
    if(NULL == m_pFile)
	{
        printf("Not a valid file\n");
		return;// IF the file is not open then no need to add another function call
	}

    fseek(m_pFile, index, SEEK_SET);
}

long FileRead::setRestorePointInternal()
{
    m_restorePoints.insert(pair<long, long int>(m_iNextNum++, getFileIndexInternal()));
    return (m_iNextNum - 1);
}

bool FileRead::positionToRestorePointInternal(const long index, const bool remove)
{
    if(!m_restorePoints.empty())
    {
        fseek(m_pFile, m_restorePoints[index], SEEK_SET);
        if(true == remove)
            m_restorePoints.erase(index);
        return true;
    }

    return false;
}