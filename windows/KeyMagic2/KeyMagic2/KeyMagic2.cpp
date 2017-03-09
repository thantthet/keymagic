// KeyMagic2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "KeyMagic2.h"
#include <CommCtrl.h>
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
#include "../MagicAssit/MagicAssit.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
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

	lstrcpyn(text, converter.from_bytes(keyboard.name).c_str(), 200);

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

#define MY_TRAY_ICON_ID 999
#define MY_TRAY_ICON_MESSAGE WM_APP + 999

void CreateShellNotifyIcon(HWND hWnd)
{
	NOTIFYICONDATA ni = { 0 };
	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.uID = MY_TRAY_ICON_ID;
	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	ni.hIcon =
		(HICON)LoadImage(hInst,
			MAKEINTRESOURCE(IDI_KEYMAGIC2),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR);
	ni.hWnd = hWnd;
	ni.dwInfoFlags = NIIF_NOSOUND;
	lstrcpy(ni.szTip, _T("KeyMagic"));
	lstrcpy(ni.szInfo, _T("Find me in notification tray!"));
	ni.uCallbackMessage = MY_TRAY_ICON_MESSAGE;

	Shell_NotifyIcon(NIM_ADD, &ni);
}

void SetShellNotifyIcon(HWND hWnd)
{
	KeyboardManager *mgr = KeyboardManager::sharedManager();
	Keyboard * keyboard = mgr->SelectedKeyboard();

	HWND hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
	HIMAGELIST himl = ListView_GetImageList(hControl, LVSIL_SMALL);

	NOTIFYICONDATA ni = { 0 };
	ni.cbSize = sizeof(NOTIFYICONDATA);
	ni.uID = MY_TRAY_ICON_ID;
	ni.uFlags = NIF_ICON;
	ni.hWnd = hWnd;
	if (keyboard != nullptr) {
		HICON hIcon = ImageList_GetIcon(himl, keyboard->index, 0);
		ni.hIcon = hIcon;
	}
	else {
		ni.hIcon =
			(HICON)LoadImage(hInst,
				MAKEINTRESOURCE(IDI_KEYMAGIC2),
				IMAGE_ICON,
				GetSystemMetrics(SM_CXSMICON),
				GetSystemMetrics(SM_CYSMICON),
				LR_DEFAULTCOLOR);
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

	json keyboards = config["keyboards"];
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

	config["keyboards"] = keyboards;

	ConfigUtils::Write(config);
}

void UnregisterKeyboard(Keyboard &keyboard)
{
	std::string dirName = dirname(ConfigUtils::jsonFilePath());

	json config = ConfigUtils::Read();
	json &j = config["keyboards"];
	for (auto it = j.begin(); it != j.end(); ++it) {
		auto &k = *it;
		if (k["path"].get<std::string>() == keyboard.path)
		{
			j.erase(it);

			ConfigUtils::Write(config);

			DeleteFileA((dirName + keyboard.path).c_str());

			return;
		}
	}
}

void AddKeyboardFile(HWND hWnd, LPCTSTR filePath)
{
	std::wstring basename = base_name<std::wstring>(filePath);
	if (CopyFile(filePath, (AppDataDirectory() + basename).c_str(), true)) {
		RegisterKeyboardFile(hWnd, basename.c_str());
		ReloadKeyboards(hWnd);
	}
	else {
		MessageBox(NULL, _T("Copying failed while registering keyboard!"), _T("Oh Snap!"), MB_OK);
	}
}

void ReloadKeyboards(HWND hWnd)
{
	json config = ConfigUtils::Read();

	HWND hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
	SendMessage(hControl, LVM_DELETEALLITEMS, 0, 0); // clear list view items

	KeyboardManager *mgr = KeyboardManager::sharedManager();
	json keyboards = config["keyboards"];
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

		std::wstring wide = converter.from_bytes(keyboard.name);
		memcpy(title, wide.c_str(), wide.size() * sizeof(wchar_t));

		MENUITEMINFO mii = { 0 };
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.cch = wide.size() * sizeof(wchar_t);
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
		case ID_OPTIONS_HOTKEYS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HOTKEYS), hWnd, Hotkeys);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
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
				int result = MessageBox(hWnd, _T("Are you sure you want to remove selected keyboard(s)?"), _T("Y sure?"), MB_YESNO);
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
		CreateShellNotifyIcon(hWnd);

		KeyboardManager *mgr = KeyboardManager::sharedManager();
		mgr->addOnKeyboardDidChangeHandler([hWnd]() {
			SetShellNotifyIcon(hWnd);
		});

		ReloadKeyboards(hWnd);

		UpdateMenuState(hWnd);
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