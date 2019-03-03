#include "stdafx.h"

#include "NotifyIcon.h"

#include <shellapi.h>
#include <CommCtrl.h>

void CreateShellNotifyIcon(HWND hWnd, HICON icon, LPCTSTR tooltipText, LPCTSTR title)
{
	NOTIFYICONDATA ni = { 0 };
	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.uID = MY_TRAY_ICON_ID;
	ni.uFlags = NIF_ICON | NIF_MESSAGE;
	ni.hIcon = icon;
	ni.hWnd = hWnd;
	ni.dwInfoFlags = NIIF_NOSOUND;
	if (tooltipText && title) {
		ni.uFlags |= NIF_TIP | NIF_INFO;
		lstrcpyn(ni.szTip, title, sizeof(ni.szTip));
		lstrcpyn(ni.szInfo, tooltipText, sizeof(ni.szInfo));
	}
	ni.uCallbackMessage = MY_TRAY_ICON_MESSAGE;

	Shell_NotifyIcon(NIM_ADD, &ni);
}

void UpdateShellNotifyIcon(HWND hWnd, HICON icon, LPCTSTR tooltipText, LPCTSTR title)
{
	NOTIFYICONDATA ni = { 0 };
	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.uID = MY_TRAY_ICON_ID;
	ni.uFlags = 0;
	ni.hWnd = hWnd;
	if (icon)
	{
		ni.uFlags |= NIF_ICON;
		ni.hIcon = icon;
	}
	if (tooltipText && title)
	{
		ni.uFlags |= NIF_TIP | NIF_INFO;
		lstrcpyn(ni.szTip, title, sizeof(ni.szTip));
		lstrcpyn(ni.szInfo, tooltipText, sizeof(ni.szInfo));
	}
	Shell_NotifyIcon(NIM_MODIFY, &ni);
}

void DeleteShellNotifyIcon(HWND hWnd)
{
	NOTIFYICONDATA ni = { 0 };
	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.hWnd = hWnd;
	ni.uID = MY_TRAY_ICON_ID;

	Shell_NotifyIcon(NIM_DELETE, &ni);
}