#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#ifndef _SCRIPT_H
#define _SCRIPT_H

#pragma once

#include "common.h"

using namespace std;

#define EOS 0xFFFF

class script
{
public:

	script::script (){};

	script::script (wchar_t * Script)
	{
		loadSource(Script);
	}

	script::~script ()
	{
		if (s)
			delete [] s;
		s = NULL;
		stLinePos.~vector();
	}

	void copy(script * src)
	{
		size_t input_length = wcschr(src->s, EOS) - src->s;
		s = new wchar_t[input_length+2];
		wcsncpy(s, src->s, input_length);
		s[input_length] = EOS;
		s[input_length+1] = NULL;
		stLinePos = src->stLinePos;
	}

	void loadSource(wchar_t * script)
	{
		size_t input_length = wcslen(script);
		s = new wchar_t[input_length+2];
		wcscpy(s, script);
		s[input_length] = EOS;
		s[input_length+1] = NULL;
		parse_lines();
	}

	int getLineNum(int pos)
	{
		int i ;
		for (i = 0; i < stLinePos.size() && stLinePos[i] < pos; i++){}
		return i;
	}

	int getPosLine(int pos)
	{
		return pos - stLinePos[getLineNum(pos)-1];
	}

	wchar_t wCharAt(int index)
	{
		if (index > spt_length)
			return NULL;
		return s[index];
	}

	wchar_t * lpwStrAt(int index)
	{
		if (index > spt_length)
			return NULL;
		return &s[index];
	}

private:
	wchar_t * s;
	vector<int> stLinePos;
	size_t spt_length;

	void parse_lines()
	{
		int i = 0;

		stLinePos.push_back(i);

		while (s[i] != EOS)
		{
			if (s[i] == '\n')
			{
				i++;
				stLinePos.push_back(i);
			}
			else {i++;}
		}

		stLinePos.push_back(i);

		spt_length = i;
	}
};

#endif