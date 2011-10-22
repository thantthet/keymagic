#ifndef OSK_H
#define OSK_H

class kOSK
{
public:
	std::wstring nshifted[57];
	std::wstring shifted[57];

	kOSK()
	{
		InitDefLayout();
	}
	void InitDefLayout();
	BOOL CreateLWindow();
	BOOL ShowLWindow(int);

private:
	HWND m_hwnd;
};

LRESULT CALLBACK LWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif //OSK_H