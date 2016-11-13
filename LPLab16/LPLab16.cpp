#include "stdafx.h"
#include "In.h"
#include "LexAnaliz.h"
#include "Log.h"
#include "Error.h"
#include "Parm.h"
#include "IT.h"
#include "GRB.h"
#include "MFST.h"
#include "LT.h"
#include "FST.h"
#include "PolishNotation.h"

#pragma comment(lib, "..\\Debug\\SystemInformation.lib")

int _tmain(int argc, wchar_t *argv[])
{
	setlocale(LC_CTYPE, "Russian");
	Log::LOG log;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv); 
		log = Log::getstream(parm.log);
		Log::writeLog(log); 
		Log::writeParm(log, parm);
		In::IN in = In::getin(log.stream, parm.in); 
		in.tokens = In::parsOfToken(in.tokens, in.text, in.code); 
		Lexer::LEX tables = Lexer::Fill_LTIT(in, log, parm); 

		if (Log::Eror) 
		{
			Log::writeIn(log, in);
			if (parm.LTtable == true)
				Log::writeLexTable(log, tables.Lextable);
			if (parm.ITtable == true)
				Log::writeIDtable(log, tables.IDtable);
			Log::writeIntermediateCode(log, tables.Lextable);

			MFST_TRACE_START(log);  
			MFST::Mfst mfst(tables, GRB::getGreibach()); 
			mfst.start(log);
			mfst.savededucation(); //сохранить правила вывода
			if (parm.Rules == true)
			    mfst.printrules(log); 
		}

		PN::CallPolishNotation(&tables.Lextable, &tables.IDtable);
		Log::writeLexTable(log, tables.Lextable);
		std::cout << "¬ыполнено успешно!" << std::endl;
		Log::Close(log);

	}
	catch (Error::ERROR e)
	{
		Log::writeError(log.stream, e);
	}
}