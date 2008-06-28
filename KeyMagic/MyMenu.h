#include <windows.h>

void	CreateMyMenu(HMENU hMenu,UINT uID);
void	DrawMyMenu(LPDRAWITEMSTRUCT lpdis);
void	OnMenuMeasure(HWND hwnd, LPMEASUREITEMSTRUCT lpmis);
void	DestroyMyMenu(HMENU hMenu, UINT uID);

extern HMENU hKeyMenu;
extern UINT KeyBoardNum;