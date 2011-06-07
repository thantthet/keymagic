// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>

#include "keymagic.h"
#include "KeyMagicKeyboard.h"
#include "KeyMagicEngine.h"

using namespace msclr::interop;

namespace KeyMagicDotNet {

	bool KeyMagicEngine::loadKeyboardFile(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);
		
		bool ret = engine->loadKeyboardFile(szfileName);

		delete context;
		return ret;
	}

	bool KeyMagicEngine::processKeyEvent(int keyval, int keycode, int modifier)
	{
		return engine->processKeyEvent(keyval, keycode, modifier);
	}

	void KeyMagicEngine::reset()
	{
		return engine->reset();
	}

	String ^ KeyMagicEngine::getContextText()
	{
		libkm::KeyMagicString * s = engine->getContextText();
		return gcnew String(s->c_str());
	}

	void KeyMagicEngine::setContextText(String ^ textContext)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szTextContext = context->marshal_as<const wchar_t*>(textContext);

		libkm::KeyMagicString * s = new libkm::KeyMagicString(szTextContext);
		
		engine->setContextText(s);

		delete s;
		delete context;
	}

	KeyMagicKeyboard ^ KeyMagicEngine::getKeyboard()
	{
		libkm::KeyMagicKeyboard * keyboard = engine->getKeyboard();
		KeyMagicKeyboard ^ managedKeyboard = gcnew KeyMagicKeyboard(keyboard);
		return managedKeyboard;
	}
}