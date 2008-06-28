#include "Keymagic.h"
#include "MyButton.h"

void DrawMyButton(LPDRAWITEMSTRUCT lpdis) 
{
   
   if (lpdis->CtlType != ODT_BUTTON)
	   return;

   SetBkMode(lpdis->hDC, TRANSPARENT);
   HBRUSH OldBrush = (HBRUSH) SelectObject(lpdis->hDC,CreateSolidBrush(RGB(240,250,255)));
   HPEN OldPen = (HPEN)SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(153,217,234)));

   if (lpdis->itemState & ODS_SELECTED){
	   SelectObject(lpdis->hDC,CreateSolidBrush(RGB(210,240,255)));
	   SelectObject(lpdis->hDC,CreatePen(PS_INSIDEFRAME, 1, RGB(110,205,220)));
   }

   RoundRect(lpdis->hDC, lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, 5, 5);

   char szText[30]={0};
   GetWindowText(lpdis->hwndItem, szText, 30);

   COLORREF crOldColor = SetTextColor(lpdis->hDC, RGB(10,120,245));;
   DrawText(lpdis->hDC, szText, lstrlen(szText), 
      &lpdis->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   SetTextColor(lpdis->hDC, crOldColor);
   SelectObject(lpdis->hDC, OldBrush);
   SelectObject(lpdis->hDC, OldPen);
}