#pragma once

#include "json.hpp"
#include <codecvt>
#include <Shlobj.h>
#include <fstream>
#include <iosfwd>


// config keys
extern LPCSTR ConfigKeyKeyboards;
extern LPCSTR ConfigKeyHotkeys;
extern LPCSTR ConfigKeyHotkeysOnOff;
extern LPCSTR ConfigKeyHotkeysNext;
extern LPCSTR ConfigKeyKeyboardPerWindow;

extern std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

BOOL DirectoryExists(LPCTSTR szPath);
std::wstring AppDataDirectory(BOOL roaming = true, BOOL create = true);
int SHRemoveDirectory(LPCTSTR dir);

using json = nlohmann::json;

class ConfigUtils
{
public:
	static std::wstring JsonFilePath() {
		char temp[MAX_PATH];
		size_t converted;

		std::wstring dataDirectory = AppDataDirectory();
		std::wstring jsonFile = dataDirectory + _T("config.json");

		return jsonFile;
	}

	static bool Write(json & config)
	{
		std::ofstream out(JsonFilePath());
		out << config.dump(4);
		out.close();

		return true;
	}

	static json Read()
	{
		std::wstring jsonFile = JsonFilePath();
		std::ifstream t(jsonFile);

		json config = json::object();

		if (t.good())
		{
			config = json::parse(t);
		}
		else {
			config = json::object();
			config[ConfigKeyKeyboards] = json::array();
		}

		if (config.find(ConfigKeyHotkeys) == config.end()) {
			WORD hky_onoff = MAKEWORD(0, HOTKEYF_SHIFT | HOTKEYF_CONTROL);
			WORD hky_nextkbd = MAKEWORD(VK_SPACE, HOTKEYF_CONTROL);

			config[ConfigKeyHotkeys] = json::object();
			config[ConfigKeyHotkeys][ConfigKeyHotkeysOnOff] = hky_onoff;
			config[ConfigKeyHotkeys][ConfigKeyHotkeysNext] = hky_nextkbd;
		}

		if (config.find(ConfigKeyKeyboardPerWindow) == config.end()) {
			config[ConfigKeyKeyboardPerWindow] = false;
		}

		return config;
	}
};