#pragma once

#include "InfoList.h"
#include "RuleInfo.h"
#include "LayoutOptions.h"

using namespace System;
using namespace System::Collections::Generic;

namespace KeyMagicDotNet {

	//typedef Dictionary<String ^, array<Byte^> ^>  InfoList;

	public ref class KeyMagicKeyboard
	{
	public:
		KeyMagicKeyboard() : m_keyboard ( new libkm::KeyMagicKeyboard() ) {}
		KeyMagicKeyboard(libkm::KeyMagicKeyboard * keyboard) : m_keyboard(new libkm::KeyMagicKeyboard(*keyboard)) {}
		KeyMagicKeyboard(KeyMagicKeyboard ^ keyboard) : m_keyboard(new libkm::KeyMagicKeyboard(*keyboard->m_keyboard)) {}
		~KeyMagicKeyboard() 
		{
			delete m_keyboard;
		}
	protected:
		!KeyMagicKeyboard() {

		}
	public:
		bool LoadKeyboardFile(String ^ fileName);
		static InfoList ^ GetInfosFromKeyboardFile(String ^ fileName);
		array<String^> ^ GetStrings();
		LayoutOptions ^ GetLayoutOptions();
		//array<RuleInfo^> ^ getRules();
		InfoList ^ GetInfoList();
		static Version ^ GetVersion(String ^ fileName);

	private:
		libkm::KeyMagicKeyboard * m_keyboard;
	};
}
