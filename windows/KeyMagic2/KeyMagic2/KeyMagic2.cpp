// KeyMagic2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "version.h"

#include "KeyMagic2.h"
#include "HudWindow.h"

#include <Commdlg.h>
#include <string>
#include <shellapi.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include "ConfigUtils.h"
#include "HookProc.h"
#include "KeyboardManager.h"
#include "HotkeyManager.h"
#include <keymagic.h>
#include "Tasker.h"
#include "NotifyIcon.h"
#include "../MagicAssit/MagicAssit.h"

#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "version.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDM_KEYBOARD_ 0x5000

const int kRightColumnWidth = 150;
const int kRightColumnPadding = 10;
const int kListViewMargin = 10;
const int kButtonWidth = kRightColumnWidth - kRightColumnPadding * 2;
const int kButtonHeight = 30;

using json = nlohmann::json;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BOOL updateVersionNotificationWasShown = FALSE;

// Forward declarations of functions included in this code module:
ATOM                RegisterWindowClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Hotkeys(HWND, UINT, WPARAM, LPARAM);

template<typename string_t>
string_t dirname(string_t source)
{
	source.erase(std::find(source.rbegin(), source.rend(), '\\').base(), source.end());
	return source;
}

template<class T>
T base_name(T const & path)
{
	return path.substr(path.find_last_of('\\') + 1);
}
template<class T>

T remove_extension(T const & filename)
{
	typename T::size_type const p(filename.find_last_of('.'));
	return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}

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
		// The mutex exists so this is the
		// the second instance so return.

		HWND hWnd = FindWindow(szWindowClass, szTitle);

		ShowWindow(hWnd, SW_SHOW);

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
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_KEYMAGIC2, szWindowClass, MAX_LOADSTRING);

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

	AddFont();

	HudWindow * hud = new HudWindow(hInstance, KeyboardManager::sharedManager());
	hud->InitInstance();

    RegisterWindowClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KEYMAGIC2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	//Dll_UnHook();

    return (int) msg.wParam;
}



//
//  FUNCTION: RegisterWindowClass()
//
//  PURPOSE: Registers the window class.
//
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KEYMAGIC2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_KEYMAGIC2));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 500, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   InitHooks(hWnd);

   return TRUE;
}

BOOL InsertListViewItems(HWND hWndListView, Keyboard keyboard)
{
	LVITEM lvI;

	TCHAR text[200];

	lstrcpyn(text, keyboard.name.c_str(), 200);

	lvI.pszText = text;
	lvI.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;
	lvI.iItem = keyboard.index;
	lvI.iImage = keyboard.imageListIndex;

	if (ListView_InsertItem(hWndListView, &lvI) == -1)
		return FALSE;

	return TRUE;
}

BOOL InitListViewColumns(HWND hWndListView)
{
	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	for (iCol = 0; iCol < 1; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.cx = 200;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

									// Load the names of the column headings from the string resources.
		LoadString(hInst,
			IDS_LVC_FIRST + iCol,
			szText,
			sizeof(szText) / sizeof(szText[0]));

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;
	}

	return TRUE;
}

BOOL CreateListView(HWND hWnd)
{
	HWND hControl;

	hControl = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW,
		_T(""),
		WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_HSCROLL | WS_VSCROLL,
		0, 0, 0, 0,
		hWnd,
		(HMENU)IDC_LV_KEYBOARDS,
		hInst,
		NULL);
	if (hControl == NULL) {
		MessageBox(hWnd, _T("Could not create list view."), _T("Error"), MB_OK | MB_ICONERROR);
		return false;
	}

	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

	InitListViewColumns(hControl);

	return true;
}

HWND CreateButton(HWND hWnd, LPTSTR title, HMENU buttonId)
{
	HWND hControl;

	hControl = CreateWindow(WC_BUTTON,
		title,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		0, 0, 0, 0,
		hWnd,
		buttonId,
		hInst,
		NULL);

	if (hControl == NULL) {
		MessageBox(hWnd, _T("Could not create button."), _T("Error"), MB_OK | MB_ICONERROR);
	}

	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

	return hControl;
}

HWND CreateAddKeyboardButton(HWND hWnd)
{
	HWND hControl = CreateButton(hWnd, _T("Add"), (HMENU)IDC_BTN_ADD);

	return hControl;
}

