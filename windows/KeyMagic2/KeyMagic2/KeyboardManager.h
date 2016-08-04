#pragma once

#include <list>
#include "json.hpp"

class Keyboard
{
private:
	HBITMAP hIcon = NULL;
public:
	std::string basePath;

	int index; // index for convience

	std::string name;
	std::string path;
	HBITMAP GetKeyboardIcon();
private:
	void LoadKeyboardIcon();
};

typedef std::list<Keyboard> TKeyboardList;

class KeyboardManager
{
private:
	TKeyboardList keyboards;
	std::string m_basePath;

public:
	
	std::string const& basePath() const;
	void basePath(std::string const& newBasePath);

	BOOL SetKeyboards(nlohmann::json);
	TKeyboardList GetKeyboards();
};