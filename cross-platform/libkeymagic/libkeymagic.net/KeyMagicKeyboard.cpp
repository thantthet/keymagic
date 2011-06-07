// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>

#include "keymagic.h"
#include "RuleInfo.h"
#include "KeyMagicKeyboard.h"

using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

namespace KeyMagicDotNet {

	bool KeyMagicKeyboard::loadKeyboardFile(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);

		bool ret = m_keyboard->loadKeyboardFile(szfileName);

		delete context;
		return ret;
	}

	array<String^> ^ KeyMagicKeyboard::getStrings()
	{
		libkm::StringList * strList = m_keyboard->getStrings();

		//array<String^>^ strings = gcnew array<String^>(strList->size());
		System::Collections::Generic::List<String^>^ strings = gcnew System::Collections::Generic::List<String^>();
		for (libkm::StringList::iterator i = strList->begin(); i != strList->end(); i++)
		{
			libkm::KeyMagicString str = *i;
			strings->Add(gcnew String(str.c_str()));
		}

		delete strList;

		return strings->ToArray();
	}

	//array<RuleInfo^> ^ KeyMagicKeyboard::getRules()
	//{
	//}

	array<Byte>^ KeyMagicKeyboard::getInfo(String ^ id)
	{
		if (id->Length != 4) return nullptr;

		int intId = id[0] << 24 | id[1] << 16 | id[2] << 8 | id[3];
		const libkm::InfoList list = m_keyboard->getInfoList();

		if (list.find(intId) == list.end())
		{
			return nullptr;
		}

		libkm::Info i = list.find(intId)->second;

		array<Byte>^ data = gcnew array<Byte>(i.size);

		Marshal::Copy((IntPtr)i.data, data, 0, (int)i.size);

		return data;
	}
}