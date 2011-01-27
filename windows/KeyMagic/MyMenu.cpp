//OwnerDrawMenu
//Copyright (C) 2008  KeyMagic Project
//http://keymagic.googlecode.com
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "Keymagic.h"
#include "MyMenu.h"

void CreateMyMenu(HMENU hMenu) {

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

	int intMenu = GetMenuItemCount(hMenu);

	for (int i=0; intMenu > i; i++){

		//pMyItem = (MYITEM *) LocalAlloc(LPTR,
		//	sizeof(MYITEM) + CCH_MAXITEMTEXT);
		//pMyItem = (MYITEM *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		//	sizeof(MYITEM) + CCH_MAXITEMTEXT);
		pMyItem = (MYITEM *) malloc(sizeof (MYITEM) + CCH_MAXITEMTEXT);

		// Save the item text in the item structure.

		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_STRING; 
		mii.dwTypeData = pMyItem->szItemText; 
		mii.cch = CCH_MAXITEMTEXT; 
		GetMenuItemInfo(hMenu, i, TRUE, &mii); 
		pMyItem->cchItemText = mii.cch;

		// Reallocate the structure to the minimum required size.

		//pMyItem = (MYITEM *) LocalReAlloc(pMyItem,
		//	sizeof(MYITEM) + mii.cch, LPTR); 
		//pMyItem = (MYITEM *) HeapReAlloc(GetProcessHeap(), HEAP_REALLOC_IN_PLACE_ONLY,
		//	pMyItem, sizeof(MYITEM) + mii.cch);
		realloc(pMyItem, sizeof(MYITEM) + (mii.cch * sizeof(TCHAR)) );

		// Change the item to an owner-drawn item, and save 
		// the address of the item structure as item data. 

		mii.fMask = MIIM_FTYPE | MIIM_DATA; 
		mii.fType = MFT_OWNERDRAW; 
		mii.dwItemData = (ULONG_PTR) pMyItem; 
		SetMenuItemInfo(hMenu, i, TRUE, &mii);
	}
}

void DestroyMyMenu(HMENU hMenu){
	
	MENUITEMINFO mii;
    MYITEM *pMyItem; 

	// Get the item data. 
	int intMenu = GetMenuItemCount(hMenu);

	for (int i=0; intMenu > i; i++){
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_DATA; 
		GetMenuItemInfo(hMenu, i, TRUE, &mii);
		pMyItem = (MYITEM *) mii.dwItemData;
		if (pMyItem)
			//LocalFree(pMyItem);
			//HeapFree(GetProcessHeap(), 0, pMyItem);
			free(pMyItem);
	}

}

