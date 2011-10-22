#include <Windows.h>
#include "../KeyMagic.h"
#include "../../global/global.h"
#include <map>

#include "osk.h"

void kOSK::InitDefLayout()
{
	wchar_t ** it;
	int i;

	wchar_t * nshift[] = {
							L"`", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"-", L"=", L"backspace"
							L"tab", L"q", L"w", L"e", L"r", L"t", L"y", L"u", L"i", L"o", L"p", L"[", L"]", L"\\",
							L"caps lock", L"a", L"s", L"d", L"f", L"g", L"h", L"j", L"k", L"l", L";", L"'", L"return",
							L"shift", L"z", L"x", L"c", L"v", L"b", L"n", L"m", L",", L".", L"/", L"shift",
							L"control", L"alt", L"space", L"alt", L"control", L"\0"
							}
							;

	//ASSIGN UNSHIFTED
	it = nshift;
	i = 0;

	for (++it; *it[0]; ++it) {
		nshifted[i++] = *it;
	}

	wchar_t * shift[] = {
							L"~", L"!", L"@", L"#", L"$", L"%", L"^", L"&", L"*", L"(", L")", L"_", L"+", L"backspace"
							L"tab", L"Q", L"W", L"E", L"R", L"T", L"Y", L"U", L"I", L"O", L"P", L"{", L"}", L"|",
							L"caps lock", L"A", L"S", L"D", L"F", L"G", L"H", L"J", L"K", L"L", L":", L"\"", L"return",
							L"shift", L"Z", L"X", L"C", L"V", L"B", L"N", L"M", L"<", L">", L"?", L"shift",
							L"control", L"alt", L"space", L"alt", L"control", L"\0"
							}
							;

	//ASSIGN SHIFTED
	it = shift;
	i = 0;

	for (++it; *it[0]; ++it) {
		shifted[i++] = *it;
	}
}

BOOL kOSK::CreateLWindow()
{
	WNDCLASSEX wincl;

    wincl.hInstance = hInst;
    wincl.lpszClassName = TEXT("kOSK");
    wincl.lpfnWndProc = LWndProc;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

	wincl.hIcon = LoadIcon (hInst, MAKEINTRESOURCE(IDI_KEYMAGIC));
    wincl.hIconSm = LoadIcon (hInst, MAKEINTRESOURCE(IDI_KEYMAGIC));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
	wincl.hbrBackground = CreateSolidBrush(RGB(236, 244, 255));

    RegisterClassEx (&wincl);

	m_hwnd = CreateWindowEx (
        0, TEXT("kOSK"), TEXT("On-screen Keyboard"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        750, 280,
        HWND_DESKTOP, NULL,
        hInst, NULL
		);

	PrintLastError();

	if (m_hwnd == 0)
		return false;

	return true;
}

BOOL kOSK::ShowLWindow(int cmd)
{
	ShowWindow(m_hwnd, cmd);
	return true;
}

LRESULT CALLBACK LWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
			HDC hdc;
			PAINTSTRUCT ps;
			
			hdc = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);

			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}