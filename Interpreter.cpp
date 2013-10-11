/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/

#include "Interpreter.h"
#include "TimeLogger.h"

#define START_RESTORE_PT( a ) const long a( m_lex.setRestorePoint() )
#define END_RESTORE_PT( a ) 

void Interpreter::interpreteScript()//Main routine to start script interpretation. this is semantic analysis or meaning findout method.
{
	FUNCTION_BEGIN("Interpreter::interpreteScript()");
	m_lex.getNextToken();
	while( eTokenOver != m_lex.getCurrentToken() )
	{
		enmTokens tok = m_lex.getCurrentToken();//shakti del
		Sentence();
		tok = m_lex.getCurrentToken();
	}
	FUNCTION_END;
}

// #B(<Block-Name>:<Block-Content>) - for defining a block, which translates to as below
// eBlock eLeftParen (eCharVal)+ eColon eRightParen
// #B(<Block-Name>) calls a block which translate into
// eBlock eLeftParen (eCharVal)+ eRightParen 
void Interpreter::Block()
{
	FUNCTION_BEGIN("Interpreter::Block()");
	/*
	A block is of the form #B(BlockName)[BlockContent]
	*/
	char szBlockName[BLOCK_NAME_SIZE] = {0};
	static unsigned int functionCount(0);

	if( eLeftParen == m_lex.getNextToken() )//#B already consumed, check if we have a (
	{
		int index = 0;

		//Get the block name and store it
		while( (eCharVal == m_lex.getNextToken()) //Any thing after ( is a block names
			&& (index < BLOCK_NAME_SIZE) 
			)
		{
			szBlockName[index++] = (char)m_lex.getAttribVal(); //extract the name
		}


		if((index > BLOCK_NAME_SIZE) || 
			isBlankOrNull(szBlockName))
		{
			printf("Invalid block name [ %s ].\n", szBlockName);//TODO proper error handling
		}

		/*Now if we find a ":" like #B(<NAME>: we know that a body follows.
		When a body follows, we jbust store the body starting location with the block name
		in a map. So when the block is called as #B(<NAME>) we can execute the code.
		This is done by searching the map for the name, get the index and repositdion the 
		cursor to that position.
		*/
		if(eColon == m_lex.getCurrentToken())
		{
			//Store the name and the index
			pair<symTable::iterator, bool> ret = 
				m_funSymTable.insert( 
				pair<string, ulong>( szBlockName, m_lex.getTokenIndex() ) 
				);
			
			//If false is returned, then the block is already defined.
			//re definition of block not possible.
			if(false == ret.second)
			{
				printf("Function [ %s ] already defined. Do you want to call it?\n", szBlockName);
			}

			unsigned int parenCount(1); //Already we have seen one (

			while( eTokenOver != m_lex.getNextToken())
			{
				if(eLeftParen == m_lex.getCurrentToken())
					++parenCount;
				else if(eRightParen == m_lex.getCurrentToken())
					--parenCount;
				if(0 == parenCount)
					break;
			}
		}//if(eColon == m_lex.getCurrentToken()) end
		else if(eRightParen == m_lex.getCurrentToken())
		{/*
		 if instead of ":" we see a ")" then we know that its a block call. 
		 */
			if( m_funSymTable.empty() )
			{
				printf("No functions defined.\n");
				FUNCTION_END;
				return;
			}
			
			++functionCount;
			//Search for the block name and find the location
			const symTable::iterator it = m_funSymTable.find( szBlockName );
			if( it == m_funSymTable.end() )
			{
				printf("Function not found.\n");
				FUNCTION_END;
				return;
			}

			//Before going there, store the return address, so after  block is 
			//executed we can return to this place.
			const ulong returnAddress = m_lex.getTokenIndex();
			m_lex.setTokenIndex( it->second );
			while(eTokenOver != m_lex.getNextToken())
			{
				if(eRightParen == m_lex.getCurrentToken())
				{// When we find ) we know a function body ends, so return
					--functionCount;
					//Set the index before retrning.
					m_lex.setTokenIndex( returnAddress );
					break;
				}
				ExecuteSentence();//If ")" not found block bodt still remaining
			}
			FUNCTION_END;
			return;
		}//else if(eRightParen == m_lex.getCurrentToken()) end
		else
		{
			printf("Unknown token in Block.\n");
		}
	}
	else
	{
		printf("Not a valid block.\n");
	}
	FUNCTION_END;
	return;
}

