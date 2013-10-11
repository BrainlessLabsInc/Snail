#ifndef __BRAINLESS_LEXER__
#define __BRAINLESS_LEXER__
/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/

/*This header contains declarations and definitions for class which are used for tokenizing the souce file*/
#include <string>
#include <stdio.h>
#include "Helper.h"

/*All the tokens should be here. If some one is adding a new token, they have to include it in this token list.*/
enum enmTokens
{
    eRdAndStrVal = 0, // >>
    eDspCurVal = 1, // .
    eExclaimationMark = 2, // !
    eLeftSqrParen = 3, // [
    eRightSqrParen = 4, // ]
    eIncrContent = 5, // ++
    eDecrContent = 6, // --
    eAddWithNext = 7, // +*
    eSubNext = 8, // -*
    eStarChar = 9, // *
    //eDecrHead, // <
    eIntNumber, // any integer number
    eSingleQuote,//'
    eDoubleQuote,//"
    eForewardSlash,// /
    eBackSlash,// \ 
    eCharVal,// all characters
    eLeftAngleBracket,// <
    eRightAngleBracket,// >
    eLeftParen,// (
    eRightParen,// )
    eEqualTo,// =
    eLessThan,// \<
    eGreaterThan,// \>
    eEquality,// \=
    eCompareGeneral,// \<>
    eIF,// IF
    eELSE,// ELSE
    eFI,// FI
    eDO,// Do LOOP
	eDONE,
    eHash,// #
    eBlock,// #B one execution block of code
	eColon,// :
    eTokenOver = 666, // Represents that we have reached end of file.
    eUnknown // Any value out side of the above values.
};

/*tokens can have attributes, like a group of characters like "123" is a single number with the
value attribute of 123. This value is held by this class.*/
class TokenAttrib
{
private:
    long m_iVal; //This holds the value of the attribute, currently only supports integer number
    bool m_bAttribSet; //If this is set to "true" then only we know we need to look for m_iVal.

public:
    TokenAttrib():
      m_bAttribSet(false)//Set default to false
    {m_iVal = -666;}//Giving some absurd but easy to remember number. Helps in debugging.

    void setAttribVal(const long val)//Will set the attribute value and also the bools.
    {
        m_iVal = val;//Set the value
        m_bAttribSet = true;//Do not foreget to make it true.
    }

    long getAttribVal()
    {return m_iVal;}

    bool isAttribSet()
    {return m_bAttribSet;}//Check if the attribute is set. Return true if it is set

    void resetAttrib()
    {m_bAttribSet = false;}//Reset the value.

};

/*This class will get one by one characters from the source file and then
catagorize it according to any of the enmTokens groups.*/
class Lex
{
private:
    char m_szCurChar;
    char m_szPrevChar;
    const string m_sFileName;//Stores the source file name
    const bool m_bWriteTokenTofile;//If we want to create a token file for fun we can create it. Not used still
    string m_sTokenFile;//Token file name.
    enmTokens m_eCurToken;//Type of the current token that is scanned.
    FILE* m_pTokenfile;//Token file name
    FileRead m_fileReader;//This is the reader class. It will help us read the source file.
    TokenAttrib m_tokAttrib;//Store the attributes of the token.

private:
    char getCurrentChar()
    {return m_szCurChar;}
   
    char getNextChar()
    {
        m_szPrevChar = m_szCurChar;//First store the previous character.
        m_szCurChar = (char)m_fileReader.getNextChar();
        return m_szCurChar;
    }
    
    void openTokenFile()
    {
        if(NULL == m_pTokenfile)
            m_pTokenfile = fopen(m_sTokenFile.c_str(), "w");
    }

public:
    Lex(const string sFileName, const bool WriteTokenTofile = false /*Make it default to false.*/):
        m_sFileName(sFileName),
        m_szCurChar(0),
        m_szPrevChar(0),
        m_bWriteTokenTofile(WriteTokenTofile),
        m_eCurToken(eUnknown),
        m_fileReader(m_sFileName)
    {
        m_pTokenfile = NULL;

        if(true == m_bWriteTokenTofile)
        {
            setTokenFileName(m_sFileName);
            openTokenFile();
        }
    }

    ~Lex()
    {
        if(NULL != m_pTokenfile)
        {
            fclose(m_pTokenfile);
            m_pTokenfile = NULL;
        }
    }

    void setTokenFileName(string sFileName)
    {
        m_sTokenFile = sFileName + ".Token";
    }
    
    string getTokenFileName()
    {return m_sTokenFile;}

    enmTokens getNextToken();//Get the next token

    enmTokens getCurrentToken()
    {return m_eCurToken;}//Get the current token. We do not increment the token here.

    int getAttribVal()
    {return m_tokAttrib.getAttribVal();}//Get the token attribute value here.

    long setRestorePoint()
    {return m_fileReader.setRestorePoint();}
    bool positionToRestorePoint(const long index, const bool remove = false)
    {return m_fileReader.positionToRestorePoint(index, remove);}

	long getTokenIndex()
	{
		return m_fileReader.getFileIndex();
	}

	void setTokenIndex(const long index)
	{
		m_fileReader.setFileIndex(index);
	}

};


#endif //__BRAINLESS_LEXER__