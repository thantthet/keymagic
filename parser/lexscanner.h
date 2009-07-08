#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#ifndef _LEXSCANNER_H
#define _LEXSCANNER_H

#pragma once

#include "../global/global.h"
#include "script.h"

class lexscanner
{
public:
	lexscanner::lexscanner()
	{
		init_vars();
	}

	lexscanner::lexscanner(script * s_input)
	{
		init_vars();
		s.copy(s_input);
		scan();
	}

	void lexscanner::setScript(script * s_input)
	{
		s.copy(s_input);
		scan();
	}

	vector<structToken> getTokens()
	{
		return tokens;
	}

private:
	vector<structToken> tokens;
	wchar_t * wPlus, * wASSIGN, * wPRINT, * wAND , * wStar;
	script s;

	void init_vars()
	{
		wPlus = L"+";
		wASSIGN = L"=";
		wPRINT = L"=>";
		wAND = L"&";
		wStar = L"*";
	}

	bool isNewLine(int * Index)
	{
		structToken kToken;
		int scannedIndex = *Index;

		if (s.wCharAt(scannedIndex) == '\r')
			scannedIndex++;

		if (s.wCharAt(scannedIndex) == '\n')
		{
			kToken.iLength = 1;
			kToken.iStartIndex = scannedIndex;
			kToken.Type = T_NEWLINE;
			kToken.Value = L"\n";
			tokens.push_back(kToken);

			DumpToken(L"New Object Assigned:", kToken);

			scannedIndex++;

			if (s.wCharAt(scannedIndex) == '\r')
				scannedIndex++;

			isNewLine(&scannedIndex);
			*Index = scannedIndex;
			return true;
		}
		return false;
	}

	bool isPreDefined(int * Index)
	{
		structToken kToken;
		int scannedIndex = *Index;

		structPREdef * preDef;
		if (preDef = getPreDef(s.lpwStrAt(scannedIndex), true))
		{
			kToken.iLength = 4;
			kToken.iStartIndex = scannedIndex;
			kToken.Type = T_PREDEFINED;
			kToken.Value = preDef->name;
			tokens.push_back(kToken);
			scannedIndex+=wcslen( preDef->name);
			*Index = scannedIndex;
			return true;
		}

		return false;
	}

	bool scan ()
	{
		int scannedIndex = 0, lineNum=1;

		bool noAdd = false;

		while (s.wCharAt(scannedIndex) != EOS || !s.wCharAt(scannedIndex))
		{
			structToken kToken;
			vector<structToken>::iterator it;
			vector<structToken>::reverse_iterator rit;
			int retlen;

			if (isNewLine(&scannedIndex))
			{
				noAdd = false;
				continue;
			}

			while (s.wCharAt(scannedIndex) == ' ' || s.wCharAt(scannedIndex) == '\t') {scannedIndex++; }

			switch (s.wCharAt(scannedIndex))
			{
			case '[':
				if (s.wCharAt(scannedIndex+2) == ']')
				{
					wchar_t* wModer = new wchar_t[2];
					wModer[1]=NULL;
					wModer[0]=s.wCharAt(scannedIndex+1);

					kToken.iLength = 3;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_MODIFIER;
					kToken.Value = wModer;
					tokens.push_back(kToken);

					scannedIndex += 3;
				}
				else
				{
					wchar_t str[50];
					swprintf(str, L"ERROR: Parse error => Line: %d Pos: %d\n", s.getLineNum(scannedIndex), s.getPosLine(scannedIndex));
					Debug(str);
					Exit(0);
				}
				break;
			case '$':
				retlen = reference(&s, scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
				{
					wchar_t * sEnd = WholeWord(s.lpwStrAt(scannedIndex+1));
					if (sEnd == s.lpwStrAt(scannedIndex))
						return false;
					scannedIndex++;
					int wlen = sEnd - s.lpwStrAt(scannedIndex);
					wchar_t * wNew = new wchar_t[wlen+1];
					wcsncpy(wNew, s.lpwStrAt(scannedIndex), wlen);
					wNew[wlen] = NULL;

					kToken.iLength = wlen;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_IDENTIFIER;
					kToken.Value = wNew;

					tokens.push_back(kToken);
					scannedIndex += wlen;

					DumpToken(L"New Object Assigned:", kToken);
				}
				break;
			case '+':

				if (!noAdd)
				{
					kToken.iLength = 1;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_ADDOP;
					kToken.Value = wPlus;
					tokens.push_back(kToken);

					DumpToken(L"New Object Assigned:", kToken);
					scannedIndex++;
				}
				else
				{
					wchar_t str[50];
					swprintf(str, L"ERROR: Unexpected '%s' => Line: %d Pos: %d\n", wPlus, s.getLineNum(scannedIndex), s.getPosLine(scannedIndex));
					Debug(str);
					Exit(0);
				}
				break;
			case '&':

				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_ANDOP;
				kToken.Value = wAND;
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);

				noAdd = true;

				scannedIndex++;
				break;
			case '=':
				if (s.wCharAt(scannedIndex+1) == '>')
				{
					kToken.iLength = 2;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_PRINT;
					kToken.Value = wPRINT;
					tokens.push_back(kToken);

					scannedIndex++;
				}
				else
				{
					kToken.iLength = 1;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_ASSIGN;
					kToken.Value = wASSIGN;
					tokens.push_back(kToken);
				}

				scannedIndex++;

				DumpToken(L"New Object Assigned:", kToken);
				
				break;
			case 'u':
				retlen = unicode(&s, scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
					return false;
				break;
			case '"':
			case '\'':
				retlen = parse_string (&s, scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
					return false;
				break;
			case '/':
				if (!isComment(&s, &scannedIndex))
					return false;
				break;
			default:
				if (isPreDefined(&scannedIndex))
					break;
				wchar_t str[50];
				swprintf(str, L"ERROR: Parse error => Line: %d Pos: %d\n", s.getLineNum(scannedIndex), s.getPosLine(scannedIndex));
				Debug(str);
				Exit(0);
				break;
			}
		}

		return true;
	}

	wchar_t * WholeWord(wchar_t * s)
	{
		while (*s != EOS){
			if ((*s < '0' || *s > '9') && (*s < 'A' || *s > 'Z') && (*s < 'a' || *s > 'z'))
				break;
			s++;
		}
		
		return s;
	}

	int reference (script * s,  int scannedIndex)
	{
		scannedIndex++;

		wchar_t * sEnd = WholeWord(s->lpwStrAt(scannedIndex));
		if (!sEnd)
			return false;

		for (wchar_t * w = sEnd-1; w >= s->lpwStrAt(scannedIndex); w--)
		{
			if (!isdigit(*w))
			{
				return false;
			}
		}
		
		int wlen = sEnd - s->lpwStrAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, s->lpwStrAt(scannedIndex), wlen);
		wNew[wlen] = NULL;

		structToken kToken;

		kToken.iLength = wlen;
		kToken.iStartIndex = scannedIndex;
		kToken.Type = T_REFERENCE;
		kToken.Value = wNew;

		tokens.push_back(kToken);

		DumpToken(L"New Object Assigned:", kToken);

		return wlen+1;
	}

	bool ishexa(wchar_t w)
	{
		if ((w >= '0' && w <= '9') || (w >= 'a' && w <= 'f') || (w >= 'A' && w <= 'F'))
			return true;
		return false;
	}

	int unicode (script * s,  int scannedIndex)
	{
		scannedIndex++;

		wchar_t * sEnd = WholeWord(s->lpwStrAt(scannedIndex));
		if (!sEnd)
			return false;

		for (wchar_t * w = sEnd-1; w > s->lpwStrAt(scannedIndex); w--)
		{
			if (!isxdigit(*w))
			{
				return false;
			}
		}
		
		int wlen = sEnd - s->lpwStrAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, s->lpwStrAt(scannedIndex), wlen);
		wNew[wlen] = NULL;

		swscanf(wNew, L"%x", wNew);

		structToken kToken;

		kToken.iLength = wlen;
		kToken.iStartIndex = scannedIndex;
		kToken.Type = T_STRING;
		kToken.Value = wNew;

		tokens.push_back(kToken);

		DumpToken(L"New Object Assigned:", kToken);

		return wlen+1;
	}

