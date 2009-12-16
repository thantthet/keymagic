#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#ifndef _PARSER_H
#define _PARSER_H

#pragma once

#include "../global/global.h"
#include "lexscanner.h"
#include "klf.h"

class parser
{

public:
	parser::parser(const wchar_t * in_s)
	{
		Script.loadSource(in_s);
		Scanner.setScript(&Script);
		tokens = Scanner.getTokens();
	}
	parser::parser(script s)
	{
		Scanner.setScript(&s);
		tokens = Scanner.getTokens();
	}
	parser::parser(vector<structToken> vtTokens)
	{
		tokens = vtTokens;
	}

	/*parser::~parser()
	{
	}*/

	void generate(wchar_t * szPath)
	{
		kmklf.toFile(szPath);
	}

	bool begin_parse();

public:
	wstring getLastError()
	{
		return LastError;
	}

private:
	struct ltwcstr
	{
	  bool operator()(const wchar_t* s1, const wchar_t* s2) const
	  {
		return wcscmp(s1, s2) < 0;
	  }
	};

	script Script;
	lexscanner Scanner;
	map<wchar_t*, wchar_t*, ltwcstr> mpVariables;
	//vector<wchar_t*> vars;
	vector<structToken> tokens;
	Kmklf kmklf;
	wstring LastError;

	LRESULT checkToken ( int * objIndex, emType Type );
	wchar_t * identifier( int * objIndex );
	void setVar(wchar_t * name, const wchar_t * value);
	bool newline( int * objIndex );
	bool vk_status(int * objIndex, wstring * outStr );
	bool combination(int * objIndex, wstring * outStr);
	bool condition( int * objIndex, wstring * outStr );
	bool context( int * objIndex, wstring * outStr );
	bool expression( int * objIndex );
	bool complexExpression( int * objIndex, wstring * value );
	bool vardeclaration( int * objIndex );
	bool pattern( int * objIndex, wstring * outStr );
	bool rule( int * objIndex );
	bool validVarName(wchar_t * name );
	int getVar(const wchar_t * value);
	//int find (wchar_t * value);
};

#endif