void DrawMyMenu(LPDRAWITEMSTRUCT lpdis){

	UINT cch;
	MENUITEMINFO mii;
	
	if (lpdis->CtlType == ODT_MENU){
		MYITEM *pMyItem = (MYITEM *) lpdis->itemData;
		if (!pMyItem)
			return;

		Debug(L"DMM->%d,%d\n",
			lpdis->rcItem.right - lpdis->rcItem.left,
			lpdis->rcItem.bottom - lpdis->rcItem.top);

		LOGBRUSH lb;
		RECT rc;
		rc.bottom = lpdis->rcItem.bottom;
		rc.left = lpdis->rcItem.left;
		rc.right = 20;
		rc.top = lpdis->rcItem.top;

		lb.lbStyle = BS_SOLID;
		lb.lbColor = RGB(215,227,230);//RGB(255,255,255);
		FillRect(lpdis->hDC, &lpdis->rcItem, CreateBrushIndirect(&lb));

		//lb.lbStyle = BS_SOLID;
		//lb.lbColor = RGB(233,236,238);//RGB(210,240,255);
		//FillRect(lpdis->hDC, &rc, CreateBrushIndirect(&lb));
		TRIVERTEX vertex[2];
		vertex[0].Red = 240 << 8;
		vertex[0].Green = 242 << 8;
		vertex[0].Blue = 251 << 8;
		vertex[0].x = rc.left;
		vertex[0].y = rc.top;

		vertex[1].Red = 211 << 8;
		vertex[1].Green = 218 << 8;
		vertex[1].Blue = 241 << 8;
		vertex[1].x = rc.right;
		vertex[1].y = rc.bottom;

		GRADIENT_RECT gRect;
		gRect.UpperLeft = 0;
		gRect.LowerRight = 1;

		GradientFill(lpdis->hDC, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);

		HBRUSH hOldBrush = (HBRUSH)SelectObject(lpdis->hDC,CreateSolidBrush(RGB(255,255,255)));
		COLORREF crOldTextColor = SetTextColor(lpdis->hDC, RGB(0,0,0));//RGB(10,120,245));
		HPEN hOldPen = (HPEN)SelectObject(lpdis->hDC, CreatePen(PS_INSIDEFRAME, 1, RGB(240,240,240)));

		MoveToEx(lpdis->hDC, rc.right, rc.top, NULL);
		LineTo(lpdis->hDC, rc.right, rc.bottom);

		if (lpdis->itemState & ODS_SELECTED){
			rc.left = rc.top = 0;
			rc.right = lpdis->rcItem.right - lpdis->rcItem.left;
			rc.bottom = lpdis->rcItem.bottom - lpdis->rcItem.top;

			HDC hdcSelector = CreateCompatibleDC(lpdis->hDC);
			HBITMAP hbmSelector = CreateCompatibleBitmap(lpdis->hDC,
				lpdis->rcItem.right - lpdis->rcItem.left,
				lpdis->rcItem.bottom - lpdis->rcItem.top);
			SelectObject ( hdcSelector, hbmSelector );

			BitBlt(hdcSelector, rc.left, rc.top,
				rc.right - rc.left,
				rc.bottom - rc.top,
				lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, SRCCOPY);

			SelectObject(hdcSelector,CreateSolidBrush(RGB(210,240,255)));
			//SetTextColor(lpdis->hDC, RGB(0,128,170));
			SelectObject(hdcSelector,CreatePen(PS_INSIDEFRAME, 1, RGB(158,202,239)));
			//RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 3, 3);
			GradientFillRoundRect(hdcSelector, rc, 5, 5,
				RGB(244,249,255), RGB(225,250,255), RGB(208,234,255), GRADIENT_FILL_RECT_V);
			
			BLENDFUNCTION bf;
			bf.AlphaFormat = 0;
			bf.BlendFlags = 0;
			bf.BlendOp = AC_SRC_OVER;
			bf.SourceConstantAlpha = 200;
			AlphaBlend(
				lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top,
				lpdis->rcItem.right - lpdis->rcItem.left,
				lpdis->rcItem.bottom - lpdis->rcItem.top,
				hdcSelector, 0, 0,
				rc.right - rc.left,
				rc.bottom - rc.top, bf);

			DeleteObject(hbmSelector);
			DeleteDC(hdcSelector);
		}

		else if (lpdis->itemState & ODS_CHECKED) {
			SelectObject(lpdis->hDC,CreateSolidBrush(RGB(240,250,255)));
			//SetTextColor(lpdis->hDC, RGB(0,128,192));
			SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(160,158,239)));
			//RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 5, 5);
			GradientFillRoundRect(lpdis->hDC, lpdis->rcItem, 5, 5,
				RGB(246,246,255), RGB(227,225,255), RGB(209,208,255), GRADIENT_FILL_RECT_V);
		}

		//lb.lbStyle = BS_NULL;
		//SelectObject(lpdis->hDC,CreateBrushIndirect(&lb));
		//RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 3, 3);

		TCHAR Temp[CCH_MAXITEMTEXT];
		lstrcpy(Temp, pMyItem->szItemText);
		wcstok(Temp, TEXT("\t"));
		TCHAR* szShortCut = wcstok(NULL, TEXT("\t"));

		lpdis->rcItem.top += 3;
		lpdis->rcItem.left += MMENU_LEFT_PADDING;

		HFONT hFont = (HFONT)SelectObject(lpdis->hDC,
			CreateFont(-MulDiv(9, GetDeviceCaps(lpdis->hDC, LOGPIXELSY), 72),
			0,0,0,
			FW_NORMAL,
			FALSE,FALSE,FALSE,
			NULL,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			MENU_FONT)
		);

		SetBkMode(lpdis->hDC, TRANSPARENT);
		DrawText(lpdis->hDC, Temp, wcslen(Temp), &lpdis->rcItem, DT_VCENTER);

		if (szShortCut) {
			lpdis->rcItem.left += GetHotkeyX(lpdis->hwndItem);
			lpdis->rcItem.right -= 8;
			DrawText(lpdis->hDC, szShortCut, wcslen(szShortCut), &lpdis->rcItem, DT_VCENTER | DT_RIGHT);
		}

		SelectObject(lpdis->hDC, hOldBrush);
		SelectObject(lpdis->hDC, hOldPen);
		SetTextColor(lpdis->hDC, crOldTextColor);
		SelectObject(lpdis->hDC, hFont);
	}
}

int GetHotkeyX(HWND hwnd) {
	HMENU hMenu = (HMENU)GetParent(hwnd);
	int intMenu = GetMenuItemCount(hMenu);

	MENUITEMINFO mii;
    MYITEM *pMyItem;

	int w = 0;

	for (int i=0; intMenu > i; i++){
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_DATA; 
		GetMenuItemInfo(hMenu, i, TRUE, &mii);
		pMyItem = (MYITEM *) mii.dwItemData;
		if (w < pMyItem->textWidth) {
			w = pMyItem->textWidth;
		}
	}
	return w;
}

