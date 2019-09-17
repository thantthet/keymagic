#pragma once

#include <string>
#include <functional>
#include <list>
#include "ConfigUtils.h"

#define kSHIFT_CONTROL	1
#define kCONTROL_SPACE	2
#define kWIN_SPACE		3

class HotkeyManager
{
public:
	static HotkeyManager * sharedManager()
	{
		static HotkeyManager * sharedManager = nullptr;
		if (sharedManager == nullptr) {
			sharedManager = new HotkeyManager(ConfigUtils::jsonFilePath());
			sharedManager->ReloadHotkey();
		}
		return sharedManager;
	}

	HotkeyManager(std::wstring jsonPath);

	void WriteHotkey();
	void ReloadHotkey();

	static std::string funcKeyNameForVK(BYTE vkCode);
	static std::string wHotkeyToString(WORD hotkey);

	void AddOnHotkeyHandler(std::function<void()> callback, LPWORD hotkeyPointer)
	{
		m_handlers[hotkeyPointer] = callback;
	}

	void OnKeyDown(int key);
	void OnKeyUp(int key);

	// match hotkeys and call callback when found
	bool MatchAndCall();

private:
	void ResetKeys();
	bool MatchHotkey(WORD hky);

private:
	std::wstring jsonPath;
	std::map<LPWORD, std::function<void()>> m_handlers;
	bool ctrl, alt, shift, win;
	int vk;
public:
	WORD hky_onoff;
	WORD hky_nextkbd;
};