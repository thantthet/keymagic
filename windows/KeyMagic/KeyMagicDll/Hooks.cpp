#include "Hooks.h"
#include "shortcut.h"
#include "InputProcessor.h"

//HKL hkl;
//char KLID[KL_NAMELENGTH];

LRESULT CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	int index;

	if (nCode < 0){
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	//Key Up
	if (nCode == HC_ACTION && lParam & 0x80000000)
	{
#ifdef _DEBUG
		Debug(L"HookKeyProc::Key Up - lParam = 0x%.8x wParam = 0x%.8x\n", lParam, wParam);
#endif
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}
	//Menu Mode is active
	if (nCode == HC_ACTION && (lParam & 0x10000000)){
		Debug(L"Menu Mode ACTIVE\n");
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}
	//Key Down
	else if (nCode == HC_ACTION && lParam)
	{
#ifdef _DEBUG
		Debug(L"HookKeyProc::Key Down - lParam = 0x%.8x wParam = 0x%.8x\n", lParam, wParam);
#endif
		//If there is no focus
		if (!GetFocus()) {
#ifdef _DEBUG
			Debug(L"No Focus\n");
#endif
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		}

		index = ShortCutCheck(wParam);

		BYTE KeyStatus[256];
		GetKeyboardState(KeyStatus);

		// If ActiveWindow is not KeyMagic Application
		for (hwndExc::iterator it = hwndExceptions.begin(); it != hwndExceptions.end(); it++){
			if (GetFocus() == (HWND)*it){
				index = -1;
				break;
			}
		}

		if (index >= 0){
			if ( (ActiveIndex == index && isActive) || index == 0) {
				isActive = false;
				PostMessage(hwndKWindows, KM_GETFOCUS, 0, 0);
				//UnloadKeyboardLayout(hkl);
				//if (KLID[0])
				//	LoadKeyboardLayout(KLID, KLF_ACTIVATE | KLF_SETFORPROCESS );
			}
			else {
				if (LoadKeymapFile(index)){
					isActive = true;
					ActiveIndex = index;
					PostMessage(hwndKWindows, KM_GETFOCUS, ActiveIndex, 0);
				} else {
					PostMessage(hwndKWindows, KM_ERR_KBLOAD, index, 0);
				}
				InternalEditor.Reset();
				//GetKeyboardLayoutName(KLID);
				//hkl = LoadKeyboardLayout("00000409", KLF_ACTIVATE | KLF_SETFORPROCESS);
			}
			return 1;
		} else {
			//ON/OFF
			if (KeyStatus[VK_CONTROL] & 0x80 && KeyStatus[VK_SPACE] & 0x80) {
				isActive = !isActive;
				if (isActive && ActiveIndex == -1) {
					ActiveIndex = 1;
					PostMessage(hwndKWindows, KM_GETFOCUS, ActiveIndex, 0);
					LoadKeymapFile(ActiveIndex);
				}

				if (isActive) {
					PostMessage(hwndKWindows, KM_GETFOCUS, ActiveIndex, 0);
				} else {
					PostMessage(hwndKWindows, KM_GETFOCUS, 0, 0);
				}

				return true;
			}
		}

		if (isActive == false)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		InternalEditor.undoBegin();
		bool handled = ProcessInput(wParam, lParam);
		InternalEditor.undoEnd();
		if (handled) {
			if (wParam == VK_BACK)
			{
				InternalEditor.undoReset();
			}
			return true;
		} else if (klf.layout.autoBksp == true && wParam == VK_BACK && (HIWORD(lParam) & 0xff) != 0xff) {
			return Undo();
		}
	}
	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

LRESULT CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	TCHAR Name[50];
	bool bSuccess = false;

	if (nCode < 0)
		return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);

	CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
	
	switch (cwp->message)
	{
	case WM_ACTIVATE: // we need to keep track of activity/inactivity of windows
					// as we need to select active one in layout menu
		if (LOWORD(cwp->wParam) == WA_INACTIVE){

			if (cwp->hwnd == hwndKWindows)
				break;

			GetClassName(cwp->hwnd, (LPTSTR)Name, 50);

			if (!lstrcmp(Name, TEXT("Shell_TrayWnd")))
				break;
			if (!lstrcmp(Name, TEXT("TrayNotifyWnd")))
				break;
			if (!lstrcmp(Name, TEXT("NotifyIconOverflowWindow")) )
				break;

			PostMessage(hwndKWindows, KM_KILLFOCUS, 0,(LPARAM) cwp->hwnd);
#ifdef _TRACK_WND_RPOC_
			InternalEditor.Reset();
#endif
		}

		if (LOWORD(cwp->wParam) == WA_ACTIVE || LOWORD(cwp->wParam) == WA_CLICKACTIVE){

			// if main keymagic windows
			if (cwp->hwnd == hwndKWindows)
				break;

			GetClassName(cwp->hwnd, (LPTSTR)Name, 50);
			
			// if this windows is tray windows
			if (!lstrcmp(Name, TEXT("Shell_TrayWnd")) )
				break;
			if (!lstrcmp(Name, TEXT("TrayNotifyWnd")) )
				break;
			// more windows in win7
			if (!lstrcmp(Name, TEXT("NotifyIconOverflowWindow")) )
				break;

			// if one of any layouts is active/using
			if (isActive)
				PostMessage(hwndKWindows, KM_GETFOCUS, ActiveIndex,(LPARAM) cwp->hwnd);
			else if (!isActive)
				PostMessage(hwndKWindows, KM_GETFOCUS, 0, (LPARAM) cwp->hwnd);
		}
		break;

	case KM_SETKBID: // notify from main keymagic windows when user select layout from menu
		isActive = cwp->lParam;

		if (isActive) {
			if (cwp->wParam == ActiveIndex) {
				isActive = false;
				ActiveIndex = -1;
			}
			else {
				bSuccess = LoadKeymapFile(cwp->wParam);
				if (bSuccess){
					ActiveIndex = cwp->wParam;
					isActive = true;
				} else {
					if (ActiveIndex == -1)
						isActive = false;
					PostMessage(hwndKWindows, KM_ERR_KBLOAD, 0, 0); // notify failed to load layout
					PostMessage(hwndKWindows, KM_GETFOCUS, 0, (LPARAM) cwp->hwnd);
				}
				InternalEditor.Reset();
			}
		}
		else {
			isActive = false;
			ActiveIndex = -1;
			bSuccess = true;
		}

		break;

	case KM_RESCAN: // notify from main keymagic windows when hotkeys are changed
		vtSC.clear();
		GetShortCuts();
	}

	return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);
}


LRESULT CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);

	MSG* msg = (MSG*)lParam;
	switch (msg->message)
	{
#ifdef _TRACK_WND_RPOC_
		case WM_KEYDOWN:
			if (isActive)
				InternalEditor.KeyDown(msg->wParam, msg->lParam);
			break;
		case WM_KEYUP:
			if (isActive)
				InternalEditor.KeyUp(msg->wParam, msg->lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				if (isActive)
					InternalEditor.Reset();
				break;
			}
			break;
#endif
	}

	return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
}