HWND CreateRemoveKeyboardButton(HWND hWnd)
{
	HWND hControl = CreateButton(hWnd, _T("Remove"), (HMENU)IDC_BTN_REMOVE);

	return hControl;
}

HWND CreateReportBugButton(HWND hWnd)
{
	HWND hControl = CreateButton(hWnd, _T("Report Bug"), (HMENU)IDC_BTN_BUG);

	return hControl;
}

HWND CreateLabel(HWND hWnd)
{
	std::wstring text;

	text += converter.from_bytes(HotkeyManager::wHotkeyToString(HotkeyManager::sharedManager()->hky_onoff));
	text += _T(" = Enable/Disable KeyMagic.\n\n");
	text += converter.from_bytes(HotkeyManager::wHotkeyToString(HotkeyManager::sharedManager()->hky_nextkbd));
	text += _T(" = Choose next keyboard.");

	HWND hControl = CreateWindow(_T("static"), _T("ST_U"),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		0, 0, 0, 0,
		hWnd, (HMENU)IDC_LABEL,
		hInst,
		NULL);
	SetWindowText(hControl, text.c_str());

	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

	return hControl;
}

void SizeListView(HWND hWnd)
{
	HWND hControl;
	RECT rcClient;

	GetClientRect(hWnd, &rcClient);

	hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
	SetWindowPos(hControl,
		NULL,
		kListViewMargin,
		kListViewMargin,
		rcClient.right - kRightColumnWidth - kListViewMargin,
		rcClient.bottom - kListViewMargin * 2,
		SWP_NOZORDER);
}

void SizeAddKeyboardButton(HWND hWnd)
{
	HWND hControl;
	RECT rcClient;

	GetClientRect(hWnd, &rcClient);

	hControl = GetDlgItem(hWnd, IDC_BTN_ADD);
	SetWindowPos(hControl,
		NULL,
		rcClient.right - kRightColumnWidth + kRightColumnPadding,
		kRightColumnPadding,
		kButtonWidth,
		kButtonHeight,
		SWP_NOZORDER);
}

void SizeRemoveKeyboardButton(HWND hWnd)
{
	HWND hControl;
	RECT rcClient;

	GetClientRect(hWnd, &rcClient);

	hControl = GetDlgItem(hWnd, IDC_BTN_REMOVE);
	SetWindowPos(hControl,
		NULL,
		rcClient.right - kRightColumnWidth + kRightColumnPadding,
		kRightColumnPadding + kButtonHeight + 5,
		kButtonWidth,
		kButtonHeight,
		SWP_NOZORDER);
}

void SizeReportBugButton(HWND hWnd)
{
	HWND hControl;
	RECT rcClient;

	hControl = GetDlgItem(hWnd, IDC_LABEL);
	GetWindowRect(hControl, &rcClient);
	MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rcClient, 2);

	hControl = GetDlgItem(hWnd, IDC_BTN_BUG);
	SetWindowPos(hControl,
		NULL,
		rcClient.left,
		rcClient.bottom + 5,
		kButtonWidth,
		kButtonHeight,
		SWP_NOZORDER);
}

void SizeLabel(HWND hWnd)
{
	HWND hControl;
	RECT rcClient;

	hControl = GetDlgItem(hWnd, IDC_BTN_REMOVE);
	GetWindowRect(hControl, &rcClient);
	MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rcClient, 2);

	hControl = GetDlgItem(hWnd, IDC_LABEL);
	SetWindowPos(hControl,
		NULL,
		rcClient.left,
		rcClient.bottom + 5,
		kButtonWidth,
		70,
		SWP_NOZORDER);
}

void RegisterKeyboardFile(HWND hWnd, LPCTSTR fileName)
{
	std::string keyboardPath = converter.to_bytes(fileName);

	json config = ConfigUtils::Read();

	json keyboards = config[ConfigKeyKeyboards];
	std::string keyboardName;

	auto infos = libkm::KeyMagicKeyboard::getInfosFromKeyboardFile(keyboardPath.c_str());
	auto name = infos->find('name');
	if (name != infos->end())
	{
		auto nameInfo = name->second;
		keyboardName.assign(nameInfo.Data(), nameInfo.Size());
	}
	else {
		keyboardName.assign(base_name(keyboardPath));
	}

	json keyboard = {
		{"name", keyboardName},
		{"path", keyboardPath}
	};

	keyboards.push_back(keyboard);

	config[ConfigKeyKeyboards] = keyboards;

	ConfigUtils::Write(config);
}

