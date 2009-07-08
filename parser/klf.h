#pragma once

#ifndef KLF_H_
#define KLF_H_

#include <stdio.h>
#include <fstream>
#include <vector>

#include "../global/global.h"
#include "keymap.h"

#define opSTRING	0x1111
#define opVARIABLE	0x2222
#define opREFERENCE	0x3333
#define opPREDEFINED	0x4444
#define opMODIFIER	0x5555
#define opANYOF		0x6666
#define opAND		0x7777

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
		if ( rxRules.size ( ) )
		{
			std::vector<structRule>::iterator it;

			for ( it=rxRules.begin( ) ; it < rxRules.end( ); it++ )
			{
				delete [ ] ( (*it).strInRule );
				delete [ ] ( (*it).strOutRule );
			}

			rxRules.~vector ( );
		}

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
		FileHeader fh;
		FILE * hFile;

		hFile = fopen(szPath, "rb");
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

	bool toFile(const char * szPath)
	{
		FileHeader fh;

		if (!create_file(szPath))
			return false;

		fh.cMagicCode[0] = 'K';fh.cMagicCode[1] = 'M';fh.cMagicCode[2] = 'K';fh.cMagicCode[3] = 'L';
		fh.bMajorVersion = 1;
		fh.bMinorVersion = 3;
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

	std::vector<structRule> getRules()
	{
		return rxRules;
	}

	std::vector<wchar_t*> getStrings()
	{
		return strStrings;
	}



private:

	std::vector<wchar_t * > strStrings;
	std::vector<structRule> rxRules;

	FILE * pFile;

	bool create_file(const char * szPath)
	{

		if (!pFile)
			pFile = fopen(szPath, "wb");

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
