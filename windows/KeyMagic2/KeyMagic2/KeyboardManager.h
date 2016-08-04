#pragma once

#include <list>
#include <keymagic.h>
#include "json.hpp"

class Keyboard
{
private:
	HBITMAP hIcon = NULL;
	libkm::KeyMagicEngine *m_engine = nullptr;

public:
	std::string basePath;

	int index; // index for convience

	std::string name;
	std::string path;
	HBITMAP GetKeyboardIcon();

private:
	std::string KeyboardFullPath();
	void LoadKeyboardIcon();

public:
	libkm::KeyMagicEngine * GetKeyMagicEngine();
};

typedef std::list<Keyboard> TKeyboardList;

class KeyboardManager
{
private:
	TKeyboardList m_keyboards;
	Keyboard* m_selectedKeyboard;
	Keyboard* m_lastSelectedKeyboard;
	std::string m_basePath;

public:
	static KeyboardManager * sharedManager();

	std::string const& basePath() const;
	void basePath(std::string const& newBasePath);
	Keyboard * SelectedKeyboard();
	Keyboard * KeyboardAtIndex(int index);
	BOOL SetKeyboards(nlohmann::json);
	TKeyboardList& GetKeyboards();
	BOOL SelectKeyboard(Keyboard * keyboard);
	BOOL ToggleKeyboard();
};