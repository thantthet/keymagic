#pragma once

#include "stdafx.h"

struct Control
{
public:
	Control(HWND hWnd) : hWnd(hWnd) {};
	Control(HWND parent, int ID) {
		hWnd = GetDlgItem(parent, ID);
	};

	RECT GetRect() {
		RECT rect;
		GetWindowRect(hWnd, &rect);
		MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&rect, 2);
		return rect;
	}

	void SetPos(int x, int y, int width, int height) {
		MoveWindow(hWnd,
			x,
			y,
			width,
			height,
			TRUE);
	}
private:
	HWND hWnd;
};