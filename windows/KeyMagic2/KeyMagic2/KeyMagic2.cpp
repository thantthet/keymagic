// KeyMagic2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <Shlwapi.h>

#include "KeyMagic2.h"
#include "ConfigUtils.h"
#include "dpi.h"
#include "Tasker.h"
#include "MainWindow.h"
#include "HookProc.h"
#include "HudWindow.h"
#include "UpdateChecker.h"

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using json = nlohmann::json;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
HWND hHotkeyLabel;

bool CheckIfRunning()
{
	// Try to open the mutex.
	HANDLE hMutex = OpenMutex(
		MUTEX_ALL_ACCESS, 0, _T("KeyMagic"));

	if (!hMutex) {
		// Mutex doesn’t exist. This is
		// the first instance so create
		// the mutex.
		hMutex = CreateMutex(0, 0, _T("KeyMagic"));
	}
	else {
		WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
		LoadStringW(GetModuleHandle(NULL), IDC_KEYMAGIC2, szWindowClass, MAX_LOADSTRING);

		// The mutex exists so this is the
		// the second instance so return.

		HWND hWnd = FindWindow(szWindowClass, szTitle);
		if (hWnd) {
			ShowWindow(hWnd, SW_RESTORE);
			SetForegroundWindow(hWnd);
		}
		return true;
	}

	return false;
}

void AddFont()
{
	TCHAR filename[256] = { 0 };
	GetModuleFileName(NULL, filename, 255);
	PathRemoveFileSpecW(filename);
	PathAppend(filename, _T("Padauk-Bold.ttf"));

	int nResults = AddFontResourceEx(
		filename, // font file name
		FR_PRIVATE, // font characteristics
		NULL);

	DebugLog("" << nResults);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);

	// Initialize global strings
	unsigned digitsCurrent[4];
	UpdateChecker::GetBinaryVersion(digitsCurrent);
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	wsprintf(szTitle, _T("%s v%d.%d.%d.%d"), szTitle, digitsCurrent[0], digitsCurrent[1], digitsCurrent[2], digitsCurrent[3]);

	if (std::wstring(lpCmdLine).find(L"/runAtBoot") != std::string::npos)
	{
		RegisterProgram();
		return 1;
	}
	else if (std::wstring(lpCmdLine).find(L"/NoRunAtBoot") != std::string::npos) {
		UnRegisterProgramForStartup();
		return 1;
	}
	else if (std::wstring(lpCmdLine).find(L"/startup") != std::string::npos) {
		nCmdShow = SW_HIDE;
	}

	if (CheckIfRunning()) {
		return 0;
	}
#ifdef DEBUG
	char szPath[MAX_PATH];

	if (SUCCEEDED(SHGetFolderPathA(NULL,
		CSIDL_APPDATA,
		NULL,
		0,
		szPath)))
	{
		PathAppendA(szPath, "keymagic.log");

		FILE * hFile;

#ifdef _MSC_VER
		fopen_s(&hFile, szPath, "wb");
#else
		hFile = fopen(szPath, "wb");
#endif

		libkm::KeyMagicLogger::getInstance()->setFile(hFile);
	}
#endif

	LoadSymbols();
	AddFont();

	HudWindow * hud = new HudWindow(hInstance, KeyboardManager::sharedManager());
	hud->InitInstance();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KEYMAGIC2));

	if (auto atom = MainWindow::Initialize(hInstance)) {
		static const auto D = CW_USEDEFAULT;
		if (auto hWnd = CreateWindow(atom, szTitle,
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			D, D, D, D, HWND_DESKTOP, NULL, hInstance, NULL)) {
			ShowWindow(hWnd, nCmdShow);
			InitHooks(hWnd);

			MSG message;
			message.wParam = 0;

			while (GetMessage(&message, NULL, 0u, 0u)) {
				if (!IsDialogMessage(GetAncestor(message.hwnd, GA_ROOT), &message)) {
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
			return (int)message.wParam;
		}
	}
	return (int)GetLastError();
}