#include <windows.h>

void CreateMyMenu(HWND hOwner,HMENU hMenu);
void DestroyMyMenu(HMENU hMenu);
void DrawMyMenu(LPDRAWITEMSTRUCT lpdis);
void OnMenuMeasure(HWND hwnd, LPMEASUREITEMSTRUCT lpmis);
INT_PTR CALLBACK MyMenuProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);