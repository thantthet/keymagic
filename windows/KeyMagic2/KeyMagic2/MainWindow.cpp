#include "stdafx.h"

#include <shellapi.h>
#include <Shlwapi.h>
#include <Commdlg.h>

#include "MainWindow.h"

#include "HotkeyManager.h"
#include "NotifyIcon.h"
#include "UpdateChecker.h"
#include "HotkeysDialog.h"
#include "Tasker.h"
#include "dpi.h"

#define IDM_KEYBOARD_ 0x5000

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

WCHAR MainWindow::szWindowClass[MAX_LOADSTRING];

void MainWindow::ShowTrayContextMenu()
{
	HMENU hMenu;
	hMenu = CreatePopupMenu();

	KeyboardManager* mgr = KeyboardManager::sharedManager();
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

void MainWindow::OpenGithubReleasePage()
{
	ShellExecute(NULL, _T("open"), _T("https://github.com/thantthet/keymagic/releases"), NULL, NULL, SW_SHOWNORMAL);
}

HFONT MainWindow::GetFont() {
	LOGFONT lf = CreateDPILogFont(hWnd);
	if (bodyFont == NULL) {
		bodyFont = CreateFontIndirect(&lf);
	}
	return bodyFont;
};

HINSTANCE MainWindow::GetInstance()
{
	return GetModuleHandle(NULL);
}

void MainWindow::CheckForUpdateAndNotify()
{
	BOOL available = FALSE;
	if (UpdateChecker::CheckForUpdate(&available) && available) {
		UpdateShellNotifyIcon(hWnd, NULL, _T("New version is available"));
		updateVersionNotificationWasShown = TRUE;
	}
}

void MainWindow::CheckForUpdateDialogEnabled()
{
	BOOL available = FALSE;
	if (!UpdateChecker::CheckForUpdate(&available))
	{
		MessageBox(hWnd, _T("There was an error while checking for update."), _T("Error"), MB_OK | MB_ICONERROR);
	}
	else if (available)
	{
		if (MessageBox(hWnd, _T("There is new version available. Go to download page?"), _T("Got new"), MB_YESNO | MB_ICONINFORMATION) == IDYES)
		{
			OpenGithubReleasePage();
		}
	}
	else
	{
		MessageBox(hWnd, _T("You are using latest version."), _T("Check Later"), MB_OK | MB_ICONINFORMATION);
	}
}

void MainWindow::OnKeyboardDidChange(bool) {

	KeyboardManager* mgr = KeyboardManager::sharedManager();
	Keyboard* keyboard = mgr->SelectedKeyboard();

	HWND hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
	HIMAGELIST himl = ListView_GetImageList(hControl, LVSIL_SMALL);

	HICON icon;

	if (keyboard != nullptr) {
		HICON hIcon = ImageList_GetIcon(himl, keyboard->index, 0);
		icon = hIcon;
	}
	else {
		icon =
			(HICON)LoadImage(GetInstance(),
				MAKEINTRESOURCE(IDI_KEYMAGIC2),
				IMAGE_ICON,
				GetSystemMetrics(SM_CXSMICON),
				GetSystemMetrics(SM_CYSMICON),
				LR_DEFAULTCOLOR);
	}

	UpdateShellNotifyIcon(hWnd, icon);
}

LRESULT MainWindow::Dispatch(UINT message, WPARAM wParam, LPARAM lParam) {
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
			ShowTrayContextMenu();
			break;
		case NIN_BALLOONUSERCLICK:
			if (updateVersionNotificationWasShown)
			{
				OpenGithubReleasePage();
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
			KeyboardManager* mgr = KeyboardManager::sharedManager();
			Keyboard* keyboard = mgr->KeyboardAtIndex(index);
			if (keyboard != nullptr)
			{
				mgr->SelectKeyboard(keyboard);
			}
		}
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(GetInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_CHECKUPDATE:
			CheckForUpdateDialogEnabled();
			break;
		case IDM_OPTIONS_HOTKEYS:
			(new HotkeysDialog(hWnd))->Show();
			SetHotKeyTexts();
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPTIONS_DIFFERENT_KEYBOARD:
			ToggleKeyboardPerWindow();
			UpdateMenuState();
			break;
		case IDM_OPTIONS_RUNATSTARTUP:
		{
			UINT state = GetMenuState(GetMenu(hWnd), IDM_OPTIONS_RUNATSTARTUP, MF_BYCOMMAND);
			if (state == MF_CHECKED)
			{
				WCHAR buffer[MAX_PATH] = { 0 };
				GetModuleFileNameW(NULL, buffer, MAX_PATH);
				ShellExecuteAndWait(L"RunAs", buffer, L"/NoRunAtBoot", L"", 0);
			}
			else {
				WCHAR buffer[MAX_PATH] = { 0 };
				GetModuleFileNameW(NULL, buffer, MAX_PATH);
				ShellExecuteAndWait(L"RunAs", buffer, L"/runAtBoot", L"", 0);
			}

			UpdateMenuState();
		}
		break;
		case IDC_BTN_ADD:
		{
			TCHAR fileName[MAX_PATH] = { 0 };
			OPENFILENAME ofn = { 0 };
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hInstance = GetInstance();
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = _T("KeyMagic Keyboard File (*.km2)\0*.km2\0");
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn)) {
				AddKeyboardFile(fileName);
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
					ReloadKeyboards();
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

		CreateListView();
		CreateAddKeyboardButton();
		CreateRemoveKeyboardButton();
		CreateReportBugButton();
		CreateLabel();
		SetHotKeyTexts();
		HICON icon = (HICON)LoadImage(GetInstance(),
			MAKEINTRESOURCE(IDI_KEYMAGIC2),
			IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON),
			GetSystemMetrics(SM_CYSMICON),
			LR_DEFAULTCOLOR);
		CreateShellNotifyIcon(hWnd, icon, _T("Find me in notification tray!"));

		KeyboardManager* mgr = KeyboardManager::sharedManager();
		mgr->addOnKeyboardDidChangeHandler([this](bool flag) { OnKeyboardDidChange(flag); });

		ReloadKeyboards();
		UpdateMenuState();
		SyncWindowMode();
		CheckForUpdateAndNotify();
	}
	break;
	case WM_SIZE:
	{
		SizeListView();
		SizeAddKeyboardButton();
		SizeRemoveKeyboardButton();
		SizeLabel();
		SizeReportBugButton();
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
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


BOOL MainWindow::InsertListViewItems(HWND hWndListView, Keyboard keyboard)
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

BOOL MainWindow::InitListViewColumns(HWND hWndListView)
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
		LoadString(GetInstance(),
			IDS_LVC_FIRST + iCol,
			szText,
			sizeof(szText) / sizeof(szText[0]));

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;
	}

	return TRUE;
}