	bool isModedVar(wchar_t * keyword, int wlen, int * scannedIndex)
	{
		if (wlen < 5){return false;}
		
		if (keyword[wlen-1] != ']' || keyword[wlen-3] != '[')
		{
			return false;
		}

		int varlen = wlen - 3;

		wchar_t * varname = new wchar_t[varlen+1];
		wcsncpy(varname, keyword,  varlen);
		varname[varlen] = NULL;

		structToken kToken;

		kToken.iLength = varlen;
		kToken.iStartIndex = *scannedIndex;
		kToken.Type = T_IDENTIFIER;
		kToken.Value = varname;
		tokens.push_back(kToken);

		wchar_t* wModer = new wchar_t[2];
		wModer[1]=NULL;
		wModer[0]=keyword[wlen-2];

		kToken.iLength = 3;
		kToken.iStartIndex = (*scannedIndex) - varlen;
		kToken.Type = T_MODIFIER;
		kToken.Value = wModer;
		tokens.push_back(kToken);

		(*scannedIndex) += wlen;

		return true;
	}

	bool isComment(script * s, int * index)
	{
		int scannedIndex = *index;
		if (s->wCharAt(scannedIndex) == '/' && s->wCharAt(scannedIndex+1) == '/')
		{
			wchar_t * lineEnd = wcspbrk(s->lpwStrAt(scannedIndex), L"\r\n");
			(*index) = lineEnd - s->lpwStrAt(0);
			return true;
		}
		else if (s->wCharAt(scannedIndex) == '/' && s->wCharAt(scannedIndex+1) == '*')
		{
			scannedIndex++;
			wchar_t * commentEnd = wcsstr(s->lpwStrAt(scannedIndex), L"*/");
			if (commentEnd)
			{
				(*index) = (commentEnd+2) - s->lpwStrAt(0);
				return true;
			}
		}
		Debug(L"/ cannot be parsed");
		Exit(0);
		return false;
	}

	int parse_string (script * s, int scannedIndex)
	{
		wchar_t * sEnd;

		wchar_t wcQuote =  s->wCharAt(scannedIndex);

		scannedIndex++;

		do {
			sEnd = wcschr(s->lpwStrAt(scannedIndex), wcQuote);
		} while (sEnd && *(sEnd-1) == '\\');
		

		if (!sEnd)
			return -1;

		int wlen = sEnd - s->lpwStrAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, s->lpwStrAt(scannedIndex), wlen);
		wNew[wlen] = NULL;

		structToken kToken;
		kToken.iLength = wlen;
		kToken.iStartIndex = scannedIndex;
		kToken.Type = T_STRING;
		kToken.Value = wNew;

		tokens.push_back(kToken);

		DumpToken(L"New Object Assigned:", kToken);

		return (wlen+2);
	}
};

#endif