void Interpreter::ExecuteSentence()
{
	FUNCTION_BEGIN("Interpreter::ExecuteSentence()");
enmTokens tok = m_lex.getCurrentToken();//shakti del
switch(m_lex.getCurrentToken())
{//Based on the tokens choose the path.
case eRdAndStrVal:
	m_tapeVirtualMachine.getCharAndStore();break;
case eCharVal:break;//TODO
	/*        case eBackSlash:
	compareWithNext();break;
	*/
case eDspCurVal:
	m_tapeVirtualMachine.displayChar();break;
case eIncrContent:
	m_tapeVirtualMachine.incrementContent();break;
case eDecrContent:
	m_tapeVirtualMachine.decrementContent();break;
case eAddWithNext:
	m_tapeVirtualMachine.addWithNext();break;
case eSubNext:
	m_tapeVirtualMachine.substractFromNext();break;
case eStarChar:
	m_tapeVirtualMachine.moveHeadForeward();break;
case eLeftAngleBracket:
	m_tapeVirtualMachine.moveHeadBackward();break;
case eLessThan:// \<
case eGreaterThan:// \>
case eEquality:// \=
	compareWithNextBoolean();break;
case eCompareGeneral://\<>
	compareWithNext();break;
case eExclaimationMark:
	intDisplayContent();
	tok = m_lex.getCurrentToken();
	break;
case eIF://IF(\<)...FI
case eFI://FI dispatch control to IF block code only, it knows how to handle this
	IFBlock();
	tok = m_lex.getCurrentToken();
	break;
case eBlock://#B(BlockName:BlockBody)
	Block();break;
case eDO://DO...DONE
	DOLoop();//No case for "DONE". DONE is consumed in DOLoop function
	break;
default:
	tok = m_lex.getCurrentToken();
	FUNCTION_END;
	exit(0);
	break;//TODO error handling
}
FUNCTION_END;
}

void Interpreter::Sentence()
{
	FUNCTION_BEGIN("Interpreter::Sentence()");
	ExecuteSentence();
	enmTokens tok = m_lex.getCurrentToken();
	tok = m_lex.getNextToken();//Advance the token
	FUNCTION_END;
}

//This is for the DO...DONE loop
//Till the flag register is 1 it will execute the code inside the DO and DONE block
void Interpreter::DOLoop()
{
	FUNCTION_BEGIN("Interpreter::DOLoop()");
	ulong startPoint(m_lex.getTokenIndex());
	ulong endPoint(0);
	while( !(HEX_1 ^ m_tapeVirtualMachine.getFlagRegister()) )//HEX_1 == m_tapeVirtualMachine.getFlagRegister()
	{
		while( eTokenOver != m_lex.getNextToken() )
		{
			if(eDONE == m_lex.getCurrentToken())
			{
				endPoint = m_lex.getTokenIndex();
				m_lex.setTokenIndex(startPoint);
				break;
			}
			ExecuteSentence();
		}
	}
	if(endPoint > 0)
		m_lex.setTokenIndex(endPoint);
	else//It hasnt gone inside the above loop so we need to consume till the DONE
	{
		unsigned int doCount(1);
		while( eTokenOver != m_lex.getNextToken() )
		{
			if(eDONE == m_lex.getNextToken())
				--doCount;
			else if(eDO == m_lex.getNextToken())
				++doCount;
			if(0 == doCount)
				return;
		}
	}
	FUNCTION_END;
}

void Interpreter::intDisplayContent()
{   //When we are in this function we know that it was due to ![eExclaimationMark] that this function is called.
	//So check for eLeftSqrParen eIntNumber eRightSqrParen structure.
	FUNCTION_BEGIN("Interpreter::intDisplayContent()");
	switch(m_lex.getNextToken())//The tokens group eExclaimationMark eLeftSqrParen eIntNumber eRightSqrParen is a valid token.
	{
	case eLeftSqrParen:
		int len(0);
		if(eIntNumber == m_lex.getNextToken())
		{
			len = (int)m_lex.getAttribVal();
		}
		if(eRightSqrParen == m_lex.getNextToken())
		{
			m_tapeVirtualMachine.displayMultiChar(len);
		}
	}
	FUNCTION_END;
}

