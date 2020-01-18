#pragma once

#include <codecvt>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <fstream>
#include <iosfwd>
#include <ShellAPI.h>

#include "json.hpp"
#include "Utils.h"

// config keys
extern LPCSTR ConfigKeyKeyboards;
extern LPCSTR ConfigKeyHotkeys;
extern LPCSTR ConfigKeyHotkeysOnOff;
extern LPCSTR ConfigKeyHotkeysNext;
extern LPCSTR ConfigKeyKeyboardPerWindow;

extern std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

int SHRemoveDirectory(LPCTSTR dir);

using json = nlohmann::json;

class DataDirectoryProvider {
public:
	static DataDirectoryProvider* Shared()
	{
		static DataDirectoryProvider* shared = nullptr;
		if (shared == nullptr) {
			shared = new DataDirectoryProvider();
			shared->Prepare();
		}
		return shared;
	}

	std::basic_string<TCHAR> ConfigDirectory(std::basic_string<TCHAR> filename = _T(""))
	{
		if (filename.length() > 0)
		{
			return JoinPath(dataDir, filename);
		}
		return dataDir;
	}

	std::basic_string<TCHAR> KeyboardDirectory(std::basic_string<TCHAR> filename = _T(""))
	{
		if (filename.length() > 0)
		{
			return JoinPath(dataDir, filename);
		}
		return dataDir;
	}
private:
	std::basic_string<TCHAR> dataDir;

	std::basic_string<TCHAR> JoinPath(std::basic_string<TCHAR> base, std::basic_string<TCHAR> join) {
		base.append(_T("\\"));
		base.append(join);

		return base;
	}

	/* Prepare data directory by copying to appdata (if doesn't exists) from common appdata */
	void Prepare()
	{
		auto common = GetSystemDirectory(CSIDL_COMMON_APPDATA);
		auto user = GetSystemDirectory(CSIDL_APPDATA);

		auto keymagic = _T("KeyMagic");

		dataDir = JoinPath(user, keymagic);

		if (DirectoryExists(user, keymagic) && PathIsDirectoryEmpty(dataDir.c_str()) == FALSE)
		{
			return;
		}
		// if user doesn't exists or empty but common is availale
		else if (DirectoryExists(common, keymagic)) {
			// use raw strings for double null-terminated string
			TCHAR toDir[MAX_PATH] = { 0 };
			TCHAR fromDir[MAX_PATH] = { 0 };

			RemoveDirectory(dataDir.c_str());
			StrCpy(toDir, (dataDir + _T("\\")).c_str());
			StrCpy(fromDir, JoinPath(JoinPath(common, keymagic), _T("*")).c_str());

			SHFILEOPSTRUCT s = { 0 };
			s.hwnd = NULL;
			s.wFunc = FO_COPY;
			s.fFlags = FOF_NO_UI;
			s.pTo = toDir;
			s.pFrom = fromDir;
			SHFileOperation(&s);
		}
	}

	bool DirectoryExists(std::basic_string<TCHAR> parent, std::basic_string<TCHAR> child)
	{
		return DirectoryExists(JoinPath(parent, child));
	}

	bool DirectoryExists(std::basic_string<TCHAR> path)
	{
		DWORD dwAttrib = GetFileAttributes(path.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	std::basic_string<TCHAR> GetSystemDirectory(int type)
	{
		TCHAR szPath[MAX_PATH];
		std::basic_string<TCHAR> destDirPath;

		if (SUCCEEDED(SHGetFolderPath(NULL, type, NULL, 0, szPath))) {
			destDirPath = szPath;
		}

		return destDirPath;
	}
};

class ConfigUtils
{
public:
	static std::wstring JsonFilePath() {
		std::wstring jsonFile = DataDirectoryProvider::Shared()->ConfigDirectory(_T("config.json"));

		return jsonFile;
	}

	static bool Write(json & config)
	{
		std::ofstream out(JsonFilePath());
		if (out.is_open()) {
			out << config.dump(4);
			out.close();

			return true;
		}
		else {
			DebugLog("Failed to open config file for writing." << std::endl);
			return false;
		}
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