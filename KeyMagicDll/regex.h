#ifndef __REGEX_H__
#define __REGEX_H__

#define MAXCHARINSQUAREBRACKET 10 /* [abcdef] */
#define MAXSOURCESTRLEN 10000 /* Length of Input Str to match */	
#define MAXMATCHSTRLEN 50 /* Length of ^01234[abcdef]ghijkl$ */
#define MAXGROUP 10		/* \0, \1 - \9 */
#define MAXRESULT 10
#define MAXREPEAT 200	/* for + * char,  dont need infinity or very large match */
#define MAXCHARCODE 65535

#include <wchar.h>

struct Burglish_Regex_Range{
	int start;
	int end;
};

struct Burglish_Regex_Char{
	int length;
	Burglish_Regex_Range range[MAXCHARINSQUAREBRACKET];
	int repeatMin; /* for ? + * */
	int repeatMax; /* ? + * */
	int group; /* (...) */
	bool eaten;
	bool invert;
};

struct Burglish_Regex{
	int length;/* length of regex source string */
	Burglish_Regex_Char ch[MAXMATCHSTRLEN];
	bool mustStart; /* ^ */
	bool mustEnd; /* $ */
};

struct Burglish_Regex_Match{
	int length;
	Burglish_Regex_Range range[MAXGROUP];
};

struct Burglish_Regex_Results{
	int length;
	Burglish_Regex_Match res[MAXRESULT];
	wchar_t* pointer;
};

/* regex headers */
class Regex{
	public:
		Regex(wchar_t* pattern, bool global=false, bool greedy=false);
		~Regex();
		void compile();
		bool test(wchar_t* srcStr);
		void sub(wchar_t*srcStr, wchar_t* replStr, wchar_t* destStr);
		wchar_t* pattern;
		bool global;
		bool greedy;
	private:
		bool exec(wchar_t* srcStr);
		int check(int chIdx, wchar_t* inputStr, int strIdx);
		Burglish_Regex r;/* compiled regex */
		Burglish_Regex_Results gr;/* executed group result */
		wchar_t buffer[MAXSOURCESTRLEN];
		bool error;
};

#ifdef _DEBUG
void test_block();
#endif

#endif //end define __REGEX_H__
