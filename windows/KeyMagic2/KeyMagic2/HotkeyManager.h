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

	// match hotkeys and call callback when found
	bool MatchAndCall(bool ctrl, bool alt, bool shift, bool win, char vk)
	{
		std::list<LPWORD> hkys = { &hky_onoff, &hky_nextkbd };
		for (auto & hky : hkys) {
			if (MatchHotkey(ctrl, alt, shift, win, vk, *hky))
			{
				if (m_handlers.find(hky) != m_handlers.end()) {
					m_handlers[hky]();
				}
				return true;
			}
		}
		return false;
	}

private:
	bool MatchHotkey(bool ctrl, bool alt, bool shift, bool win, char vk, WORD hky)
	{
		BYTE vk2 = LOBYTE(hky);
		BYTE mod = HIBYTE(hky);

		if (ctrl != ((mod & HOTKEYF_CONTROL) == HOTKEYF_CONTROL))
		{
			return false;
		}

		if (alt != ((mod & HOTKEYF_ALT) == HOTKEYF_ALT))
		{
			return false;
		}

		if (shift != ((mod & HOTKEYF_SHIFT) == HOTKEYF_SHIFT))
		{
			return false;
		}

		if (win != ((mod & HOTKEYF_EXT) == HOTKEYF_EXT))
		{
			return false;
		}

		if (vk != vk2) {
			return false;
		}

		return true;
	}

private:
	std::wstring jsonPath;
	std::map<LPWORD, std::function<void()>> m_handlers;
public:
	WORD hky_onoff;
	WORD hky_nextkbd;
};