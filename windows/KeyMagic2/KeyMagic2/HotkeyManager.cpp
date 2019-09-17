#include "stdafx.h"
#include "HotkeyManager.h"
#include "ConfigUtils.h"

HotkeyManager::HotkeyManager(std::wstring jsonPath)
	: jsonPath(jsonPath)
{
	ResetKeys();
}

void HotkeyManager::OnKeyDown(int key)
{
	if (key == VK_LSHIFT || key == VK_RSHIFT)
	{
		shift = true;
	}
	else if (key == VK_LCONTROL || key == VK_RCONTROL)
	{
		ctrl = true;
	}
	else if (key == VK_LMENU || key == VK_RMENU)
	{
		alt = true;
	}
	else if (key == VK_LWIN || key == VK_RWIN)
	{
		win = true;
	}
	else {
		vk = key;
	}
}

void HotkeyManager::OnKeyUp(int key)
{
	// match on key up
	MatchAndCall();
	// discard key states no matter what
	ResetKeys();
}

bool HotkeyManager::MatchAndCall()
{
	std::list<LPWORD> hkys = { &hky_onoff, &hky_nextkbd };
	for (auto & hky : hkys) {
		if (MatchHotkey(*hky))
		{
			if (m_handlers.find(hky) != m_handlers.end()) {
				m_handlers[hky]();
			}
			return true;
		}
	}
	return false;
}

bool HotkeyManager::MatchHotkey(WORD hky)
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

void HotkeyManager::ResetKeys()
{
	ctrl = false;
	alt = false;
	shift = false;
	win = false;
	vk = 0;
}

void HotkeyManager::WriteHotkey()
{
	json config = ConfigUtils::Read();
	json hotkeys;

	if (config.find("hotkeys") != config.end())
	{
		hotkeys = config["hotkeys"];
	}
	else {
		hotkeys = json::object();
	}

	hotkeys["onoff"] = hky_onoff;
	hotkeys["next"] = hky_nextkbd;

	config["hotkeys"] = hotkeys;

	ConfigUtils::Write(config);
}

void HotkeyManager::ReloadHotkey()
{
	json config = ConfigUtils::Read();
	json hotkeys;

	hky_onoff = 0;
	hky_nextkbd = 0;

	if (config.find("hotkeys") != config.end())
	{
		hotkeys = config["hotkeys"];
		hky_onoff = hotkeys["onoff"];
		hky_nextkbd = hotkeys["next"];
	}
	else {
		hky_onoff = MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL);
		hky_nextkbd = MAKEWORD(VK_SPACE, HOTKEYF_CONTROL); 
	}
}


std::string HotkeyManager::funcKeyNameForVK(BYTE vkCode)
{
	if (vkCode >= VK_F1 && vkCode <= VK_F24) {
		int number = (vkCode - VK_F1) + 1;
		auto name = std::string("F") + std::to_string(number);
		return name;
	}
	return std::string();
}

std::string HotkeyManager::wHotkeyToString(WORD hotkey)
{
	{
		std::stringstream ss;

		BYTE vk = LOBYTE(hotkey);
		BYTE mod = HIBYTE(hotkey);

		if (mod & HOTKEYF_ALT)
		{
			ss << "Alt+";
		}
		if (mod & HOTKEYF_CONTROL)
		{
			ss << "Ctrl+";
		}
		if (mod & HOTKEYF_SHIFT)
		{
			ss << "Shift+";
		}
		if (mod & HOTKEYF_EXT)
		{
			ss << "Win+";
		}

		if (vk)
		{
			if (auto ch = (char)MapVirtualKey(vk, MAPVK_VK_TO_CHAR)) {
				ss << ch;
			}
			else {
				ss << funcKeyNameForVK(vk);
			}
		}

		std::string s = ss.str();

		if (s.back() == '+')
{
			s.pop_back();
		}
		else if (s.back() == ' ')
		{
			s.pop_back();
			s += "Space";
		}

		return s;
	}
}