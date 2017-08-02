#pragma once

#include "json.hpp"
#include <codecvt>
#include <Shlobj.h>
#include <fstream>
#include <iosfwd>

extern std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

static LPCTSTR AppDataDirectory()
{
	TCHAR szPath[MAX_PATH];
	static std::wstring destDirPath;

	if (destDirPath.size())
	{
		return destDirPath.c_str();
	}

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath))) {
		destDirPath = szPath;
		destDirPath += _T("\\KeyMagic\\");
		CreateDirectory(destDirPath.c_str(), NULL);
	}

	return destDirPath.c_str();
}

using json = nlohmann::json;

class ConfigUtils
{
public:
	static std::wstring jsonFilePath() {
		char temp[MAX_PATH];
		size_t converted;

		std::wstring dataDirectory = AppDataDirectory();
		std::wstring jsonFile = dataDirectory + _T("config.json");

		return jsonFile;
	}

	static bool Write(json & config)
	{
		std::ofstream out(jsonFilePath());
		out << config.dump(4);
		out.close();

		return true;
	}

	static json Read()
	{
		std::wstring jsonFile = jsonFilePath();
		std::ifstream t(jsonFile);

		json config = json::object();

		if (t.good())
		{
			config = json::parse(t);
		}
		else {
			config = json::object();
			config["keyboards"] = json::array();
		}

		return config;
	}
};