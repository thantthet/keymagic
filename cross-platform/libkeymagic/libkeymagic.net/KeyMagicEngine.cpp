// This is the main DLL file.

#include "stdafx.h"
#include <msclr/marshal.h>

#include "keymagic.h"
#include "KeyMagicEngine.h"

using namespace msclr::interop;

namespace KeyMagicDotNet {

	bool KeyMagicEngine::LoadKeyboardFile(String ^ fileName)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szfileName = context->marshal_as<const wchar_t*>(fileName);
		
		bool ret = m_engine->loadKeyboardFile(szfileName);

		delete context;
		return ret;
	}

	bool KeyMagicEngine::ProcessKeyDown(System::Windows::Forms::TextBoxBase^ txtInput, System::Windows::Forms::KeyEventArgs^ e)
	{
        int keyval = 0, modifier = 0;
        UINT keycode = (UINT)e->KeyCode;

        HKL HKLCurrent = GetKeyboardLayout(GetCurrentThreadId());

        UINT scanCode = MapVirtualKeyEx(keycode, MAPVK_VK_TO_VSC, HKLCurrent);
        if (scanCode == 0)
        {
            return false;
        }

		byte KeyState[256] = {0};

		//Set Key States
		m_engine->setKeyStates(KeyState);

        GetKeyboardState(KeyState);

		int modifierList[] = {VK_CONTROL, VK_MENU, VK_SHIFT, VK_LCONTROL, VK_RCONTROL, VK_LMENU, VK_RMENU, VK_LSHIFT, VK_RSHIFT, NULL};

		for (int i = 0; modifierList[i] != NULL; i++) {
			int key = modifierList[i];
			m_engine->setKeyState(key, KeyState[key]);
		}

        //Retrive Modifier Keys
        if ((KeyState[VK_CONTROL] & 0x80) != 0)
        {
			modifier |= (int)ModifierMask::CTRL_MASK;
        }
        if ((KeyState[VK_MENU] & 0x80) != 0)
        {
			modifier |= (int)ModifierMask::ALT_MASK;
        }
        if ((KeyState[VK_SHIFT] & 0x80) != 0)
        {
            modifier |= (int)ModifierMask::SHIFT_MASK;
        }

        //Retrive KeyValue
		WCHAR TranslatedChar[2] = {0};

        UINT vkey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, (HKL)0x04090409);
		int ret = ToUnicodeEx(vkey, scanCode, KeyState, TranslatedChar, 1, 0, (HKL)0x04090409);
		libkm::KeyMagicString * contextBefore = m_engine->getContextText();

        keyval = ret > 0 ? TranslatedChar[0] : 0;

		int sel = txtInput->SelectionStart;
        //Editor Fixing
        if (txtInput->SelectionLength != 0)
        {
            txtInput->Text = txtInput->Text->Remove(sel, txtInput->SelectionLength);
		}

		System::String^ beforeContext = txtInput->Text->Substring(0, sel);
		System::String^ afterContext = txtInput->Text->Substring(sel);
        this->SetContextText(beforeContext);

        //Process
        if (m_engine->processKeyEvent(keyval, (int)keycode, modifier))
        {
            System::String^ newBeforeContext = this->GetContextText();

            if (beforeContext != newBeforeContext)
            {
                txtInput->Text = newBeforeContext + afterContext;
            }
			txtInput->SelectionStart = newBeforeContext->Length;
			txtInput->ScrollToCaret();

            e->Handled = true;
            e->SuppressKeyPress = true;

			return true;
        }
		return false;
	}

	bool KeyMagicEngine::ProcessKeyEvent(int keyval, int keycode, int modifier)
	{
		return m_engine->processKeyEvent(keyval, keycode, modifier);
	}

	void KeyMagicEngine::Reset()
	{
		return m_engine->reset();
	}

	String ^ KeyMagicEngine::GetContextText()
	{
		libkm::KeyMagicString * s = m_engine->getContextText();
		return gcnew String(s->c_str());
	}

	void KeyMagicEngine::SetContextText(String ^ textContext)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szTextContext = context->marshal_as<const wchar_t*>(textContext);

		libkm::KeyMagicString * s = new libkm::KeyMagicString(szTextContext);
		
		m_engine->setContextText(s);

		delete s;
		delete context;
	}

	KeyMagicKeyboard ^ KeyMagicEngine::GetKeyboard()
	{
		libkm::KeyMagicKeyboard * keyboard = m_engine->getKeyboard();
		KeyMagicKeyboard ^ managedKeyboard = gcnew KeyMagicKeyboard(keyboard);
		return managedKeyboard;
	}

	int KeyMagicEngine::GetDifference(String ^ contextBefore, String ^% difference)
	{
		marshal_context ^ context = gcnew marshal_context();
		const wchar_t* szContextBefore = context->marshal_as<const wchar_t*>(contextBefore);

		libkm::KeyMagicString diff;

		int c = m_engine->getDifference(szContextBefore, &diff);

		difference = gcnew String(diff.c_str());

		delete context;
		return c;
	}

	short KeyMagicEngine::GetKeyState(int keycode)
	{
		return m_engine->getKeyState(keycode);
	}

	void KeyMagicEngine::SetKeyState(int keycode, byte state)
	{
		m_engine->setKeyState(keycode, state);
	}

	void KeyMagicEngine::SetKeyStates(array<byte> ^ keystates)
	{
		pin_ptr<byte> ks = &keystates[0];
		m_engine->setKeyStates(ks);
	}

	Dictionary<int, bool> ^ KeyMagicEngine::GetSwitchMap()
	{
		std::map<int, bool> _switch = m_engine->getSwitchMap();

		Dictionary<int, bool> ^ m_switch = gcnew Dictionary<int, bool>();

		for (std::map<int, bool>::const_iterator i = _switch.begin(); i != _switch.end(); i++)
		{
			m_switch[i->first] = i->second;
		}

		return m_switch;
	}

	void KeyMagicEngine::SetSwitchMap(Dictionary<int, bool> ^ switchMap)
	{
		std::map<int, bool> n_switchMap;

		for each (KeyValuePair<int, bool> ^ e in switchMap) {
			n_switchMap[e->Key] = e->Value;
		}

		m_engine->setSwitchMap(n_switchMap);
	}
}