#include "stdafx.h"
#include "HotkeyManager.h"
#include "ConfigUtils.h"

HotkeyManager::HotkeyManager(std::wstring jsonPath)
	: jsonPath(jsonPath)
{

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