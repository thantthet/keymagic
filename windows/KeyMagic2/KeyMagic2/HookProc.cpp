#include "stdafx.h"
#include "HookProc.h"
#include <keymagic.h>
#include <iostream>
#include <array>
#include "KeyboardManager.h"
#include "HotkeyManager.h"
#include "../MagicAssit/MagicAssit.h"

HHOOK HH_KEYBOARD_LL;
HHOOK HH_MOUSE_LL;

HWINEVENTHOOK HWH_SYSTEM_FOREGROUND;

HWND ignoreHandleForegroundEvent;

using namespace libkm;

BYTE KeyboardStates[256];

void SendString(const KeyMagicString &);
void SendBackspace(ULONG);

struct ModifierKeyStates
{
	short SHIFT = 0;
	short MENU = 0;
	short CONTROL = 0;
	short WIN = 0;
};

ModifierKeyStates modKeyStates;

KeyMagicEngine * GetEngineForSelectedKeyboard()
{
	auto selectedKeyboard = KeyboardManager::sharedManager()->SelectedKeyboard();
	if (selectedKeyboard == nullptr)
	{
		return nullptr;
	}

	KeyMagicEngine *engine = selectedKeyboard->GetKeyMagicEngine();

	return engine;
}

bool ResetEngine()
{
	KeyMagicEngine *engine = GetEngineForSelectedKeyboard();
	if (engine)
	{
		engine->reset();
		return true;
	}
	return false;
}

LRESULT CALLBACK LowLevelMouseProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	if (nCode < 0) {
		return CallNextHookEx(HH_MOUSE_LL, nCode, wParam, lParam);
	}

	UINT mouseAction = wParam;
	switch (mouseAction)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	{
		ResetEngine();
	}
		break;
	default:
		break;
	}

	return CallNextHookEx(HH_MOUSE_LL, nCode, wParam, lParam);
}

DWORD handleNumpadKeys(DWORD sc)
{
	if (GetKeyState(VK_NUMLOCK) & 0xffff != 0) {
		switch (sc) {
		case 0x52: return VK_KEY_0;
		case 0x4f: return VK_KEY_1;
		case 0x50: return VK_KEY_2;
		case 0x51: return VK_KEY_3;
		case 0x4b: return VK_KEY_4;
		case 0x4c: return VK_KEY_5;
		case 0x4d: return VK_KEY_6;
		case 0x47: return VK_KEY_7;
		case 0x48: return VK_KEY_8;
		case 0x49: return VK_KEY_9;
		case 0x53: return VK_OEM_PERIOD;
		}
	}
	return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	if (nCode < 0) {
		return CallNextHookEx(HH_KEYBOARD_LL, nCode, wParam, lParam);
	}

	LPKBDLLHOOKSTRUCT kbd = (LPKBDLLHOOKSTRUCT)lParam;

	std::array<std::pair<std::vector<int>, short*>, 4> map = {
		std::pair<std::vector<int>, short*>({ VK_LSHIFT, VK_RSHIFT }, &modKeyStates.SHIFT),
		std::pair<std::vector<int>, short*>({ VK_LCONTROL, VK_RCONTROL }, &modKeyStates.CONTROL),
		std::pair<std::vector<int>, short*>({ VK_LMENU, VK_RMENU }, &modKeyStates.MENU),
		std::pair<std::vector<int>, short*>({ VK_LWIN, VK_RWIN }, &modKeyStates.WIN)
	};

	bool isModifierKey = false;

	for (auto &pair : map)
	{
		auto vks = pair.first;
		auto pointer = pair.second;

		for (auto vk: vks)
		{
			if (vk == kbd->vkCode)
			{
				*pointer = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? 0x80 : 0;
				isModifierKey = true;
			}
		}
	}

	if (wParam == WM_KEYDOWN)
	{
		HotkeyManager::sharedManager()->OnKeyDown(kbd->vkCode);
	}
	else if (wParam == WM_KEYUP)
	{
		HotkeyManager::sharedManager()->OnKeyUp(kbd->vkCode);
	}

	if ((kbd->flags & LLKHF_EXTENDED) == LLKHF_EXTENDED || // ignore function keys
		(kbd->flags & LLKHF_UP) == LLKHF_UP || // ignore key ups
		kbd->vkCode == VK_PACKET || // ignore packets
		kbd->dwExtraInfo == 0xDEADC0DE)
	{
		return CallNextHookEx(HH_KEYBOARD_LL, nCode, wParam, lParam);
	}

	BYTE states[256] = { 0 };

	states[VK_CONTROL] = modKeyStates.CONTROL;
	states[VK_SHIFT] = modKeyStates.SHIFT;
	states[VK_MENU] = modKeyStates.MENU;

	int modifier = 0;
	bool CTRL, ALT, SHIFT;
	if (CTRL = (states[VK_CONTROL] & 0x80) != 0)
	{
		modifier |= KeyMagicEngine::CTRL_MASK;
	}
	if (ALT = (states[VK_MENU] & 0x80) != 0)
	{
		modifier |= KeyMagicEngine::ALT_MASK;
	}
	if (SHIFT = (states[VK_SHIFT] & 0x80) != 0)
	{
		modifier |= KeyMagicEngine::SHIFT_MASK;
	}

	TCHAR unicode[2] = { 0 };

	states[VK_MENU] = 0; // set to zero for ToUnicodeEx

	UINT code = MapVirtualKeyEx(kbd->scanCode, MAPVK_VSC_TO_VK_EX, (HKL)0x04090409);
	if (isModifierKey) {
		// we dont want to pass value/code value for modifier keys
		code = 0;
	}
	else {
		DWORD vk = handleNumpadKeys(kbd->scanCode);
		if (vk) code = vk;
		ToUnicodeEx(code, kbd->scanCode, states, unicode, 1, 0, (HKL)0x04090409);
		states[code] = 0x80;
	}
	
	states[VK_MENU] = modKeyStates.MENU;

	KeyMagicEngine *engine = GetEngineForSelectedKeyboard();
	if (engine == nullptr)
	{
		return CallNextHookEx(HH_KEYBOARD_LL, nCode, wParam, lParam);
	}

	KeyMagicString contextBefore = engine->getContextText();
	KeyMagicString difference;

	BYTE emptyStates[256] = { 0 };
	engine->setKeyStates(states);

	if (engine->processKeyEvent(unicode[0], code, modifier)) {

		ULONG deleteCount = engine->getDifference(contextBefore, &difference);

		SendBackspace(deleteCount);
		SendString(difference);
		return true;
	}
	else if (unicode[0]) {
		ResetEngine();
	}
	return CallNextHookEx(HH_KEYBOARD_LL, nCode, wParam, lParam);
}

