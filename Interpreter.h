#ifndef __BRAINLESS_INTERPRETER__
#define __BRAINLESS_INTERPRETER__
/*
Author : Shakti Misra
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/
#include "VMInterface.h"
#include "TapeMachine.h"
#include "Lexer.h"
#include <map>
#include <string>

using namespace std;

typedef map<string, ulong> symTable;
/*This class starts the interpretation of the instructions. This will find out the meaning and interprete.*/
class Interpreter
{
private:
	Lex m_lex;//We will need a lexer to get the tokens from the source file
	TapeMachine<ulong, IOInterface, TAPE_SIZE> m_tapeVirtualMachine;//We will interprete the meaning of the above tokens and we will invoke the proper function in the VM
	const string m_sFileName;

	symTable m_funSymTable;

	int _ifCount;

public:
	Interpreter(string sFileName, string sTokenFile = ""):
	  m_sFileName(sFileName),
		  m_lex(sFileName),
		  _ifCount(0)
	  {
	  }

	  ~Interpreter()
	  {
	  }

	  void interpreteScript();

private:
	void ExecuteSentence();
	bool isBlankOrNull(const char*);

	void compareWithNext();
	void compareWithNextBoolean();
	void IFBlock();
	void Sentence();
	void Block();
	void DOLoop();
	void intDisplayContent();
};

#endif //__BRAINLESS_INTERPRETER__