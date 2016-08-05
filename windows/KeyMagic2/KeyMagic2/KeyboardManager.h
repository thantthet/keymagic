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

typedef std::vector<Keyboard> TKeyboardList;

class KeyboardManager
{
private:
	TKeyboardList m_keyboards;
	Keyboard* m_selectedKeyboard;
	Keyboard* m_lastSelectedKeyboard;
	std::string m_basePath;
	std::function<void()> m_callback;

public:
	static KeyboardManager * sharedManager();

	void addOnKeyboardDidChangeHandler(std::function<void()> callback)
	{
		m_callback = callback;
	}

	std::string const& basePath() const;
	void basePath(std::string const& newBasePath);
	Keyboard * SelectedKeyboard();
	Keyboard * KeyboardAtIndex(int index);
	BOOL SetKeyboards(nlohmann::json);
	TKeyboardList& GetKeyboards();
	BOOL SelectKeyboard(Keyboard * keyboard);
	BOOL ToggleKeyboard();
	BOOL AdvanceToNextKeyboard();
};