void UnregisterKeyboard(Keyboard &keyboard)
{
	std::wstring dirName = dirname(ConfigUtils::jsonFilePath());

	json config = ConfigUtils::Read();
	json &j = config[ConfigKeyKeyboards];
	std::string path = converter.to_bytes(keyboard.path);
	for (auto it = j.begin(); it != j.end(); ++it) {
		auto &k = *it;
		if (k["path"].get<std::string>() == path)
		{
			j.erase(it);

			ConfigUtils::Write(config);

			DeleteFile((dirName + keyboard.path).c_str());

			return;
		}
	}
}

void AddKeyboardFile(HWND hWnd, LPCTSTR filePath)
{
	std::wstring basename = base_name<std::wstring>(filePath);

	std::wstring destFilePath = (AppDataDirectory() + basename);

	if (PathFileExists(destFilePath.c_str())) {
		int result = MessageBox(hWnd, _T("File with the same name already exits. Do you want to overwrite?"), _T("Y sure?"), MB_YESNO | MB_ICONWARNING);
		if (result == IDYES)
		{
			DeleteFile(destFilePath.c_str());
			AddKeyboardFile(hWnd, filePath);
		}
	}
	else if (CopyFile(filePath, destFilePath.c_str(), true)) {
		RegisterKeyboardFile(hWnd, basename.c_str());
		ReloadKeyboards(hWnd);
	}
	else {
		MessageBox(NULL, _T("Copying failed while registering keyboard!"), _T("Oh Snap!"), MB_OK | MB_ICONERROR);
	}
}

void ReloadKeyboards(HWND hWnd)
{
	json config = ConfigUtils::Read();

	HWND hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
	SendMessage(hControl, LVM_DELETEALLITEMS, 0, 0); // clear list view items

	KeyboardManager *mgr = KeyboardManager::sharedManager();
	json keyboards = config[ConfigKeyKeyboards];
	mgr->basePath(dirname(ConfigUtils::jsonFilePath()));
	mgr->SetKeyboards(keyboards);

	HIMAGELIST himl = ListView_GetImageList(hControl, LVSIL_SMALL);

	if (himl)
	{
		ImageList_Destroy(himl);
	}

	himl = ImageList_Create(16, 16, ILC_COLOR, mgr->GetKeyboards().size(), 1);

	for (auto& keyboard : mgr->GetKeyboards()) {
		int index = ImageList_Add(himl, keyboard.GetKeyboardIcon(), NULL);
		keyboard.imageListIndex = index;
	}

	ListView_SetImageList(hControl, himl, LVSIL_SMALL);

	for (auto& keyboard : mgr->GetKeyboards()) {
		InsertListViewItems(hControl, keyboard);
	}
}

void ShowTrayContextMenu(HWND hWnd)
{
	HMENU hMenu;
	hMenu = CreatePopupMenu();

	KeyboardManager *mgr = KeyboardManager::sharedManager();
	for (auto& keyboard : mgr->GetKeyboards())
	{
		TCHAR title[200] = { 0 };

		std::wstring name = keyboard.name;
		memcpy(title, name.c_str(), name.size() * sizeof(wchar_t));

		MENUITEMINFO mii = { 0 };
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.cch = name.size() * sizeof(wchar_t);
		mii.fMask = MIIM_STRING | MIIM_STATE | MIIM_ID | MIIM_BITMAP;
		mii.dwTypeData = title;
		mii.fState = MFS_UNCHECKED;
		mii.hbmpItem = keyboard.GetKeyboardIcon();
		if (mgr->SelectedKeyboard() == &keyboard)
		{
			mii.fState = MFS_CHECKED;
		}
		mii.wID = IDM_KEYBOARD_ + keyboard.index;

		InsertMenuItem(hMenu, IDM_KEYBOARD_ + keyboard.index, false, &mii);
	}

	MENUITEMINFO mii = { 0 };
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_TYPE;
	mii.fType = MFT_SEPARATOR;
	InsertMenuItem(hMenu, 0, false, &mii);

	AppendMenu(hMenu, MF_STRING, IDM_EXIT, _T("Exit"));

	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow(hWnd);
	TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_VERNEGANIMATION, pt.x, pt.y, 0, hWnd, NULL);
}

