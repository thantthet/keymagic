#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#ifndef _LEXSCANNER_H
#define _LEXSCANNER_H

#pragma once

#include <boost/regex.hpp>
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
		scpt = s_input;
		scan();
	}

	void lexscanner::setScript(script * s_input)
	{
		scpt = s_input;
		scan();
	}

	vector<structToken> getTokens()
	{
		return tokens;
	}

private:
	vector<structToken> tokens;
	wchar_t * wPlus, * wASSIGN, * wPRINT, * wAND , * wStar, * wLess, * wGreater;
	script scpt;

	void init_vars()
	{
		wPlus = L"+";
		wASSIGN = L"=";
		wPRINT = L"=>";
		wAND = L"&";
		wStar = L"*";
		wLess = L"<";
		wGreater = L">";
	}

	bool isNewLine(int * Index)
	{
		structToken kToken;
		int scannedIndex = *Index;

		if (scpt.wCharAt(scannedIndex) == '\r')
			scannedIndex++;

		if (scpt.wCharAt(scannedIndex) == '\n')
		{
			kToken.iLength = 1;
			kToken.iStartIndex = scannedIndex;
			kToken.Type = T_NEWLINE;
			kToken.Value = L"\n";
			tokens.push_back(kToken);

			DumpToken(L"New Object Assigned:", kToken);

			scannedIndex++;

			if (scpt.wCharAt(scannedIndex) == '\r' || scpt.wCharAt(scannedIndex) == '\n')
				scannedIndex++;

			isNewLine(&scannedIndex);
			*Index = scannedIndex;
			return true;
		}
		return false;
	}

	bool isPreDefined(int * Index, int length)
	{
		structToken kToken;
		int scannedIndex = *Index;

		structPREdef * preDef;
		
		if (preDef = getPreDef(scpt.lpwStrAt(scannedIndex), length, true))
		{
			kToken.iLength = 4;
			kToken.iStartIndex = scannedIndex;
			kToken.Type = T_PREDEFINED;
			kToken.Value = preDef->name;
			tokens.push_back(kToken);
			scannedIndex+=length;
			*Index = scannedIndex;
			return true;
		}

		return false;
	}

	bool scan ()
	{
		int scannedIndex = 0, lineNum=1;

		bool noAdd = false;

		while (scpt.wCharAt(scannedIndex) != EOS || !scpt.wCharAt(scannedIndex))
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

			while (scpt.wCharAt(scannedIndex) == ' ' || scpt.wCharAt(scannedIndex) == '\t') {scannedIndex++; }

			switch (scpt.wCharAt(scannedIndex))
			{
			case '\\':
				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_BSLASH;
				kToken.Value = L"\\";

				tokens.push_back(kToken);
				scannedIndex++;

				DumpToken(L"New Object Assigned:", kToken);
				break;
			case '\r':
			case '\n':
				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_NEWLINE;
				kToken.Value = L"\n";
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);

				scannedIndex++;

				if (scpt.wCharAt(scannedIndex) == '\r' || scpt.wCharAt(scannedIndex) == '\n')
					scannedIndex++;
				break;
			case '[':
				if (scpt.wCharAt(scannedIndex+2) == ']')
				{
					wchar_t* wModer = new wchar_t[2];
					wModer[0]=scpt.wCharAt(scannedIndex+1);
					wModer[1]=0;

					kToken.iLength = 3;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_MODIFIER;
					kToken.Value = wModer;
					tokens.push_back(kToken);

					scannedIndex += 3;
				}
				else if (scpt.wCharAt(scannedIndex+3) == ']')
				{
					wchar_t* wModer = new wchar_t[3];
					wModer[0]=scpt.wCharAt(scannedIndex+1);
					wModer[1]=scpt.wCharAt(scannedIndex+2);
					wModer[2]=0;

					kToken.iLength = 4;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_MODIFIER;
					kToken.Value = wModer;
					tokens.push_back(kToken);

					scannedIndex += 4;
				}
				else
				{
					Exit(0, L"ERROR: Parse error => Line: %d Pos: %d\n", scpt.getLineNum(scannedIndex+3), scpt.getPosLine(scannedIndex+3));
				}
				break;
			case '$':
				retlen = reference(scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
				{
					wchar_t * sEnd = WholeWord(scpt.lpwStrAt(scannedIndex+1));
					if (sEnd == scpt.lpwStrAt(scannedIndex))
						return false;
					scannedIndex++;
					int wlen = sEnd - scpt.lpwStrAt(scannedIndex);

					wchar_t * wNew = new wchar_t[wlen+1];
					wcsncpy(wNew, scpt.lpwStrAt(scannedIndex), wlen);
					wNew[wlen] = 0;

					kToken.iLength = wlen;
					kToken.iStartIndex = scannedIndex;
					kToken.Type = T_IDENTIFIER;
					kToken.Value = wNew;

					tokens.push_back(kToken);
					scannedIndex += wlen;

					DumpToken(L"New Object Assigned:", kToken);
				}
				break;
			case '(':
				noAdd = true;

				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_SWITCH_START;
				kToken.Value = L"(";
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);
				scannedIndex++;

				break;
			case ')':
				noAdd = false;

				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_SWITCH_END;
				kToken.Value = L")";
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);
				scannedIndex++;

				break;
			case '<':
				noAdd = true;

				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_COMBINE_START;
				kToken.Value = wLess;
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);
				scannedIndex++;

				break;
			case '>':
				noAdd = false;

				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_COMBINE_END;
				kToken.Value = wGreater;
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);
				scannedIndex++;

				break;
			case '+':
				Debug(L"+");
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
					Exit(0, L"ERROR: Unexpected '%s' => Line: %d Pos: %d\n", wPlus, scpt.getLineNum(scannedIndex), scpt.getPosLine(scannedIndex));
				}
				break;
			case '&':
				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_ANDOP;
				kToken.Value = wAND;
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);

				scannedIndex++;
				break;
			case '*':
				kToken.iLength = 1;
				kToken.iStartIndex = scannedIndex;
				kToken.Type = T_ANY;
				kToken.Value = wStar;
				tokens.push_back(kToken);

				DumpToken(L"New Object Assigned:", kToken);

				scannedIndex++;
				break;
			case '=':
				if (scpt.wCharAt(scannedIndex+1) == '>')
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
			case 'U':
			case 'u':
				retlen = unicode(scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
					return false;
				break;
			case '"':
			case '\'':
				retlen = parse_string (scannedIndex);
				if (retlen > 0)
					scannedIndex += retlen;
				else
					return false;
				break;
			case '/':
				if (!isComment(&scannedIndex))
					return false;
				break;
			default:
				wchar_t * end = WholeWord(scpt.lpwStrAt(scannedIndex));
				int wwlength = end - scpt.lpwStrAt(scannedIndex);
				if (!wwlength)
					Exit(0, L"ERROR: Parse error => Line: %d Pos: %d\n", scpt.getLineNum(scannedIndex), scpt.getPosLine(scannedIndex));
				wchar_t * wholeWord = new wchar_t [wwlength];
				wholeWord[wwlength] = 0;

				wcsncpy(wholeWord, scpt.lpwStrAt(scannedIndex), wwlength);

				if (isPreDefined(&scannedIndex, wwlength))
					break;
				
				Exit(0, L"ERROR: Parse error '%s' => Line: %d Pos: %d\n", wholeWord, scpt.getLineNum(scannedIndex), scpt.getPosLine(scannedIndex));
				break;
			}
		}

		return true;
	}

	wchar_t * WholeWord(wchar_t * s)
	{
		while (*s != EOS){
			if ((*s < '0' || *s > '9') && (*s < 'A' || *s > 'Z') && (*s < 'a' || *s > 'z') && *s != '_')
				break;
			s++;
		}
		
		return s;
	}

	int reference (int scannedIndex)
	{
		scannedIndex++;

		wchar_t * sEnd = WholeWord(scpt.lpwStrAt(scannedIndex));
		if (!sEnd)
			return false;

		for (wchar_t * w = sEnd-1; w >= scpt.lpwStrAt(scannedIndex); w--)
		{
			if (!isdigit(*w))
			{
				return false;
			}
		}
		
		int wlen = sEnd - scpt.lpwStrAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, scpt.lpwStrAt(scannedIndex), wlen);
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

	int unicode (int scannedIndex)
	{
		scannedIndex++;

		wchar_t * sEnd = WholeWord(scpt.lpwStrAt(scannedIndex));
		if (!sEnd)
			return false;

		for (wchar_t * w = sEnd-1; w > scpt.lpwStrAt(scannedIndex); w--)
		{
			if (!isxdigit(*w)){
				return false;
			}
		}
		
		int wlen = sEnd - scpt.lpwStrAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, scpt.lpwStrAt(scannedIndex), wlen);
		wNew[wlen] = 0;

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

	bool isComment( int * index)
	{
		int scannedIndex = *index;
		if (scpt.wCharAt(scannedIndex) == '/' && scpt.wCharAt(scannedIndex+1) == '/')
		{
			wchar_t * lineEnd = wcspbrk(scpt.lpwStrAt(scannedIndex), L"\r\n");
			if (lineEnd)
				(*index) = lineEnd - scpt.lpwStrAt(0);
			else
			{
				(*index) = scpt.length();
			}
			return true;
		}
		else if (scpt.wCharAt(scannedIndex) == '/' && scpt.wCharAt(scannedIndex+1) == '*')
		{
			scannedIndex++;
			wchar_t * commentEnd = wcsstr(scpt.lpwStrAt(scannedIndex), L"*/");
			if (commentEnd)
			{
				(*index) = (commentEnd+2) - scpt.lpwStrAt(0);
				return true;
			}
		}
		Exit(0, L"/ cannot be parsed");
		return false;
	}

	int parse_string (int scannedIndex)
	{
		wchar_t * sEnd;

		wchar_t wcQuote =  scpt.wCharAt(scannedIndex);

		scannedIndex++;

		sEnd = scpt.lpwStrAt(scannedIndex);
		do {
			sEnd++;
			sEnd = wcschr(sEnd, wcQuote);
		} while (sEnd && *(sEnd-1) == '\\' && *(sEnd-2) != '\\');

		if (!sEnd)
			return -1;

		int wlen = sEnd - scpt.lpwStrAt(scannedIndex);
		wchar_t * wNew = new wchar_t[wlen+1];
		wcsncpy(wNew, scpt.lpwStrAt(scannedIndex), wlen);
		wNew[wlen] = 0;

		//static boost::wregex e(L"\\\\(.)");
		//wcscpy(wNew, boost::regex_replace(std::wstring(wNew), e, std::wstring(L"$1")).c_str());
		std::wstring unescaped_str = std::wstring();
		for (int i=0; i < wlen; i++){
			if (wNew[i] == '\\'){
				i++;
				int j=0;
				switch (wNew[i]){
					case 'x':
					case 'u':
					case 'X':
					case 'U':
						i++;
						while (ishexa(wNew[i])) i++,j++;
						if (j) {
							WORD uni[2] = {0};
							swscanf(&wNew[i-j], L"%x", uni);
							unescaped_str.push_back(uni[0]);
							i--;
						}
						else {
							Exit(0, L"ERROR: Parse error => Line: %d Pos: %d\n", scpt.getLineNum(scannedIndex+i), scpt.getPosLine(scannedIndex+i));
						}
						break;
					default:
						unescaped_str.push_back(wNew[i]);
				}
			}
			else {
				unescaped_str.push_back(wNew[i]);
			}
		}

		wcscpy(wNew, unescaped_str.c_str());

		structToken kToken;
		kToken.iLength = unescaped_str.size();
		kToken.iStartIndex = scannedIndex;
		kToken.Type = T_STRING;
		kToken.Value = wNew;

		unescaped_str.clear();

		tokens.push_back(kToken);

		DumpToken(L"New Object Assigned:", kToken);

		return (wlen+2);
	}
};

#endif
