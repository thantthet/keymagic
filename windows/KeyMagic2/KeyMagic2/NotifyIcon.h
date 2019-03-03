#pragma once

#define MY_TRAY_ICON_ID 999
#define MY_TRAY_ICON_MESSAGE WM_APP + 999

void CreateShellNotifyIcon(HWND hWnd, HICON icon, LPCTSTR tooltipText = NULL, LPCTSTR title = _T("KeyMagic"));
void UpdateShellNotifyIcon(HWND hWnd, HICON icon = NULL, LPCTSTR tooltipText = NULL, LPCTSTR title = _T("KeyMagic"));
void DeleteShellNotifyIcon(HWND hWnd);