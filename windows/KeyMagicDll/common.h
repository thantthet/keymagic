#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef _DEBUG
	#define _TRACK_WND_RPOC_
#endif

#include <windows.h>
#include <vector>
#include <Userenv.h>

#include "../parser/klf.h"
#include "InternalEditor.h"
#include <boost/regex.hpp>

extern HHOOK hKeyHook;
extern HHOOK hWndProcHook;
extern HHOOK hGetMsgHook;
extern HWND hwndKWindows;
extern hwndExc hwndExceptions;
extern TCHAR szDir[1000];

typedef std::vector<BYTE> VIRTUALKEYS;
typedef std::vector<int> SWITCHES;

struct KM_ShortCut {
	BYTE ukey;
	BYTE modkey;
};
typedef std::vector<KM_ShortCut> SHORTCUTS;

struct structCClass{
	int idx;
	const wchar_t * start;
	const wchar_t * end;
} ;
typedef std::vector<structCClass> CHARCLASSES;

struct structExpendedRule
{
	std::wstring*	match_pattern;
	VIRTUALKEYS*	vk;
	SWITCHES*		switches;
	int				estimated_length;
	CHARCLASSES		cc;
	boost::wregex*	regex;
	RULELIST::iterator	rule_it;

	bool operator< (structExpendedRule r1) {
		int diff = 0;
		if (switches->size() != r1.switches->size()) {
			diff = switches->size() - r1.switches->size();
			return diff < 0;
		}

		if (vk->size() != r1.vk->size()) {
			diff = vk->size() - r1.vk->size();
			return diff < 0;
		}

		if (estimated_length != r1.estimated_length) {
			diff = estimated_length - r1.estimated_length;
			return diff < 0;
		}

		return false;
	}
};
typedef std::list<structExpendedRule> EXPENDEDRULELIST;

extern EXPENDEDRULELIST vtERs;
extern SHORTCUTS vtSC;
extern classInternalEditor InternalEditor;

extern void GetShortCuts();
extern void Logger(TCHAR* fmt, ...);
extern LPCTSTR GetKeyBoard(UINT Index, TCHAR * szKBPath);
extern bool LoadKeymapFile(int index);
extern UINT TranslateToUnicode (WORD *uVKey, LPBYTE GlobalKeyStates);
extern kmKLF klf;

// add slash within character class
static boost::wregex slash(L"[][?(){}*+?.\\-\\\\|]");
static std::wstring slash_r(L"\\\\$0");

extern int findLastOpenBracket(std::wstring * s);
extern bool AppendVariableValue(int index, std::wstring * s);
extern bool makeRegex(std::wstring*,
					  VIRTUALKEYS*,
					  SWITCHES*,
					  WORD*,
					  int,
					  bool = true,
					  boost::wcmatch* = NULL,
					  CHARCLASSES* = NULL);

extern HKL hkl;

#endif
