#pragma once

#include "stdafx.h"

#define	LEXEMA_FIXSIZE   1	        //������ �������
#define	LT_MAXSIZE		 4096	    //������������ ���������� ����� � ������� ������
#define	LT_TI_NULLDX	 0xffffffff	//��� �������� ������� ���������������
#define LEX_SEPARATORS	 'S'
#define	LEX_ID_TYPE    	 't'			
#define	LEX_ID			 'i'			
#define	LEX_LITERAL		 'l'			
#define	LEX_FUNCTION     'f'			
#define	LEX_MAIN		 'm'			
#define	LEX_RETURN		 'r'			
#define	LEX_PRINT		 'p'			
#define LEX_DECLARE      'd'
#define	LEX_SEPARATOR	 ';'			
#define	LEX_COMMA		 ','			
#define	LEX_LEFTBRACE	 '{'			
#define	LEX_BRACELET	 '}'			
#define	LEX_LEFTHESIS	 '('			
#define	LEX_RIGHTTHESIS	 ')'			
#define	LEX_PLUS		 '+'			
#define	LEX_MINUS		 '-'	
#define LEX_STRLEN		 's'
#define LEX_SUBSTR		 'a'
#define	LEX_STAR		 '*'			
#define	LEX_DIRSLASH	 '/'			
#define	LEX_EQUAL		 '='			
#define	LEX_OPERATION	 'v'			
#define	INTEGER			 "integer"		
#define	STRING			 "string"
#define	LTRL			 "ltrl"

namespace LT	//������� ������
{
	struct Entry
	{
		char lexema;					//�������
		int sn;							//����� ������ � �������� ������
		int idxTI;						//������ � ������� ��������������� ��� LT_TI_NULLIDX
		
		Entry();
		Entry(char lexema, int snn, int idxti = LT_TI_NULLDX);
	};

	struct LexTable						//��������� ������� ������
	{
		int maxsize;					//������� ������� ������
		int size;						//������� ������ ������� ������
		Entry* table;					//������ (Entity)����� ������� ������
	};

	LexTable Create(int size);		            //������� < LT_MAXSIZE
	void Add(LexTable &lextable, Entry entry);	//��������� ������� ������, ������ ������� ������
}