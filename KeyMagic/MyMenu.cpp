#include "Keymagic.h"
#include "MyMenu.h"

HMENU hKeyMenu;
UINT KeyBoardNum;

void CreateMyMenu(HMENU hMenu,UINT uID){

	MENUINFO mi;
	MENUITEMINFO mii;
	MYITEM *pMyItem;

	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_APPLYTOSUBMENUS | MIM_BACKGROUND;
	
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(255,255,255);
	mi.hbrBack = CreateBrushIndirect(&lb);

	SetMenuInfo(hMenu, &mi);

	pMyItem = (MYITEM *) LocalAlloc(LPTR,
		sizeof(MYITEM) + CCH_MAXITEMTEXT);

	// Save the item text in the item structure. 

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING; 
	mii.dwTypeData = pMyItem->szItemText; 
	mii.cch = CCH_MAXITEMTEXT; 
	GetMenuItemInfo(hMenu, uID, FALSE, &mii); 
	pMyItem->cchItemText = mii.cch; 

	// Reallocate the structure to the minimum required size. 

	pMyItem = (MYITEM *) LocalReAlloc(pMyItem,
		sizeof(MYITEM) + mii.cch, LPTR); 

	// Change the item to an owner-drawn item, and save 
	// the address of the item structure as item data. 

	mii.fMask = MIIM_FTYPE | MIIM_DATA; 
	mii.fType = MFT_OWNERDRAW; 
	mii.dwItemData = (ULONG_PTR) pMyItem; 
	SetMenuItemInfo(hMenu, uID, FALSE, &mii); 

}

void DestroyMyMenu(HMENU hMenu, UINT uID){
	
	MENUITEMINFO mii;
    MYITEM *pMyItem; 

	// Get the item data. 

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_DATA; 
	GetMenuItemInfo(hKeyMenu, uID, FALSE, &mii); 
	pMyItem = (MYITEM *) mii.dwItemData; 

	LocalFree(pMyItem); 

}

void DrawMyMenu(LPDRAWITEMSTRUCT lpdis){

	UINT cch;
	MENUITEMINFO mii;
	MYITEM *pMyItem = (MYITEM *) lpdis->itemData;
	
	if (lpdis->CtlType == ODT_MENU){

		LOGBRUSH lb;
		RECT rc;
		rc.bottom = lpdis->rcItem.bottom;
		rc.left = lpdis->rcItem.left;
		rc.right = 20;
		rc.top = lpdis->rcItem.top;

		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(210,240,255);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(lpdis->hDC,CreateSolidBrush(RGB(255,255,255)));
		COLORREF crOldTextColor = SetTextColor(lpdis->hDC, RGB(10,120,245));
		HPEN hOldPen = (HPEN)SelectObject(lpdis->hDC, CreatePen(PS_INSIDEFRAME, 1, RGB(255,255,255)));

		if (lpdis->itemState & ODS_SELECTED)
		{
			SelectObject(lpdis->hDC,CreateSolidBrush(RGB(210,240,255)));
			SetTextColor(lpdis->hDC, RGB(0,128,170));
			SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(153,217,234)));
		}

		else if (lpdis->itemState & ODS_CHECKED)
		{
			SelectObject(lpdis->hDC,CreateSolidBrush(RGB(240,250,255)));
			SetTextColor(lpdis->hDC, RGB(0,128,192));
			SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(110,205,220)));
		}
		
		SetBkMode(lpdis->hDC, TRANSPARENT);
		RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 3, 3);

		FillRect(lpdis->hDC, &rc, CreateBrushIndirect(&lb));

		lb.lbStyle = BS_NULL;
		SelectObject(lpdis->hDC,CreateBrushIndirect(&lb));
		RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 3, 3);

		char Temp[256];

		strcpy(Temp, pMyItem->szItemText);

		strtok(Temp, "\t");
		char* szShortCut = strtok(NULL, "\t");

		lpdis->rcItem.top += 3;

		lpdis->rcItem.left += 24;

		DrawText(lpdis->hDC, Temp, lstrlen(Temp), &lpdis->rcItem, DT_VCENTER);

		lpdis->rcItem.right -= 8;
		DrawText(lpdis->hDC, szShortCut, lstrlen(szShortCut), &lpdis->rcItem,DT_VCENTER | DT_RIGHT);

		SelectObject(lpdis->hDC, hOldBrush);
		SelectObject(lpdis->hDC, hOldPen);
		SetTextColor(lpdis->hDC, crOldTextColor);
	}
}

void OnMenuMeasure(HWND hwnd,LPMEASUREITEMSTRUCT lpmis)
{

	if (lpmis->CtlType != ODT_MENU)
		return;
    MYITEM *pMyItem = (MYITEM *) lpmis->itemData;
	HDC hdc = GetDC(hwnd);
    SIZE size; 
 
    GetTextExtentPoint32(hdc, pMyItem->szItemText, 
            pMyItem->cchItemText, &size); 
 
    lpmis->itemWidth = size.cx+20; 
    lpmis->itemHeight = size.cy+5;

    ReleaseDC(hwnd, hdc);

}