#include "stdafx.h"

#include "Order.h"
#include "KeyMagicDll.h"

bool Order::Pattern(const wchar_t* SPattern, int length){
	const wchar_t *sp = SPattern;
	wchar_t *cp = CPattern;
	wchar_t prevchar = NULL;

	for (int i=0; i<length; i++, sp++) {
		if (!*sp)
			break;
		switch (*sp){
		case '[':
			*cp++ = CCL;
			*cp++ = SET;

			if (*++sp == '^'){
				i++;
				*cp++ = NOT;
			}

			if (*(sp+1) == '-' && *(sp+3) == ']'){
				MakeSet(*(sp),*(sp+2));
				sp+=4;
				i+=4;
				continue;
			}
			while (*sp && *sp != L']') {
				MakeSet(*++sp);
				i++;
			}
			
			break;

		default :               /* an ordinary char */
			if (*sp){
				*cp++ = CHR;
				*cp++ = *sp;
			}
		}
	}
	*cp = END;
	return false;
}

void Order::MakeSet(const wchar_t Start,const wchar_t End){
	SetStart = Start;
	SetEnd = End;
}

void Order::MakeSet(const wchar_t New){
	wcscat_s((wchar_t*)Set, 1, &New);
}

bool Order::IsInSet(const wchar_t Input){
	if (SetStart && SetEnd){
		if (Input >= SetStart && Input <= SetEnd)
			return true;
	}
	else{
		return (bool)wcsstr((wchar_t*)Set, (wchar_t*)&Input);
	}
	return false;
}

bool Order::Match(const wchar_t* Dest,int length){
	const wchar_t *cp = CPattern;
	const wchar_t *d = Dest;

	for (int i=0 , s=0; i<length; i++) {
		switch (cp[s]){
		case CHR:
			s++;
			if (cp[s] == *d){
				s++;
				if (cp[s]==END)
					return true;
			}
			else s=0;
			break;
		case CCL:
			if (IsInSet(*d)){
				s+=2;
				if (cp[s]==END)
					return true;
			}
			else s=0;
			break;
		}
		*++d;
	}
	return false;
}

wchar_t Order::order(wchar_t* Dest){

	wchar_t Temp;

	HWND hwnd = GetFocus();
	if(!hwnd)
		return false;

	Temp = *(Dest+1);
	*(Dest+1) = *Dest;
	*Dest = Temp;
	
	PostMessageW(hwnd, WM_CHAR, (WPARAM)VK_BACK, 0xE001 );
	PostMessageW(hwnd, WM_CHAR, (WPARAM)*Dest, 0x110001 );
	PostMessageW(hwnd, WM_CHAR, (WPARAM)*(Dest+1), 0x110001 );
	
	return *(Dest+1);
}