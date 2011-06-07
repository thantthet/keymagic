
#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace KeyMagicDotNet {

	typedef Dictionary<String ^, array<Byte^> ^>  InfoList;

	public ref class KeyMagicKeyboard
	{
	public:
		KeyMagicKeyboard() : m_keyboard ( new libkm::KeyMagicKeyboard() ) {}
		KeyMagicKeyboard(libkm::KeyMagicKeyboard * keyboard) : m_keyboard(keyboard) {}
		bool loadKeyboardFile(String ^ fileName);
		array<String^> ^ getStrings();
		//array<RuleInfo^> ^ getRules();
		array<Byte>^ getInfo(String ^ id);

	private:
		libkm::KeyMagicKeyboard * m_keyboard;
	};
}
