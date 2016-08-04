#include "stdafx.h"
#include <keymagic.h>

#include "KeyboardManager.h"
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
void Keyboard::LoadKeyboardIcon()
{
	auto infoList = libkm::KeyMagicKeyboard::getInfosFromKeyboardFile((this->basePath + this->path).c_str());
	auto pair = infoList->find('icon');
	if (pair == infoList->end())
	{
		return;
	}
	auto info =pair->second;

	IStream *pStream = SHCreateMemStream((BYTE *)info.Data(), info.Size());
	
	CImage image;
	if (SUCCEEDED(image.Load(pStream)))
	{
		this->hIcon = image.Detach();
	}

	pStream->Release();
}

BOOL KeyboardManager::SetKeyboards(nlohmann::json jsonKeyboards)
{
	this->keyboards.clear();

	int index = 0;
	for (auto& k : jsonKeyboards) {
		Keyboard keyboard;
		keyboard.index = index++;
		keyboard.basePath = this->m_basePath;

		keyboard.name = k["name"].get<std::string>();
		keyboard.path = k["path"].get<std::string>();

		this->keyboards.push_back(keyboard);
	}
	return true;
}

TKeyboardList KeyboardManager::GetKeyboards()
{
	return this->keyboards;
}

std::string const& KeyboardManager::basePath() const
{
	return this->m_basePath;
}

void KeyboardManager::basePath(std::string const& newBasePath)
{
	this->m_basePath = newBasePath;
}