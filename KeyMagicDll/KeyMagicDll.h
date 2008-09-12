#ifdef KEYMAGICDLL_EXPORTS
#define KEYMAGICDLL_API __declspec(dllexport)
#else
#define KEYMAGICDLL_API __declspec(dllimport)
#endif

#include <windows.h>

#define DEBUG 1

struct KbFileHeader{
	DWORD Magic;
	short One_Count;
	short Multi_Count;
	short Customize_Count;
	short Back_Count;
};

struct KM_ShortCut{
	UINT ukey;
	BYTE modkey;
};

struct One2One{
	const wchar_t *Input;
	const wchar_t *Output;
};

struct Internal_One2Multi{
	wchar_t Input_Key;
	bool CTRL;
	bool L_ALT;
	bool R_ALT;
	bool SHIFT;

	const wchar_t Output[1];
};

struct File_One2Multi{
	short size;
	Internal_One2Multi One2Multi;
};

struct File_Custom{
	short size_MatchPattern;
};

struct File_Delete{
	short size_MatchPattern;
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