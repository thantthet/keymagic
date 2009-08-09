#pragma once

#ifndef KLF_H_
#define KLF_H_

#define KMKL_VERSION 1
#define KMKL_SUBVERSION 3

#include <stdio.h>
#include <fstream>
#include <vector>

#include "../global/global.h"
#include "keymap.h"

#define opSTRING		0x00F0
#define opVARIABLE		0x00F1
#define opREFERENCE		0x00F2
#define opPREDEFINED	0x00F3
#define opMODIFIER		0x00F4
#define opANYOF			0x00F5
#define opAND			0x00F6

struct structRule{

	WORD* strInRule;
	WORD* strOutRule;

};

class Kmklf
{
public:

	Kmklf()
	{
		pFile = NULL;
	}

	Kmklf::~Kmklf()
	{
		if (pFile)
			fclose(pFile);
	}

	void add_rule(const wchar_t * in, const wchar_t * out)
	{
		wchar_t * in_rule_copy, * out_rule_copy;
		size_t in_rule_length, out_rule_length;
		structRule Rule;

		in_rule_length = wcslen( in );
		out_rule_length = wcslen( out );

		in_rule_copy = new wchar_t [ in_rule_length+1 ];
		out_rule_copy = new wchar_t [ out_rule_length+1 ];

		wcscpy ( in_rule_copy, in );
		wcscpy ( out_rule_copy, out );

		Rule.strInRule = (WORD*)in_rule_copy;
		Rule.strOutRule = (WORD*)out_rule_copy;
		rxRules.push_back( Rule );
	}

	bool add_str(const wchar_t * str)
	{
		wchar_t * str_copy;
		size_t  str_length;

		str_length = wcslen(str);

		str_copy = new wchar_t [ str_length+1 ];

		wcscpy(str_copy, str);

		if (find(str_copy) != -1)
			return false;

		strStrings.push_back(str_copy);
		return true;
	}

	int is_exist(wchar_t * str)
	{
		return find(str);
	}

	bool fromFile(const char * szPath)
	{
		int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szPath, -1, 0, 0);

		wchar_t * lpWideCharStr = new wchar_t[cchWideChar];

		MultiByteToWideChar(CP_ACP, 0, szPath, -1, lpWideCharStr, cchWideChar);

		bool ret = fromFile(lpWideCharStr);

		delete[] lpWideCharStr;

		return ret;
	}

	bool fromFile(const wchar_t * szPath)
	{
		FileHeader fh;
		FILE * hFile;

		hFile = _wfopen(szPath, L"rb");
		if (!hFile)
		{
			PrintLastError();
			return false;
		}

		fread(&fh, sizeof(FileHeader), 1, hFile);

		if (fh.cMagicCode[0] != 'K' ||
			fh.cMagicCode[1] != 'M' || 
			fh.cMagicCode[2] != 'K' || 
			fh.cMagicCode[3] != 'L') { return false; }

		strStrings.clear();
		rxRules.clear();

		for (int i = 0; i < fh.sNumOfString; i++)
		{
			short sLength;
			fread(&sLength, sizeof(short), 1, hFile);

			wchar_t * local_buf = new wchar_t[sLength+1];
			local_buf[sLength]='\0';
			fread(local_buf, sLength * sizeof(wchar_t), 1, hFile);
			
			strStrings.push_back(local_buf);
		}

		for (int i = 0; i < fh.sNumOfRules; i++)
		{
			structRule Rule;
			short sLength;

			fread(&sLength, sizeof(short), 1, hFile);
			wchar_t * in_rule = new wchar_t[sLength+1];
			in_rule[sLength]='\0';
			fread(in_rule, sLength * sizeof(wchar_t), 1, hFile);

			fread(&sLength, sizeof(short), 1, hFile);
			wchar_t * out_rule = new wchar_t[sLength+1];
			out_rule[sLength]='\0';
			fread(out_rule, sLength * sizeof(wchar_t), 1, hFile);
			
			Rule.strInRule = (WORD*)in_rule;
			Rule.strOutRule = (WORD*)out_rule;

			rxRules.push_back(Rule);
		}

		fclose(hFile);

		return true;
	}

	bool toFile(const wchar_t * szPath)
	{
		FileHeader fh;

		if (!create_file(szPath))
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
		fh.sNumOfString = strStrings.size();
		fh.sNumOfRules = rxRules.size();

		file_put(&fh, sizeof(FileHeader));
		
		for (std::vector<wchar_t *>::iterator it = strStrings.begin();
			it != strStrings.end();
			it++)
		{
			size_t len = wcslen(*it);
			file_put(&len, sizeof(short));
			file_put(*it, len * sizeof(wchar_t));
		}

		for (std::vector<structRule>::iterator it = rxRules.begin();
			it != rxRules.end();
			it++)
		{
			size_t inlen = wcslen ((wchar_t*)(*it).strInRule);
			file_put(&inlen, sizeof(short));
			file_put((*it).strInRule, inlen * sizeof(wchar_t));

			size_t outlen = wcslen ((wchar_t*)(*it).strOutRule);
			file_put(&outlen, sizeof(short));
			file_put((*it).strOutRule, outlen * sizeof(wchar_t));
		}

		fclose(pFile);

		return true;
	}

	std::vector<structRule> * getRules()
	{
		return &rxRules;
	}

	std::vector<wchar_t*> * getStrings()
	{
		return &strStrings;
	}



private:

	std::vector<wchar_t * > strStrings;
	std::vector<structRule> rxRules;

	FILE * pFile;

	bool create_file(const wchar_t * szPath)
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

	bool file_put(void * buffer, size_t size)
	{
		if (pFile)
			fwrite(buffer, 1, size, pFile);
		else
			return false;
		return true;
	}

	int find (wchar_t * str)
	{
		for ( int i =0 ; i < strStrings.size(); i++ )
		{
			if (!wcscmp(str, strStrings[i]))
				return i;
		}
		return -1;
	}

	int FileSize(const char* sFileName)
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
