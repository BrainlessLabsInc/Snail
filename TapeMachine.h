#ifndef __BRAINLESS_TAPEMACHINE__
#define __BRAINLESS_TAPEMACHINE__
/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
#include <memory.h>
#include "constNDefs.h"

/*This is a virtual machine  for which we are writing the programing language.*/
template< typename storageType, typename IO, ulong iTapeSize>
class TapeMachine
{
private:
    storageType m_iTapeStore[iTapeSize];
    IO m_inAndout;
    ulong m_iHeadPos;
    const ulong m_iSpecialRegStart;

public:
    TapeMachine():
      m_iHeadPos(0),
      m_iSpecialRegStart( iTapeSize - REGISTER_FILE ),
      m_inAndout()
    {
        memset(m_iTapeStore, HEX_0, sizeof(iTapeSize) * DIM(m_iTapeStore));
    }

    void moveHead(ulong num = 1, bool foreward = true)
    {
        if(false == foreward)
        {
            m_iHeadPos -= num;
        }
        else
        {
            m_iHeadPos += num;
        }
    }

    void moveHeadForeward(ulong num = 1)
    {moveHead(num, true);}
    void moveHeadBackward(ulong num = 1)
    {moveHead(num, false);}

    void addWithNext()
    {m_iTapeStore[m_iHeadPos] += m_iTapeStore[m_iHeadPos + 1];}
    void substractFromNext()
    {m_iTapeStore[m_iHeadPos] -= m_iTapeStore[m_iHeadPos + 1];}

    void getCharAndStore();
    void displayChar();
    void displayMultiChar(const ulong);

    void incrementContent()
    {m_iTapeStore[m_iHeadPos] += 1;}
    void decrementContent()
    {m_iTapeStore[m_iHeadPos] -= 1;}

    void setFlagRegister(ulong iFlag)
    {
        m_iTapeStore[m_iSpecialRegStart] &= ~COMPARE_FLAG_MASK;
        m_iTapeStore[m_iSpecialRegStart] |= iFlag;
    }

    ulong getFlagRegister()
    {
        return COMPARE_FLAG_MASK & m_iTapeStore[m_iSpecialRegStart];
    }

    char* getVMVersion()
    {return BCHAR(VM_VERSION);}

    storageType& getCurrentHeadValue();

    storageType& getNthPosValFrmHead(const long  iPos = 0);

    ulong getCurHeadPos()
    {return m_iHeadPos;}

};//End of class TapeMachine


template< typename storageType, typename IO, ulong iTapeSize>
inline void TapeMachine< storageType, IO, iTapeSize >::getCharAndStore()
{
    int val = 0;
    m_inAndout.putStringOnScreen("\nEnter a character>>");
    val = m_inAndout.getChar();
    m_iTapeStore[m_iHeadPos] = (storageType)val;
}

template< typename storageType, typename IO, ulong iTapeSize>
inline void TapeMachine< storageType, IO, iTapeSize >::displayChar()
{
    m_inAndout.printToScreen("\nValue at TapePosition[%d] = %c", m_iHeadPos, (char)m_iTapeStore[m_iHeadPos]);
}

template< typename storageType, typename IO, ulong iTapeSize>
inline storageType& TapeMachine< storageType, IO, iTapeSize >::getCurrentHeadValue()
{
    return getNthPosValFrmHead(/*default is 0*/);
}

template< typename storageType, typename IO, ulong iTapeSize>
inline storageType& TapeMachine< storageType, IO, iTapeSize >::getNthPosValFrmHead(const long iPos)
{
    return m_iTapeStore[m_iHeadPos + iPos];
}

template< typename storageType, typename IO, ulong iTapeSize>
inline void TapeMachine< storageType, IO, iTapeSize >::displayMultiChar(const ulong iSize)
{
    /*TODO improve using m_inAndout*/
	printf("Displaying %d elements::\n", iSize);
    for(ulong i = 0; i < iSize; i++)
    {
        printf("| %c |\n", (char)m_iTapeStore[m_iHeadPos + i]);
    }
	printf("Stop Displaying %d elements::\n", iSize);
}

#endif //__BRAINLESS_TAPEMACHINE__