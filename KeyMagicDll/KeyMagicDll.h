#ifdef KEYMAGICDLL_EXPORTS
#define KEYMAGICDLL_API __declspec(dllexport)
#else
#define KEYMAGICDLL_API __declspec(dllimport)
#endif

#include <windows.h>
#include "../KeyMagic/KeyMagic.h"

//#define _DEBUG 1

struct KbFileHeader{
	DWORD Magic;
	WORD lvk;
	WORD luc;
	WORD nOrdData;
	WORD nComData;
	WORD nUnKey;
	WORD RESERVE;
};

struct KbData{
	wchar_t* vk;
	wchar_t* uc;
};

struct OrdData{
	const wchar_t Key[10];
	const wchar_t Data[10];
	const wchar_t Method;
};

struct CombineData{
	const wchar_t Key;
	const wchar_t Data[5];
};

struct KM_ShortCut{
	UINT ukey;
	BYTE modkey;
};

struct UniqueKey{

	bool CTRL;
	bool L_ALT;
	bool R_ALT;
	bool SHIFT;

	char vKEY;
	wchar_t wChars[5];
};

void KEYMAGICDLL_API HookInit(HWND hWnd,HHOOK hKbHook,
							  HHOOK hWPHook, 
							  HHOOK hGMHook,
							  LPCSTR ParentPath);

LRESULT KEYMAGICDLL_API CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT KEYMAGICDLL_API CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT KEYMAGICDLL_API CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam);

bool	OpenKbFile(int Index);
void	CloseMapping();
void	GetShortCuts();

extern KM_ShortCut *SC;