#include <windows.h>
#include "keyMagic.h"

INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				OnInitDlg(HWND hWnd);
void				SetKbData(HWND hWnd);
void				DeleteDlgData(HWND hWnd);
bool				OpenDialog(HWND hwnd, char* szFileName,DWORD nMaxFile);
bool				RemoveKeyBoard();
bool				DeleteKeyFile();
bool				AddKeyBoardToList(HWND hWnd, char* szFileName);
void				GetHotKey(WORD wHotkey, LPSTR ShortCutDisplay);
bool				UpdateDlgData(HWND hWnd);
void				restart(HWND hWnd);
void				error(LPCSTR lpszFunction) ;
void				GetKeyBoards();

extern strDelete *szFileToDelete;
extern int cbFileToDelete;
extern int kbindex;
extern HWND hList;