BOOL MainWindow::CreateListView()
{
	HWND hControl;

	hControl = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW,
		_T(""),
		WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_HSCROLL | WS_VSCROLL,
		0, 0, 0, 0,
		hWnd,
		(HMENU)IDC_LV_KEYBOARDS,
		GetInstance(),
		NULL);
	if (hControl == NULL) {
		MessageBox(hWnd, _T("Could not create list view."), _T("Error"), MB_OK | MB_ICONERROR);
		return false;
	}

	SendMessage(hControl, WM_SETFONT, (WPARAM)GetFont(), MAKELPARAM(FALSE, 0));

	InitListViewColumns(hControl);

	return true;
}

HWND MainWindow::CreateButton(LPTSTR title, HMENU buttonId)
{
	HWND hControl;

	hControl = CreateWindow(WC_BUTTON,
		title,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		0, 0, 0, 0,
		hWnd,
		buttonId,
		GetInstance(),
		NULL);

	if (hControl == NULL) {
		MessageBox(hWnd, _T("Could not create button."), _T("Error"), MB_OK | MB_ICONERROR);
	}

	SendMessage(hControl, WM_SETFONT, (WPARAM)GetFont(), MAKELPARAM(FALSE, 0));

	return hControl;
}

HWND MainWindow::CreateAddKeyboardButton()
{
	HWND hControl = CreateButton(_T("Add"), (HMENU)IDC_BTN_ADD);

	return hControl;
}

HWND MainWindow::CreateRemoveKeyboardButton()
{
	HWND hControl = CreateButton(_T("Remove"), (HMENU)IDC_BTN_REMOVE);

	return hControl;
}

HWND MainWindow::CreateReportBugButton()
{
	HWND hControl = CreateButton(_T("Report Bug"), (HMENU)IDC_BTN_BUG);

	return hControl;
}

