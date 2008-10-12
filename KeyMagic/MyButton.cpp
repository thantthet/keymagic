//OwnerDrawButton
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

#include <string>
#include "Keymagic.h"
#include "MyButton.h"

ButtonCr Up, Down;

using namespace std;

void DrawMyButton(LPDRAWITEMSTRUCT lpdis)
{
	if (lpdis->CtlType != ODT_BUTTON)
		return;

	SetBkMode(lpdis->hDC, TRANSPARENT);
	HBRUSH OldBrush = (HBRUSH) SelectObject(lpdis->hDC,CreateSolidBrush(Up.Fill));
	HPEN OldPen = (HPEN)SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, Up.Frame));
	COLORREF crOldColor = SetTextColor(lpdis->hDC, Up.Text);

	if (lpdis->itemState & ODS_SELECTED){
		SelectObject(lpdis->hDC,CreateSolidBrush(Down.Fill));
		SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, Down.Frame));
		SetTextColor(lpdis->hDC, Down.Text);
	}

	RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 5, 5);

	char szText[30]={0};
	GetWindowText(lpdis->hwndItem, szText, 30);

	std::string strBtnText(szText);
	int s = strBtnText.find("(SI)");

	if (s != -1)
	{
		strBtnText.replace(s, 4, "   ");
		strcpy(szText, strBtnText.c_str());
		DrawIconEx(lpdis->hDC, 
			lpdis->rcItem.left+10, 
			lpdis->rcItem.top+2,
			LoadIcon(hInst, MAKEINTRESOURCE(IDI_BSHIELD)),
			20, 20, NULL, NULL, DI_NORMAL | DI_COMPAT);
	}

	DrawText(lpdis->hDC,szText, strlen(szText),
		&lpdis->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

	SetTextColor(lpdis->hDC, crOldColor);
	SelectObject(lpdis->hDC, OldBrush);
	SelectObject(lpdis->hDC, OldPen);

}