void ShellExecuteAndWait(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd)
{
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = hwnd;
	ShExecInfo.lpVerb = lpOperation;
	ShExecInfo.lpFile = lpFile;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.lpDirectory = lpDirectory;
	ShExecInfo.nShow = nShowCmd;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void UpdateMenuState(HWND hWnd)
{
	UINT state = MF_UNCHECKED;
	if (IsRegisteredForStartup())
	{
		state = MF_CHECKED;
	}

	CheckMenuItem(GetMenu(hWnd), IDM_OPTIONS_RUNATSTARTUP, MF_BYCOMMAND | state);

	json config = ConfigUtils::Read();
	state = config[ConfigKeyKeyboardPerWindow] ? MF_CHECKED : MF_UNCHECKED;

	CheckMenuItem(GetMenu(hWnd), IDM_OPTIONS_DIFFERENT_KEYBOARD, MF_BYCOMMAND | state);
}

bool ParseVersionString(unsigned digits[4], const std::string & i_version)
{
	return sscanf_s(i_version.c_str(), "%d.%d.%d.%d", &digits[0], &digits[1],
		&digits[2], &digits[3]) == 4;
}

void GetBinaryVersion(unsigned digits[4])
{
	DWORD  verHandle = 0;
	UINT   size = 0;
	LPBYTE lpBuffer = NULL;
	TCHAR szVersionFile[255];
	GetModuleFileName(NULL, szVersionFile, 255);
	DWORD  verSize = GetFileVersionInfoSize(szVersionFile, &verHandle);

	if (verSize != NULL)
	{
		LPSTR verData = new char[verSize];

		if (GetFileVersionInfo(szVersionFile, verHandle, verSize, verData))
		{
			if (VerQueryValue(verData, _T("\\"), (VOID FAR* FAR*)&lpBuffer, &size))
			{
				if (size)
				{
					VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
					if (verInfo->dwSignature == 0xfeef04bd)
					{
						digits[0] = (verInfo->dwFileVersionMS >> 16) & 0xffff;
						digits[1] = (verInfo->dwFileVersionMS >> 0) & 0xffff;
						digits[2] = (verInfo->dwFileVersionLS >> 16) & 0xffff;
						digits[3] = (verInfo->dwFileVersionLS >> 0) & 0xffff;
					}
				}
			}
		}
		delete[] verData;
	}
}

BOOL CheckForUpdate(BOOL * newVersionAvailable)
{
	TCHAR LocalCacheFilePath[MAX_PATH] = { 0 };
	LPTSTR Url = _T("http://storage.keymagic.net.s3-ap-southeast-1.amazonaws.com/releases/windows/latest/version.txt");
	
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	BOOL success = FALSE;
	*newVersionAvailable = FALSE;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(_T("KeyMagic/1.0"),
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, _T("storage.keymagic.net.s3-ap-southeast-1.amazonaws.com"),
			INTERNET_DEFAULT_HTTP_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect) {
		LPCWSTR types[] = { _T("*/*"), NULL };
		hRequest = WinHttpOpenRequest(hConnect, _T("GET"), _T("/releases/windows/latest/version.txt"),
			NULL, WINHTTP_NO_REFERER,
			types,
			WINHTTP_FLAG_REFRESH);
	}

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS, 0,
			WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		// Check for available data.
		dwSize = 0;
		if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
			DebugLog(_T("Error in WinHttpQueryDataAvailable : ") << GetLastError() << std::endl);

		// Allocate space for the buffer.
		pszOutBuffer = new char[dwSize + 1];
		if (!pszOutBuffer)
		{
			DebugLog(_T("Out of memory") << std::endl);
			dwSize = 0;
		}
		else
		{
			// Read the data.
			ZeroMemory(pszOutBuffer, dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
				dwSize, &dwDownloaded))
			{
				DebugLog(_T("Error in WinHttpReadData : ") << GetLastError() << std::endl);
			}
			else
			{
				printf("%s", pszOutBuffer);
				unsigned digitsRemote[4];
				if (ParseVersionString(digitsRemote, pszOutBuffer))
				{
					unsigned digitsCurrent[4];
					GetBinaryVersion(digitsCurrent);

					int result = memcmp(digitsCurrent, digitsRemote, sizeof(digitsCurrent));
					if (result < 0) {
						*newVersionAvailable = TRUE;
					}
					success = TRUE;
				}
			}

			// Free the memory allocated to the buffer.
			delete[] pszOutBuffer;
		}
	}


	// Report any errors.
	if (!bResults)
		DebugLog(_T("Error has occurred : ") << GetLastError() << std::endl);

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	return success;
}

