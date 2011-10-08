// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>

#include "keymagic.h"
#include "KeyMagicKeyboard.h"

using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

namespace KeyMagicDotNet {

	bool KeyMagicKeyboard::LoadKeyboardFile(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);

		bool ret = m_keyboard->loadKeyboardFile(szfileName);

		delete context;
		return ret;
	}

	array<String^> ^ KeyMagicKeyboard::GetStrings()
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

	LayoutOptions ^ KeyMagicKeyboard::GetLayoutOptions()
	{
		libkm::LayoutOptions * options = m_keyboard->getLayoutOptions();

		LayoutOptions ^ managedOptions = gcnew LayoutOptions();

		managedOptions->trackCaps = options->trackCaps;
		managedOptions->autoBksp = options->autoBksp;
		managedOptions->eat = options->eat;
		managedOptions->posBased = options->posBased;
		managedOptions->rightAlt = options->rightAlt;

		return managedOptions;
	}

	//array<RuleInfo^> ^ KeyMagicKeyboard::getRules()
	//{
	//}

	InfoList ^ KeyMagicKeyboard::GetInfosFromKeyboardFile(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);

		libkm::InfoList * infos = libkm::KeyMagicKeyboard::getInfosFromKeyboardFile(szfileName);

		if (infos == NULL) {
			delete context;
			return nullptr;
		}

		InfoList ^ managedInfos = gcnew InfoList(*infos);

		delete infos;

		delete context;
		return managedInfos;
	}

	InfoList ^ KeyMagicKeyboard::GetInfoList()
	{
		const libkm::InfoList infos = m_keyboard->getInfoList();
		InfoList ^ managedInfos = gcnew InfoList(infos);

		return managedInfos;
	}

	Version ^ KeyMagicKeyboard::GetVersion(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);

		DWORD dwVersion = libkm::KeyMagicKeyboard::getVersion(szfileName);

		delete context;
		return gcnew Version(LOWORD(dwVersion), HIWORD(dwVersion));
	}
}