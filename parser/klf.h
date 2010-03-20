#pragma once

#ifndef KLF_H_
#define KLF_H_

#define KMKL_VERSION 1
#define KMKL_SUBVERSION 3

#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "../global/global.h"
#include "keymap.h"

#define opSTRING		0x00F0
#define opVARIABLE		0x00F1
#define opREFERENCE		0x00F2
#define opPREDEFINED	0x00F3
#define opMODIFIER		0x00F4
#define opANYOF			0x00F5
#define opAND			0x00F6
#define opNANYOF		0x00F7
#define opANY			0x00F8
#define opSWITCH		0x00F9

struct structRule{

	WORD* strInRule;
	WORD* strOutRule;

};

class kmKLF
{
public:

	kmKLF(){
		pFile = NULL;
		layout.autoBksp = false;
		layout.eat = true;
		layout.trackCaps = true;
		layout.posBased = true;
	}

	kmKLF::~kmKLF(){
		if (pFile)
			fclose(pFile);
	}

	void newDebugBreak(size_t idx)
	{
		debugBreaks.push_back(idx);
	}

	BOOL isDebugBreak(size_t idx)
	{
		std::list<int>::iterator it;
		for ( it=debugBreaks.begin() ; it != debugBreaks.end(); it++ )
			if (idx == *it)
				return true;
		return false;
	}

	const wchar_t* debugBreaksToString()
	{
		std::list<int>::iterator it;
		std::wstringstream st ;

		for ( it=debugBreaks.begin() ; it != debugBreaks.end(); it++ )
			st << *it << L",";
		st.get();
		std::wstring * str = new std::wstring(st.str());
		return str->c_str();
	}

	void debugBreaksFromString(const wchar_t * strIdx)
	{
		std::vector<std::wstring> idxs;
		std::vector<std::wstring>::iterator it;

		boost::split(idxs, strIdx, boost::is_any_of(TEXT(",")));

		for ( it=idxs.begin() ; it != idxs.end(); it++ )
			debugBreaks.push_back(_wtoi((*it).c_str()));

		if (debugBreaks.back() == 0)
			debugBreaks.pop_back();
	}

	size_t newRule(const wchar_t * in, const wchar_t * out)
	{
		wchar_t * inRuleCopy, * outRuleCopy;
		size_t inRuleSize, outRuleSize;
		structRule newRule;

		inRuleSize = wcslen( in );
		outRuleSize = wcslen( out );

		inRuleCopy = new wchar_t [ inRuleSize+1 ];
		outRuleCopy = new wchar_t [ outRuleSize+1 ];

		wcscpy ( inRuleCopy, in );
		wcscpy ( outRuleCopy, out );

		newRule.strInRule = (WORD*)inRuleCopy;
		newRule.strOutRule = (WORD*)outRuleCopy;
		rxRules.push_back( newRule );

		return rxRules.size();
	}

	bool newString(const wchar_t * str)
	{
		wchar_t * strCopy;
		size_t  lenStr;

		lenStr = wcslen(str);
		strCopy = new wchar_t [ lenStr+1 ];
		wcscpy(strCopy, str);

		if (find(strCopy) != -1)
			return false;

		strStrings.push_back(strCopy);
		return true;
	}

	int searchInStrings(const wchar_t * str)
	{
		return find(str);
	}

	bool fromFile(const char * szPath)
	{
		int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szPath, -1, 0, 0);

		wchar_t * lpWideCharStr = new wchar_t[cchWideChar];

		MultiByteToWideChar(CP_ACP, 0, szPath, -1, lpWideCharStr, cchWideChar);

		bool ret = fromFile(lpWideCharStr);

		//delete[] lpWideCharStr;