void OnMenuMeasure(HWND hwnd, LPMEASUREITEMSTRUCT lpmis)
{

	if (lpmis->CtlType != ODT_MENU)
		return;
    MYITEM *pMyItem = (MYITEM *) lpmis->itemData;
	if (!pMyItem)
		return;
	HDC hdc = GetDC(hwnd);
    SIZE txSize, hkSize;

	TCHAR Text[CCH_MAXITEMTEXT];
	lstrcpy(Text, pMyItem->szItemText);
	wcstok(Text, TEXT("\t"));
	TCHAR* szShortCut = wcstok(NULL, TEXT("\t"));

	HFONT hFont = (HFONT)SelectObject(hdc,
		CreateFont(-MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72),
		0,0,0,
		FW_NORMAL,
		FALSE,FALSE,FALSE,
		NULL,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		MENU_FONT)
	);
 
    GetTextExtentPoint32(hdc, Text, wcslen(Text), &txSize);
	if (szShortCut) {
		GetTextExtentPoint32(hdc, szShortCut, wcslen(szShortCut), &hkSize);
		hkSize.cx += 20; //tab width
	} else {
		hkSize.cx = 0;
	}

	pMyItem->textWidth = txSize.cx;
	pMyItem->hotkeyWidth = hkSize.cx;

    lpmis->itemWidth = txSize.cx + hkSize.cx + MMENU_LEFT_PADDING;
    lpmis->itemHeight = txSize.cy + 5;

	Debug(L"OMM->%d,%d\n", lpmis->itemWidth, lpmis->itemHeight);

	SelectObject(hdc, hFont);
    ReleaseDC(hwnd, hdc);
}

bool GradientFillRoundRect(HDC hDC, RECT rcItem, int w, int h,
						   DWORD rgb1, DWORD rgb2, DWORD rgb3, ULONG ulMode){
	HRGN hClipRgn = CreateRoundRectRgn(rcItem.left, rcItem.top,
		rcItem.right + 1, rcItem.bottom + 1, w, h);
	SelectClipRgn(hDC, hClipRgn);

	TRIVERTEX vertex[4] ;
	vertex[0].x     = rcItem.left;
	vertex[0].y     = rcItem.top;
	vertex[0].Red   = MAKEWORD(0, GetRValue(rgb1));
	vertex[0].Green = MAKEWORD(0, GetGValue(rgb1));
	vertex[0].Blue  = MAKEWORD(0, GetBValue(rgb1));
	vertex[0].Alpha = 0x0000;

	vertex[1].x     = rcItem.right;
	vertex[1].y     = rcItem.bottom - ((rcItem.bottom-rcItem.top)/2); 
	vertex[1].Red   = MAKEWORD(0, GetRValue(rgb2));
	vertex[1].Green = MAKEWORD(0, GetGValue(rgb2));
	vertex[1].Blue  = MAKEWORD(0, GetBValue(rgb2));
	vertex[1].Alpha = 0x0000;

	vertex[2].x     = rcItem.left;
	vertex[2].y     = rcItem.bottom - ((rcItem.bottom-rcItem.top)/2);
	vertex[2].Red   = MAKEWORD(0, GetRValue(rgb3));
	vertex[2].Green = MAKEWORD(0, GetGValue(rgb3));
	vertex[2].Blue  = MAKEWORD(0, GetBValue(rgb3));
	vertex[2].Alpha = 0x0000;


	vertex[3].x     = rcItem.right;
	vertex[3].y     = rcItem.bottom; 
	vertex[3].Red   = MAKEWORD(0, GetRValue(rgb3));
	vertex[3].Green = MAKEWORD(0, GetGValue(rgb3));
	vertex[3].Blue  = MAKEWORD(0, GetBValue(rgb3));
	vertex[3].Alpha = 0x0000;

	GRADIENT_RECT gRect[2];
	gRect[0].UpperLeft  = 0;
	gRect[0].LowerRight = 1;

	gRect[1].UpperLeft  = 2;
	gRect[1].LowerRight = 3;

	bool bSuccess = GradientFill(hDC, vertex, 4, gRect, 2, ulMode);
	SelectClipRgn(hDC, 0);
	DeleteObject(hClipRgn);

	LOGBRUSH lb;
	lb.lbStyle = BS_NULL;
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, CreateBrushIndirect(&lb));
	RoundRect(hDC, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, w, h);
	SelectObject(hDC, hOldBrush);

	return bSuccess;
}