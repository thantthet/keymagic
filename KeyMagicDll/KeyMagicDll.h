// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the KEYMAGICDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// KEYMAGICDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef KEYMAGICDLL_EXPORTS
#define KEYMAGICDLL_API __declspec(dllexport)
#else
#define KEYMAGICDLL_API __declspec(dllimport)
#endif

#include <windows.h>
#include "../KeyMagic/KeyMagic.h"

//#define _DEBUG 1

extern UINT KM_CHAR;

struct KbFileHeader{
	DWORD Magic;
	WORD lvk;
	WORD luc;
	WORD nOrdData;
	WORD nComData;
};

struct KbData{
	wchar_t* vk;
	wchar_t* uc;
};

struct OrdData{
	const wchar_t Key[10];
	const wchar_t Data[10];
	wchar_t isEnd;
};

struct CombineData{
	const wchar_t Key;
	const wchar_t Data[5];
};

void KEYMAGICDLL_API HookInit(HWND hWnd,HHOOK hKbHook,HHOOK hWPHook, HHOOK hGMHook, 
							  UINT KM_SETKB_ID, UINT KM_KILL_FOCUS, LPCSTR ParentPath);
LRESULT KEYMAGICDLL_API CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT KEYMAGICDLL_API CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT KEYMAGICDLL_API CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam);

bool	OpenKbFile(int Index);
void	CloseMapping();

