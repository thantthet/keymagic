#include "stdafx.h"
#include <keymagic.h>

#include "KeyboardManager.h"
#include "ConfigUtils.h"
#include <atlimage.h>
#include <gdiplus.h>

struct membuf :
	std::streambuf
{
	membuf(char* b, char* e) {
		this->setg(b, b, e);
	}
};

HBITMAP Keyboard::GetKeyboardIcon()
{
	if (!this->hIcon)
	{
		this->LoadKeyboardIcon();
	}
	return this->hIcon;
}

std::wstring Keyboard::KeyboardFullPath()
{
	return (this->basePath + this->path);
}

void Keyboard::LoadKeyboardIcon()
{
	auto infoList = libkm::KeyMagicKeyboard::getInfosFromKeyboardFile(this->KeyboardFullPath().c_str());
	auto pair = infoList->find('icon');
	if (pair == infoList->end())
	{
		return;
	}
	auto info = pair->second;

	IStream *pStream = SHCreateMemStream((BYTE *)info.Data(), info.Size());
	
	CImage image;
	if (SUCCEEDED(image.Load(pStream)))
	{
		this->hIcon = image.Detach();
	}

	pStream->Release();
}

libkm::KeyMagicEngine * Keyboard::GetKeyMagicEngine()
{
	if (m_engine != nullptr) {
		return m_engine;
	}

	m_engine = new libkm::KeyMagicEngine();
#ifdef DEBUG
	m_engine->m_verbose = true;
#endif
	m_engine->loadKeyboardFile(this->KeyboardFullPath().c_str());

	return m_engine;
}

BOOL KeyboardManager::SetKeyboards(nlohmann::json jsonKeyboards)
{
	this->SelectKeyboard(nullptr);
	this->m_lastSelectedKeyboard = nullptr;
	this->m_keyboards.clear();

	int index = 0;
	for (auto& k : jsonKeyboards) {
		Keyboard keyboard;
		keyboard.index = index++;
		keyboard.basePath = this->m_basePath;

		std::string name = k["name"].get<std::string>();
		std::string path = k["path"].get<std::string>();

		keyboard.name = converter.from_bytes(name);
		keyboard.path = converter.from_bytes(path);

		this->m_keyboards.push_back(keyboard);
	}
	return true;
}

TKeyboardList& KeyboardManager::GetKeyboards()
{
	return this->m_keyboards;
}

BOOL KeyboardManager::SelectKeyboard(Keyboard * keyboard)
{
	if (keyboard != nullptr)
	{
		for (auto &kb : this->m_keyboards)
		{
			if (&kb == keyboard) {
				this->m_lastSelectedKeyboard = this->m_selectedKeyboard;
				this->m_selectedKeyboard = keyboard;
				this->m_callback();
				return true;
			}
		}
		return false;
	}
	else {
		this->m_lastSelectedKeyboard = this->m_selectedKeyboard;
		this->m_selectedKeyboard = nullptr;
		this->m_callback();
	}
	return true;
}

BOOL KeyboardManager::ToggleKeyboard()
{
	if (this->m_selectedKeyboard != nullptr) {
		this->SelectKeyboard(nullptr);
	}
	else if (this->m_lastSelectedKeyboard) {
		this->SelectKeyboard(this->m_lastSelectedKeyboard);
	}
	else if (this->m_keyboards.size()) {
		this->SelectKeyboard(&this->m_keyboards.front());
	}
	else {
		return false;
	}

	return true;
}

BOOL KeyboardManager::AdvanceToNextKeyboard()
{
	if (this->m_keyboards.size()) {
		if (this->m_selectedKeyboard == nullptr) {
			this->SelectKeyboard(&this->m_keyboards.front());
		}
		else
		{
			int nextIndex = this->m_selectedKeyboard->index + 1;
			if (nextIndex == this->m_keyboards.size())
			{
				this->SelectKeyboard(nullptr);
			}
			else {
				Keyboard &keyboard = this->m_keyboards.at(nextIndex);
				this->SelectKeyboard(&keyboard);
			}
		}
	}
	else {
		return false;
	}
	return true;
}

KeyboardManager * KeyboardManager::sharedManager()
{
	static KeyboardManager * sharedManager = nullptr;
	if (sharedManager == nullptr) {
		sharedManager = new KeyboardManager();
	}
	return sharedManager;
}

std::wstring const& KeyboardManager::basePath() const
{
	return this->m_basePath;
}

void KeyboardManager::basePath(std::wstring const& newBasePath)
{
	this->m_basePath = newBasePath;
}

Keyboard * KeyboardManager::SelectedKeyboard()
{
	return this->m_selectedKeyboard;
}

Keyboard * KeyboardManager::KeyboardAtIndex(int index)
{
	for (auto &keyboard : this->m_keyboards) {
		if (keyboard.index == index)
		{
			return &keyboard;
		}
	}
	return nullptr;
}