HWND MainWindow::CreateLabel()
{
	HWND hControl = CreateWindow(_T("static"), _T("ST_U"),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		0, 0, 0, 0,
		hWnd, (HMENU)IDC_LABEL,
		GetInstance(),
		NULL);

	SendMessage(hControl, WM_SETFONT, (WPARAM)GetFont(), MAKELPARAM(FALSE, 0));

	return hControl;
}

void MainWindow::SetHotKeyTexts()
{
	std::wstring text;
	auto manager = HotkeyManager::sharedManager();

	if (manager->hky_onoff) {
		text += converter.from_bytes(HotkeyManager::wHotkeyToString(manager->hky_onoff));
		text += _T(" = Enable/Disable KeyMagic.\n\n");
	}

	if (manager->hky_nextkbd) {
		text += converter.from_bytes(HotkeyManager::wHotkeyToString(manager->hky_nextkbd));
		text += _T(" = Choose next keyboard.");
	}
	SetWindowText(GetDlgItem(hWnd, IDC_LABEL), text.c_str());
}

void MainWindow::SizeListView()
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

void MainWindow::SizeAddKeyboardButton()
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

void MainWindow::SizeRemoveKeyboardButton()
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

void MainWindow::SizeReportBugButton()
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

void MainWindow::SizeLabel()
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
		100,
		SWP_NOZORDER);
}

void MainWindow::RegisterKeyboardFile(LPCTSTR fileName)
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

void MainWindow::UnregisterKeyboard(Keyboard& keyboard)
{
	std::wstring dirName = dirname(ConfigUtils::JsonFilePath());

	json config = ConfigUtils::Read();
	json& j = config[ConfigKeyKeyboards];
	std::string path = converter.to_bytes(keyboard.path);
	for (auto it = j.begin(); it != j.end(); ++it) {
		auto& k = *it;
		if (k["path"].get<std::string>() == path)
		{
			j.erase(it);

			ConfigUtils::Write(config);

			DeleteFile((dirName + keyboard.path).c_str());

			return;
		}
	}
}

void MainWindow::AddKeyboardFile(LPCTSTR filePath)
{
	std::wstring basename = base_name<std::wstring>(filePath);

	std::wstring destFilePath = (AppDataDirectory() + basename);

	if (PathFileExists(destFilePath.c_str())) {
		int result = MessageBox(hWnd, _T("File with the same name already exits. Do you want to overwrite?"), _T("Y sure?"), MB_YESNO | MB_ICONWARNING);
		if (result == IDYES)
		{
			DeleteFile(destFilePath.c_str());
			AddKeyboardFile(filePath);
		}
	}
	else if (CopyFile(filePath, destFilePath.c_str(), true)) {
		RegisterKeyboardFile(basename.c_str());
		ReloadKeyboards();
	}
	else {
		MessageBox(NULL, _T("Copying failed while registering keyboard!"), _T("Oh Snap!"), MB_OK | MB_ICONERROR);
	}
}

void MainWindow::ReloadKeyboards()
{
	json config = ConfigUtils::Read();

	HWND hControl = GetDlgItem(hWnd, IDC_LV_KEYBOARDS);
	SendMessage(hControl, LVM_DELETEALLITEMS, 0, 0); // clear list view items

	KeyboardManager* mgr = KeyboardManager::sharedManager();
	json keyboards = config[ConfigKeyKeyboards];
	mgr->basePath(dirname(ConfigUtils::JsonFilePath()));
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

void MainWindow::ShellExecuteAndWait(LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd)
{
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = hWnd;
	ShExecInfo.lpVerb = lpOperation;
	ShExecInfo.lpFile = lpFile;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.lpDirectory = lpDirectory;
	ShExecInfo.nShow = nShowCmd;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void MainWindow::UpdateMenuState()
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

void MainWindow::SyncWindowMode()
{
	json config = ConfigUtils::Read();
	bool flag = config[ConfigKeyKeyboardPerWindow];
	KeyboardManager::sharedManager()->SetWindowMode(flag);
}

void MainWindow::ToggleKeyboardPerWindow()
{
	json config = ConfigUtils::Read();
	bool flag = config[ConfigKeyKeyboardPerWindow];

	flag = !flag;

	config[ConfigKeyKeyboardPerWindow] = flag;
	ConfigUtils::Write(config);

	SyncWindowMode();
}