VOID WinEventHookProc(
	HWINEVENTHOOK hWinEventHook,
	DWORD         event,
	HWND          hwnd,
	LONG          idObject,
	LONG          idChild,
	DWORD         idEventThread,
	DWORD         dwmsEventTime) {

	if (event == EVENT_SYSTEM_FOREGROUND && ignoreHandleForegroundEvent != hwnd) {
		HWND hwnd = GetForegroundWindow();
		TCHAR name[256];
		if (GetWindowText(hwnd, name, 255) > 0) {
			KeyboardManager::sharedManager()->SetWindowHandle(hwnd);
		}
	}
}

void SendString(const KeyMagicString & s)
{
	int eventCount = s.size() * 2;

	if (eventCount == 0) {
		return;
	}

	INPUT *inputs = new INPUT[eventCount];

	for (int i = 0, ii = 0; ii < eventCount; i++, ii++)
	{
		inputs[ii].type = INPUT_KEYBOARD;
		inputs[ii].ki.dwExtraInfo = 0xDEADC0DE;
		inputs[ii].ki.dwFlags = KEYEVENTF_UNICODE;
		inputs[ii].ki.time = 0;
		inputs[ii].ki.wScan = s[i];
		inputs[ii].ki.wVk = 0;

		ii++;

		inputs[ii].type = INPUT_KEYBOARD;
		inputs[ii].ki.dwExtraInfo = 0;
		inputs[ii].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
		inputs[ii].ki.time = 0;
		inputs[ii].ki.wScan = s[i];
		inputs[ii].ki.wVk = 0;
	}

	SendInput(eventCount, inputs, sizeof(INPUT));

	free(inputs);
}

void SendBackspace(ULONG count)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.dwExtraInfo = 0xDEADC0DE;
	input.ki.time = 0;
	for (ULONG i = 0; i < count; i++)
	{
		input.ki.wScan = 255;
		input.ki.dwFlags = 0;
		input.ki.wVk = VK_BACK;
		SendInput(1, &input, sizeof(INPUT));

		input.ki.wScan = 0;
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		input.ki.wVk = VK_BACK;
		SendInput(1, &input, sizeof(INPUT));
	}
}

BOOL InitHooks(HWND mainHwnd)
{
	ignoreHandleForegroundEvent = mainHwnd;

	HH_KEYBOARD_LL = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, GetModuleHandle(NULL), NULL);
	HWH_SYSTEM_FOREGROUND = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, 0, &WinEventHookProc, 0, 0, WINEVENT_OUTOFCONTEXT);
#ifndef _DEBUG
	HH_MOUSE_LL = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, GetModuleHandle(NULL), NULL);
#endif

	HotkeyManager * hmgr = HotkeyManager::sharedManager();
	hmgr->AddOnHotkeyHandler([]() {
		KeyboardManager *mgr = KeyboardManager::sharedManager();
		mgr->AdvanceToNextKeyboard();
	}, &hmgr->hky_nextkbd);

	hmgr->AddOnHotkeyHandler([]() {
		KeyboardManager *mgr = KeyboardManager::sharedManager();
		mgr->ToggleKeyboard();
	}, &hmgr->hky_onoff);

	//HMODULE hModule = LoadLibrary(L"MagicAssit.dll");

	//Dll_SetMainWindowsHandle(mainHwnd);
	//Dll_SetWindowsHooks(hModule);

	return HH_KEYBOARD_LL != NULL;
}