/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/

#include "Lexer.h"
/*
Parse the input characters and return the proper token for each. Get character one by one, and
*/

enmTokens Lex::getNextToken()
{   
    switch(getNextChar())
    {
    case '\0':
        m_eCurToken = eTokenOver;break;//End of file reached
    case '<':
        m_eCurToken = eLeftAngleBracket;break;
    case '=':
        m_eCurToken = eEqualTo;break;
    case '\\':
        switch(getNextChar())
        {
        case '>':
            m_eCurToken = eGreaterThan;break;
        case '<':
            if('>' == getNextChar())
            {
                m_eCurToken = eCompareGeneral;
            }
            else
            {
                m_fileReader.unreadChar( getCurrentChar() );
                m_eCurToken = eLessThan;
            }
            break;
        case '=':
            m_eCurToken = eEquality;break;
        default:
            m_fileReader.unreadChar( getCurrentChar() );
            m_eCurToken = eBackSlash;break;
        }
        break;
    case '.':
        m_eCurToken = eDspCurVal;break;//Display value
    case '*':
        m_eCurToken = eStarChar;break;
    case '+'://If we have + it can be either ++ or +*. So see where it falls
        switch(getNextChar())
        {
        case '+':
            m_eCurToken = eIncrContent;break;
        case '*':
            m_eCurToken = eAddWithNext;break;
        default:
            m_eCurToken = eUnknown;break;
        }
        break;
    case '-'://If we have - it can be either -- or -*. So see where it falls
        switch(getNextChar())
        {
        case '-':
            m_eCurToken = eDecrContent;break;
        case '*':
            m_eCurToken = eSubNext;break;
        default:
            m_eCurToken = eUnknown;break;
        }
        break;
    case '>'://If we have >, it can be >> if we have single > then its not avalid token 
        switch(getNextChar())
        {
        case '>':
            m_eCurToken = eRdAndStrVal;break;
        default:
            m_fileReader.unreadChar(getCurrentChar());
            m_eCurToken = eRightAngleBracket;break;
        }
        break;
    case '!':
        m_eCurToken = eExclaimationMark;break;
    case '[':
        m_eCurToken = eLeftSqrParen;break;
    case ']':
        m_eCurToken = eRightSqrParen;break;
    case '(':
        m_eCurToken = eLeftParen;break;
    case ')':
        m_eCurToken = eRightParen;break;
    case 'I':
        switch(getNextChar())
        {
        case 'F':
            m_eCurToken = eIF;break;
        default:
            m_fileReader.unreadChar(getCurrentChar());
            m_eCurToken = eUnknown;break;
        }
        break;
    case 'F':
        switch(getNextChar())
        {
        case 'I':
            m_eCurToken = eFI;break;
        default:
            m_fileReader.unreadChar(getCurrentChar());
            m_eCurToken = eUnknown;break;
        }
        break;
    case 'D':
        switch(getNextChar())
        {
        case 'O':
			if('N' == getNextChar())
			{
				if('E' == getNextChar())
					m_eCurToken = eDONE;
				else
				{//Back track block
					m_fileReader.unreadChar('O');
					m_fileReader.unreadChar('N');//If i come in this block i know its already N
					m_fileReader.unreadChar(getCurrentChar());//Back trace
				}
				break;
			}
			else
			{
				m_fileReader.unreadChar(getCurrentChar());//Back trace
				m_eCurToken = eDO;//We dont seperate tokens by first match
				//We do them by first cum first serve basis
			}
			break;
        default:
            m_fileReader.unreadChar(getCurrentChar());//Back trace
            m_eCurToken = eUnknown;break;
        }
        break;
    case '#':
        switch(getNextChar())
        {
        case 'B':
            m_eCurToken = eBlock;break;
        default:
            m_fileReader.unreadChar(getCurrentChar());//Back trace
            m_eCurToken = eUnknown;break;
        }
        break;
	case ':':
		m_eCurToken = eColon;
		break;
    case '/':
        switch(getNextChar())
        {
        case '*':
            while('/' != getNextChar());break;//TODO need to improve it.
        }
        break;
    default:
        if( isdigit( getCurrentChar() ) )
        {
            m_fileReader.unreadChar(getCurrentChar());
            int val = m_fileReader.getNextNumber();
            m_eCurToken = eIntNumber;
            m_tokAttrib.setAttribVal(val);
        }
        else if( isalpha( getCurrentChar() ) )
        {
            char val = getCurrentChar();
            m_eCurToken = eCharVal;
            m_tokAttrib.setAttribVal(val);
        }
        else
        {
            m_eCurToken = eUnknown;
        }
        break;
    }

    return m_eCurToken;
}