void openGithubReleasePage()
{
	ShellExecute(NULL, _T("open"), _T("https://github.com/thantthet/keymagic/releases"), NULL, NULL, SW_SHOWNORMAL);
}

void CheckForUpdateAndNotify(HWND hWnd)
{
	BOOL available = FALSE;
	if (CheckForUpdate(&available) && available) {
		UpdateShellNotifyIcon(hWnd, NULL, _T("New version is available"));
		updateVersionNotificationWasShown = TRUE;
	}
}

void CheckForUpdateDialogEnabled(HWND hWnd)
{
	BOOL available = FALSE;
	if (!CheckForUpdate(&available))
	{
		MessageBox(hWnd, _T("There was an error while checking for update."), _T("Error"), MB_OK | MB_ICONERROR);
	}
	else if (available)
	{
		if (MessageBox(hWnd, _T("There is new version available. Go to download page?"), _T("Got new"), MB_YESNO | MB_ICONINFORMATION) == IDYES)
		{
			openGithubReleasePage();
		}
	}
	else
	{
		MessageBox(hWnd, _T("You are using latest version."), _T("Check Later"), MB_OK | MB_ICONINFORMATION);
	}
}

void SyncWindowMode()
{
	json config = ConfigUtils::Read();
	bool flag = config[ConfigKeyKeyboardPerWindow];
	KeyboardManager::sharedManager()->SetWindowMode(flag);
}

