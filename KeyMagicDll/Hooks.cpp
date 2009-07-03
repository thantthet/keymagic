#include "Hooks.h"
#include "shortcut.h"
#include "InputProcessor.h"

LRESULT CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	int index;

	if (nCode < 0)
	{
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	if (nCode == HC_ACTION && lParam & 0x80000000)
	{
		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
	}

	else if (nCode == HC_ACTION && lParam)
	{
		if (!GetFocus())
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		index = ShortCutCheck(wParam);

		if (index >= 0){
			if ( (ActiveIndex == index && isActive) || index == 0)
			{
				isActive = false;
				PostMessage(hwndKWindows, KM_GETFOCUS, 0, 0);
			}
			else{
				//LoadAndMap(index);
				PostMessage(hwndKWindows, KM_GETFOCUS, index, 0);
			}
			return 1;
		}

		if (isActive == false)
			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

		if (ProcessInput(wParam))
			return true;

		//if (wParam == VK_BACK)
		//{
		//	if (inner_back < 1)
		//	{
		//		//Logger("UnInnerBack wParam = %X lParam %X", wParam, lParam);
		//		if (BackCustomize())
		//			return 1;
		//		else {
		//			InternalEditor.Delete();
		//			return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		//		}
		//	}

		//	else if (inner_back)
		//	{
		//		//Logger("InnerBack wParam = %X lParam %X", wParam, lParam);
		//		inner_back--;
		//		return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
		//	}
		//}

		//if (TranslateToAscii((UINT*)&Input))
		//{
		//	//if (Do_Operation(Input))
		//	//	return true;
		//	InternalEditor.AppendText((wchar_t*)&Input, 1);
		//}
	}
	return CallNextHookEx(hKeyHook, nCode, wParam, lParam);

}

LRESULT CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	char Name[50];

	if (nCode < 0)
		return CallNextHookEx(hWndProcHook, nCode, wParam, lParam);

	CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
	
	switch (cwp->message)
	{
	case WM_ACTIVATE:
		if (LOWORD(cwp->wParam) == WA_INACTIVE){

			//kmCursorPos = 0;

			if (cwp->hwnd == hwndKWindows)
				break;

			GetClassName(cwp->hwnd, (LPSTR)Name, 50);

			if (!lstrcmp(Name,"Shell_TrayWnd"))
				break;
			if (!lstrcmp(Name, "TrayNotifyWnd"))
				break;

			PostMessage(hwndKWindows, KM_KILLFOCUS, 0,(LPARAM) cwp->hwnd);
#ifdef Debug
			InternalEditor.Restart();
#endif
		}

		if (LOWORD(cwp->wParam) == WA_ACTIVE){

			//kmCursorPos = 0;

			if (cwp->hwnd == hwndKWindows)
				break;

			GetClassName(cwp->hwnd, (LPSTR)Name, 50);
			
			if (!lstrcmp(Name,"Shell_TrayWnd"))
				break;
			if (!lstrcmp(Name, "TrayNotifyWnd"))
				break;

			if (isActive)
				PostMessage(hwndKWindows, KM_GETFOCUS, ActiveIndex,(LPARAM) cwp->hwnd);
			else if (!isActive)
				PostMessage(hwndKWindows, KM_GETFOCUS, 0, (LPARAM) cwp->hwnd);
		}
		break;

	case KM_SETKBID:
		isActive = cwp->lParam;
		if (isActive)
		{
			if (cwp->wParam == ActiveIndex)
			{
				isActive = false;
				ActiveIndex = -1;
			}
			else
			{
				//LoadAndMap(cwp->wParam);
				LoadKeymapFile(cwp->wParam);
				ActiveIndex = cwp->wParam;
			}
		}
		break;

	case KM_RESCAN:
		//LocalFree(vtSC);
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
	switch (msg->message){
		case WM_KEYDOWN:
			switch (msg->wParam)
			{
				case VK_LEFT:
				case VK_RIGHT:
				case VK_DOWN:
				case VK_UP:
					InternalEditor.Restart();
					break;
				default:
					InternalEditor.KeyDown(msg->wParam);
					break;
			}
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				InternalEditor.Restart();
				break;
			}
			break;
	}

	return CallNextHookEx(hGetMsgHook, nCode, wParam, lParam);
}