void Interpreter::compareWithNextBoolean()
{
	FUNCTION_BEGIN("Interpreter::compareWithNextBoolean()");
	compareWithNext();

	const ulong flagVal = m_tapeVirtualMachine.getFlagRegister();
	bool valSet = false;

	switch( m_lex.getCurrentToken() )
	{
	case eLessThan:
		if(!(flagVal ^ HEX_1))
		{
			m_tapeVirtualMachine.setFlagRegister(HEX_1);
			valSet = true;
		}
		break;
	case eGreaterThan:
		if(!(flagVal ^ HEX_2))
		{
			m_tapeVirtualMachine.setFlagRegister(HEX_1);
			valSet = true;
		}
		break;
	case eEquality:
		if(!(flagVal ^ HEX_3))
		{
			m_tapeVirtualMachine.setFlagRegister(HEX_1);
			valSet = true;
		}
		break;
	default:
		valSet = false;
		break;
	}

	if( !valSet )
	{
		m_tapeVirtualMachine.setFlagRegister(HEX_0);
	}
	FUNCTION_END;
}

void Interpreter::IFBlock()
{
	FUNCTION_BEGIN("Interpreter::IFBlock()");
	if(eIF == m_lex.getCurrentToken())
	{
		++_ifCount;
	}
	else if(eFI == m_lex.getCurrentToken()) 
	{//No need to give this condition again but just gave to safe guard and see that only FI brings control here.
		--_ifCount;
		if(_ifCount < 0)
			printf("Mis-matched FI\n");
		FUNCTION_END;
		return;
	}
	else
	{
		printf("Not IF block. Token mismatch somewhere\a\n");
		FUNCTION_END;
		return;
	}

	if(eLeftParen == m_lex.getNextToken())
	{
		switch(m_lex.getNextToken())
		{
		case eLessThan:// \<
		case eGreaterThan:// \>
		case eEquality:// \=
			compareWithNextBoolean();
			if( (m_lex.getNextToken() == eRightParen) )
			{
				const ulong flagVal = m_tapeVirtualMachine.getFlagRegister();

				if( !(HEX_1 ^ flagVal) )
				{
					m_lex.getNextToken();
					enmTokens tok = m_lex.getCurrentToken();//shakti del tok
					ExecuteSentence();
					tok = m_lex.getCurrentToken();//shakti del tok
				}
				else/* if(HEX_1 & m_tapeVirtualMachine.getFlagRegister())*/
				{
					while( (m_lex.getNextToken() != eFI) && (m_lex.getCurrentToken() != eTokenOver) )
						;//NULL while to eat up all characters till we get ]. NULL Statement()
					if(m_lex.getCurrentToken() == eTokenOver)
						printf("Abrupt end, no FI found for IF\n");
				}
			}
			else
			{
				printf("IF condition not ending properly. \"IF(<cond>\" found, right paren missing\n");
			}
			break;
		default:
			//set error TODO
			break;
		}
	}
	else
	{
		//set error - TODO
	}
	FUNCTION_END;
}

void Interpreter::compareWithNext()
{
	FUNCTION_BEGIN("Interpreter::compareWithNext()");
	if(m_tapeVirtualMachine.getCurrentHeadValue() < m_tapeVirtualMachine.getNthPosValFrmHead(1))
	{
		m_tapeVirtualMachine.setFlagRegister(HEX_1);
	}
	else if(m_tapeVirtualMachine.getCurrentHeadValue() > m_tapeVirtualMachine.getNthPosValFrmHead(1))
	{
		m_tapeVirtualMachine.setFlagRegister(HEX_2);
	}
	else
	{
		m_tapeVirtualMachine.setFlagRegister(HEX_3);
	}
	FUNCTION_END;
}

bool Interpreter::isBlankOrNull(const char* str)
{
	FUNCTION_BEGIN("Interpreter::isBlankOrNull(const char* str)");
	//For now only check string len, TODO check for blank too
	FUNCTION_END;
	return (strlen(str) > 0)? false : true;
}