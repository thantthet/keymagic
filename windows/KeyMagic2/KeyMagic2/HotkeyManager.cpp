#include "stdafx.h"
#include "HotkeyManager.h"
#include "ConfigUtils.h"

HotkeyManager::HotkeyManager(std::string jsonPath)
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

std::string HotkeyManager::wHotkeyToString(WORD hotkey)
{
	{
		std::string s;

		BYTE vk = LOBYTE(hotkey);
		BYTE mod = HIBYTE(hotkey);

		if (mod & HOTKEYF_ALT)
		{
			s += "Alt+";
		}
		if (mod & HOTKEYF_CONTROL)
		{
			s += "Ctrl+";
		}
		if (mod & HOTKEYF_SHIFT)
		{
			s += "Shift+";
		}
		if (mod & HOTKEYF_EXT)
		{
			s += "Win+";
		}

		if (vk)
		{
			s += (char)MapVirtualKey(vk, MAPVK_VK_TO_CHAR);
		}

		if (s.back() == '+') {
			s.pop_back();
		}

		if (s.back() == ' ') {
			s.pop_back();
			s += "Space";
		}

		return s;
	}
}