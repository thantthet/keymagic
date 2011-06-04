// This is the main DLL file.

#include "stdafx.h"

#include "KeyMagicEngine.h"
#include "libkeymagic.net.h"

#include <msclr/marshal.h>
using namespace msclr::interop;

namespace KeyMagicDotNet {

	bool NetKeyMagicEngine::loadKeyboardFile(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);
		
		bool ret = engine->loadKeyboardFile(szfileName);

		delete context;
		return ret;
	}

	bool NetKeyMagicEngine::processKeyEvent(int keyval, int keycode, int modifier)
	{
		return engine->processKeyEvent(keyval, keycode, modifier);
	}

	void NetKeyMagicEngine::reset()
	{
		return engine->reset();
	}

	String ^ NetKeyMagicEngine::getContextText()
	{
		KeyMagicString * s = engine->getContextText();
		return gcnew String(s->c_str());
	}

	void NetKeyMagicEngine::setContextText(String ^ textContext)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szTextContext = context->marshal_as<const wchar_t*>(textContext);

		KeyMagicString * s = new KeyMagicString(szTextContext);
		
		engine->setContextText(s);

		delete s;
		delete context;
	}
}