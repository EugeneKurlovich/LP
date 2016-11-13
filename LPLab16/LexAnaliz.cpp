#include "stdafx.h"
#include "Graphs.h"
#include "FST.h"
#include "LexAnaliz.h"
#include "Error.h"

namespace Lexer
{
	Graph graph[N_GRAPHS] =
	{
		{ LEX_LITERAL, FST::FST(GRAPH_INT_LITERAL) },
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },
		{ LEX_DIRSLASH, FST::FST(GRAPH_FORWARD_SLASH) },
		{ LEX_DECLARE, FST::FST(GRAPH_DECLARE) },
		{ LEX_STRLEN, FST::FST(GRAPH_STRLEN) },
		{ LEX_SUBSTR, FST::FST(GRAPH_SUBSTR) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_MAIN, FST::FST(GRAPH_MAIN) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_INTEGER) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_STRING) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_RETURN, FST::FST(GRAPH_RETURN) },
		{ LEX_PRINT, FST::FST(GRAPH_PRINT) },
		{ LEX_ID, FST::FST(GRAPH_ID) }
	};

	LEX Fill_LTIT(In::IN &InStruct, Log::LOG &log, Parm::PARM &parm)
	{
		LEX Tables;
		Tables.Lextable = LT::Create(LT_MAXSIZE);
		Tables.IDtable = IT::Create(TI_MAXSIZE);

		bool isParam = false;
		int funcBody = NULL;
		char tempTokenFunction[ID_MAXSIZE] = ""; 
		char tempToken[ID_MAXSIZE] = "";		 

		IT::IDDATATYPE funcType;
		IT::IDDATATYPE idType;

		for (int i = 0; i < In::StToken::tokensSize; i++)
		{
			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fstex(InStruct.tokens[i].token, graph[j].graph);
				if (FST::execute(fstex))
				{
					switch (graph[j].lexema)
					{
						case LEX_MAIN:
						{
							funcBody++;
							if (funcBody > 1)
								Log::writeError(log.stream, Error::GetError(113, InStruct.tokens[i].line, NULL));
							
							funcType = graph[j].graph.type;
							strncpy_s(tempTokenFunction, "", 1);
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);
							LT::Add(Tables.Lextable, entrylt);
							break;
						}
						case LEX_ID:
						{
							strncpy_s(tempToken, InStruct.tokens[i].token, ID_MAXSIZE - 1);
							strncpy_s(InStruct.tokens[i].token, InStruct.tokens[i].token, ID_MAXSIZE - 1);
							strcat_s(InStruct.tokens[i].token, tempTokenFunction);
							if (IT::IsId(Tables.IDtable, InStruct.tokens[i].token) == -1)
							{
								if (Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION)
								{
									strncpy_s(tempTokenFunction, tempToken, ID_MAXSIZE - 1);
									funcType = idType;
									IT::Entry entryit(tempTokenFunction, i, idType, IT::F);
									IT::Add(Tables.IDtable, entryit);
									LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token));
									LT::Add(Tables.Lextable, entrylt);
									isParam = true;
									break;
								}
								else if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE)
								{
									if (isParam == false)
									{
										IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::V);
										IT::Add(Tables.IDtable, entryit);
									}
									else if (isParam == true)
									{
										IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::P);
										IT::Add(Tables.IDtable, entryit);
										if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTTHESIS)
											isParam = false;
									}
									LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token));
									LT::Add(Tables.Lextable, entrylt);
									break;
								}
								Log::writeError(log.stream, Error::GetError(108, InStruct.tokens[i].line, NULL));
							}
							else
							{
								if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE || Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION)
								{
									Log::writeError(log.stream, Error::GetError(107, InStruct.tokens[i].line, NULL));
								}
								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token));
								LT::Add(Tables.Lextable, entrylt);
								break;
							}
							break;
						}
						case LEX_LITERAL:
						{
							switch (Tables.Lextable.table[i - 1].lexema)
							{
							case LEX_EQUAL:
							{
								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);
								LT::Add(Tables.Lextable, entrylt);
								if (Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::F)
									Log::writeError(log.stream, Error::GetError(115, InStruct.tokens[i].line, NULL));
								if (Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::V ||
									Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::P)
									IT::SetValue(Tables.IDtable, IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token), InStruct.tokens[i].token);
								break;
							}
							case LEX_LEFTHESIS:
							case LEX_COMMA:
							case LEX_PRINT:
							case LEX_RETURN:
							{
								IT::Entry entryit(LTRL, i, graph[j].graph.type, IT::L);
								IT::Add(Tables.IDtable, entryit);
								IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token);
								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1);
								LT::Add(Tables.Lextable, entrylt);
								break;
							}
							default:
								Log::writeError(log.stream, Error::GetError(116, InStruct.tokens[i].line, NULL));
							}
							break;
						}
						case LEX_SEPARATORS:
						{
							switch (InStruct.tokens[i].token[0])
							{
								case LEX_SEPARATOR:
								{
									if (Tables.Lextable.table[i - 2].lexema == LEX_RETURN)
									{
										IT::IDDATATYPE retType;
										if (Tables.Lextable.table[i - 1].lexema == LEX_LITERAL)
											retType = Tables.IDtable.table[Tables.IDtable.size - 1].iddatatype;
										else 
											retType = Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 1].token)].iddatatype;
										if (funcType != retType)
											Log::writeError(log.stream, Error::GetError(114, InStruct.tokens[i].line, NULL));
									}
								}
								default:
								{     
									LT::Entry entrylt(InStruct.tokens[i].token[0], InStruct.tokens[i].line);
									LT::Add(Tables.Lextable, entrylt);
								}
							}
							break;
						}
						case LEX_ID_TYPE:
							idType = graph[j].graph.type;
						default:
						{
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);
							LT::Add(Tables.Lextable, entrylt);
							break;
						}
					}
					break; 
				}
			}
		}

		if (!funcBody)
		{
			Log::writeError(log.stream, Error::GetError(106));
		}

		return Tables;
	}
}