void ToggleKeyboardPerWindow()
{
	json config = ConfigUtils::Read();
	bool flag = config[ConfigKeyKeyboardPerWindow];
	
	flag = !flag;

	config[ConfigKeyKeyboardPerWindow] = flag;
	ConfigUtils::Write(config);

	SyncWindowMode();
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case MY_TRAY_ICON_MESSAGE:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowTrayContextMenu(hWnd);
			break;
		case NIN_BALLOONUSERCLICK:
			if (updateVersionNotificationWasShown)
			{
				openGithubReleasePage();
			}
			break;
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		if (wmId >= IDM_KEYBOARD_)
		{
			int index = wmId - IDM_KEYBOARD_;
			KeyboardManager * mgr = KeyboardManager::sharedManager();
			Keyboard * keyboard = mgr->KeyboardAtIndex(index);
			if (keyboard != nullptr)
			{
				mgr->SelectKeyboard(keyboard);
			}
		}
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_CHECKUPDATE:
			CheckForUpdateDialogEnabled(hWnd);
			break;
		case IDM_OPTIONS_HOTKEYS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HOTKEYS), hWnd, Hotkeys);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPTIONS_DIFFERENT_KEYBOARD:
			ToggleKeyboardPerWindow();
			UpdateMenuState(hWnd);
			break;
		case IDM_OPTIONS_RUNATSTARTUP:
		{
			UINT state = GetMenuState(GetMenu(hWnd), IDM_OPTIONS_RUNATSTARTUP, MF_BYCOMMAND);
			if (state == MF_CHECKED)
			{
				WCHAR buffer[MAX_PATH] = { 0 };
				GetModuleFileNameW(NULL, buffer, MAX_PATH);
				ShellExecuteAndWait(NULL, L"RunAs", buffer, L"/NoRunAtBoot", L"", 0);
			}
			else {
				WCHAR buffer[MAX_PATH] = { 0 };
				GetModuleFileNameW(NULL, buffer, MAX_PATH);
				ShellExecuteAndWait(NULL, L"RunAs", buffer, L"/runAtBoot", L"", 0);
			}
			
			UpdateMenuState(hWnd);
		}
			break;
		case IDC_BTN_ADD:
		{
			TCHAR fileName[MAX_PATH] = { 0 };
			OPENFILENAME ofn = { 0 };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hInstance = hInst;
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = _T("KeyMagic Keyboard File (*.km2)\0*.km2\0");
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn)) {
				AddKeyboardFile(hWnd, fileName);
			}
		}
			break;
		case IDC_BTN_REMOVE:
		{
			HWND hListView = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);

			std::list<int> selectedIndexes;

			int iPos = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			while (iPos != -1) {
				selectedIndexes.push_back(iPos);
				iPos = ListView_GetNextItem(hListView, iPos, LVNI_SELECTED);
			}

			if (selectedIndexes.size())
			{
				int result = MessageBox(hWnd, _T("Are you sure you want to remove selected keyboard(s)?"), _T("Y sure?"), MB_YESNO | MB_ICONEXCLAMATION);
				if (result == IDYES)
				{
					auto keyboards = KeyboardManager::sharedManager()->GetKeyboards();
					for (auto index : selectedIndexes) {
						UnregisterKeyboard(keyboards.at(index));
					}
					ReloadKeyboards(hWnd);
				}
			}
		}
			break;
		case IDC_BTN_BUG:
		{
			ShellExecute(0, 0, _T("https://github.com/thantthet/keymagic/issues"), 0, 0, SW_SHOW);
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
		icex.dwICC = ICC_LISTVIEW_CLASSES;
		InitCommonControlsEx(&icex);

		CreateListView(hWnd);
		CreateAddKeyboardButton(hWnd);
		CreateRemoveKeyboardButton(hWnd);
		CreateReportBugButton(hWnd);
		CreateLabel(hWnd);
		HICON icon = (HICON)LoadImage(hInst,
			MAKEINTRESOURCE(IDI_KEYMAGIC2),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR);
		CreateShellNotifyIcon(hWnd, icon, _T("Find me in notification tray!"));

		KeyboardManager *mgr = KeyboardManager::sharedManager();
		mgr->addOnKeyboardDidChangeHandler([hWnd, mgr]() {

			Keyboard * keyboard = mgr->SelectedKeyboard();

			HWND hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
			HIMAGELIST himl = ListView_GetImageList(hControl, LVSIL_SMALL);

			HICON icon;

			if (keyboard != nullptr) {
				HICON hIcon = ImageList_GetIcon(himl, keyboard->index, 0);
				icon = hIcon;
			}
			else {
				icon =
					(HICON)LoadImage(hInst,
						MAKEINTRESOURCE(IDI_KEYMAGIC2),
						IMAGE_ICON,
						GetSystemMetrics(SM_CXSMICON),
						GetSystemMetrics(SM_CYSMICON),
						LR_DEFAULTCOLOR);
			}
			
			UpdateShellNotifyIcon(hWnd, icon);
		});

		ReloadKeyboards(hWnd);
		UpdateMenuState(hWnd);
		SyncWindowMode();
		CheckForUpdateAndNotify(hWnd);
	}
	break;
	case WM_SIZE:
	{
		SizeListView(hWnd);
		SizeAddKeyboardButton(hWnd);
		SizeRemoveKeyboardButton(hWnd);
		SizeLabel(hWnd);
		SizeReportBugButton(hWnd);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		return false;
	case WM_DESTROY:
		DeleteShellNotifyIcon(hWnd);
		PostQuitMessage(0);
		break;

		// KeyMagic Messages
	case WM_COPYDATA:
	{
		PCOPYDATASTRUCT cds = (PCOPYDATASTRUCT)lParam;
		if (cds->dwData == 0x8855) {
			memcpy_s(KeyboardStates, 256, cds->lpData, cds->cbData);
		}
		break;
	}
	case KM_GOTFOCUS:
		
		break;
	case KM_LOSTFOCUS:

		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void InitHotkeyToRadioStates(HWND hDlg)
{
	std::map<UINT, WORD> OnOffControls = {
		{ IDC_RO_ONOFF_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
		{ IDC_RO_ONOFF_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
		{ IDC_RO_ONOFF_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
	};
	std::map<UINT, WORD> NextControls = {
		{ IDC_RO_NEXT_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
		{ IDC_RO_NEXT_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
		{ IDC_RO_NEXT_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
	};

	HotkeyManager *hmgr = HotkeyManager::sharedManager();

	bool checked = false;

	for (auto &pair : OnOffControls)
	{
		if (pair.second == hmgr->hky_onoff)
		{
			CheckDlgButton(hDlg, pair.first, true);
			checked = true;
		}
	}

	if (checked == false)
	{
		CheckDlgButton(hDlg, IDC_RO_ONOFF_CUSTOM, MF_CHECKED);
		SendMessage(GetDlgItem(hDlg, IDC_HK_ONOFF), HKM_SETHOTKEY, hmgr->hky_onoff, 0);
	}

	checked = false;

	for (auto &pair : NextControls)
	{
		if (pair.second == hmgr->hky_nextkbd)
		{
			CheckDlgButton(hDlg, pair.first, true);
			checked = true;
		}
	}

	if (checked == false)
	{
		CheckDlgButton(hDlg, IDC_RO_NEXT_CUSTOM, MF_CHECKED);
		EnableWindow(GetDlgItem(hDlg, IDC_HK_NEXT), true);
		SendMessage(GetDlgItem(hDlg, IDC_HK_NEXT), HKM_SETHOTKEY, hmgr->hky_nextkbd, 0);
	}
}

void ReloadRadioButtonStates(HWND hDlg)
{
	bool isChecked;

	// enable/disable custom hotkey control
	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_CUSTOM);
	EnableWindow(GetDlgItem(hDlg, IDC_HK_ONOFF), isChecked);

	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_NEXT_CUSTOM);
	EnableWindow(GetDlgItem(hDlg, IDC_HK_NEXT), isChecked);

	// get state
	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_SC);
	EnableWindow(GetDlgItem(hDlg, IDC_RO_NEXT_SC), !isChecked);
	if (isChecked) CheckDlgButton(hDlg, IDC_RO_NEXT_SC, false);

	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_CS);
	EnableWindow(GetDlgItem(hDlg, IDC_RO_NEXT_CS), !isChecked);
	if (isChecked) CheckDlgButton(hDlg, IDC_RO_NEXT_CS, false);

	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_WS);
	EnableWindow(GetDlgItem(hDlg, IDC_RO_NEXT_WS), !isChecked);
	if (isChecked) CheckDlgButton(hDlg, IDC_RO_NEXT_WS, false);
}

void SaveHotkeys(HWND hDlg)
{
	bool isChecked;

	WORD wOnOff, wNext;

	std::map<UINT, WORD> OnOffControls = {
		{IDC_RO_ONOFF_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
		{IDC_RO_ONOFF_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
		{IDC_RO_ONOFF_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
	};
	std::map<UINT, WORD> NextControls = {
		{ IDC_RO_NEXT_SC, MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL) },
		{ IDC_RO_NEXT_CS, MAKEWORD(VK_SPACE, HOTKEYF_CONTROL) },
		{ IDC_RO_NEXT_WS, MAKEWORD(VK_SPACE, HOTKEYF_EXT) }
	};

	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_ONOFF_CUSTOM);
	if (isChecked)
	{
		wOnOff = (WORD)SendMessage(GetDlgItem(hDlg, IDC_HK_ONOFF), HKM_GETHOTKEY, 0, 0);
	}
	else {
		for (auto &pair : OnOffControls)
		{
			isChecked = IsDlgButtonChecked(hDlg, pair.first);
			if (isChecked)
			{
				wOnOff = pair.second;
			}
		}
	}

	isChecked = IsDlgButtonChecked(hDlg, IDC_RO_NEXT_CUSTOM);
	if (isChecked)
	{
		wNext = (WORD)SendMessage(GetDlgItem(hDlg, IDC_HK_NEXT), HKM_GETHOTKEY, 0, 0);
	}
	else {
		for (auto &pair : NextControls)
		{
			isChecked = IsDlgButtonChecked(hDlg, pair.first);
			if (isChecked)
			{
				wNext = pair.second;
			}
		}
	}

	HotkeyManager * hmgr = HotkeyManager::sharedManager();
	hmgr->hky_onoff = wOnOff;
	hmgr->hky_nextkbd = wNext;
	hmgr->WriteHotkey();
}

INT_PTR CALLBACK Hotkeys(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_ACTIVATE:
		InitHotkeyToRadioStates(hDlg);
		ReloadRadioButtonStates(hDlg);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			SaveHotkeys(hDlg);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else {
			ReloadRadioButtonStates(hDlg);
		}
		break;
	}
	return (INT_PTR)FALSE;
}