		return ret;
	}

	bool fromFile(const wchar_t * szPath)
	{
		FileHeader fh;
		FILE * hFile;

		hFile = _wfopen(szPath, L"rb");
		if (!hFile){
			PrintLastError();
			return false;
		}

		fread(&fh, sizeof(FileHeader), 1, hFile);

		if (fh.cMagicCode[0] != 'K' ||
			fh.cMagicCode[1] != 'M' || 
			fh.cMagicCode[2] != 'K' || 
			fh.cMagicCode[3] != 'L') { return false; }

		layout = fh.layout;

		strStrings.clear();
		rxRules.clear();

		for (int i = 0; i < fh.shNumOfString; i++)
		{
			short sLength;
			fread(&sLength, sizeof(short), 1, hFile);

			wchar_t * local_buf = new wchar_t[sLength+1];
			local_buf[sLength]='\0';
			fread(local_buf, sLength * sizeof(wchar_t), 1, hFile);
			
			strStrings.push_back(local_buf);
		}

		debugBreaksFromString(strStrings.back());

		for (int i = 0; i < fh.shNumOfRules; i++)
		{
			structRule Rule;
			short sLength;

			fread(&sLength, sizeof(short), 1, hFile);
			wchar_t * ruleBinaryIn = new wchar_t[sLength+1];
			ruleBinaryIn[sLength]='\0';
			fread(ruleBinaryIn, sLength * sizeof(wchar_t), 1, hFile);

			fread(&sLength, sizeof(short), 1, hFile);
			wchar_t * ruleBinaryOut = new wchar_t[sLength+1];
			ruleBinaryOut[sLength]='\0';
			fread(ruleBinaryOut, sLength * sizeof(wchar_t), 1, hFile);
			
			Rule.strInRule = (WORD*)ruleBinaryIn;
			Rule.strOutRule = (WORD*)ruleBinaryOut;

			rxRules.push_back(Rule);
		}

		fclose(hFile);

		return true;
	}

	bool toFile(const wchar_t * szPath)
	{
		FileHeader fh;

		strStrings.push_back(debugBreaksToString());

		if (!createFile(szPath))
			return false;

		//magic bytes of
		//KeyMagic Keyboard Layout
		//KMKL
		fh.cMagicCode[0] = 'K';
		fh.cMagicCode[1] = 'M';
		fh.cMagicCode[2] = 'K';
		fh.cMagicCode[3] = 'L';

		fh.bMajorVersion = KMKL_VERSION;
		fh.bMinorVersion = KMKL_SUBVERSION;
		fh.shNumOfString = strStrings.size();
		fh.shNumOfRules = rxRules.size();

		fh.layout = layout;

		filePut(&fh, sizeof(FileHeader));
		
		for (std::vector<const wchar_t *>::iterator it = strStrings.begin();
			it != strStrings.end();
			it++)
		{
			size_t len = wcslen(*it);
			filePut(&len, sizeof(short));
			filePut((void*)*it, len * sizeof(wchar_t));
		}

		for (std::vector<structRule>::iterator it = rxRules.begin();
			it != rxRules.end();
			it++)
		{
			size_t inlen = wcslen ((wchar_t*)(*it).strInRule);
			filePut(&inlen, sizeof(short));
			filePut((*it).strInRule, inlen * sizeof(wchar_t));

			size_t outlen = wcslen ((wchar_t*)(*it).strOutRule);
			filePut(&outlen, sizeof(short));
			filePut((*it).strOutRule, outlen * sizeof(wchar_t));
		}

		fclose(pFile);

		return true;
	}

	std::vector<structRule> * getRules()
	{
		return &rxRules;
	}

	std::vector<const wchar_t*> * getStrings()
	{
		return &strStrings;
	}

	layout_options layout;

private:

	std::vector<const wchar_t * > strStrings;
	std::vector<structRule> rxRules;
	std::list<int> debugBreaks;

	FILE * pFile;

	bool createFile(const wchar_t * szPath)
	{

		if (!pFile)
			pFile = _wfopen(szPath, L"wb");

		if (pFile == NULL)
		{
			PrintLastError();
			return false;
		}
		return true;
	}

	bool filePut(void * buffer, size_t size)
	{
		if (pFile)
			fwrite(buffer, 1, size, pFile);
		else
			return false;
		return true;
	}

	int find (const wchar_t * str)
	{
		for ( int i =0 ; i < strStrings.size(); i++ ){
			if (!wcscmp(str, strStrings[i]))
				return i;
		}
		return -1;
	}

	int fileSize(const char* sFileName)
	{
	  std::ifstream f;
	  f.open(sFileName, std::ios_base::binary | std::ios_base::in);
	  if (!f.good() || f.eof() || !f.is_open()) { return 0; }
	  f.seekg(0, std::ios_base::beg);
	  std::ifstream::pos_type begin_pos = f.tellg();
	  f.seekg(0, std::ios_base::end);
	  return static_cast<int>(f.tellg() - begin_pos);
	}
};

#endif // KLF_H_
