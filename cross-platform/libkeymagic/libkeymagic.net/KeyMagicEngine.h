#pragma once

#include "KeyMagicKeyboard.h"
#include "LayoutOptions.h"

using namespace System;

namespace KeyMagicDotNet {

	public ref class KeyMagicEngine
	{
	public:
		KeyMagicEngine() : m_engine ( new libkm::KeyMagicEngine() ) {}
		KeyMagicEngine(KeyMagicEngine ^ engine) : m_engine ( new libkm::KeyMagicEngine(*engine->m_engine) ) {}
		~KeyMagicEngine() 
		{
			delete m_engine;
		}
	protected:
		!KeyMagicEngine() {
			delete m_engine;
		}
	public:
		enum class ModifierMask {
			SHIFT_MASK = 1 << 0,
			CTRL_MASK = 1 << 1,
			ALT_MASK = 1 << 2,
			CAPS_MASK = 1 << 3
		};

		bool LoadKeyboardFile(String ^ filename);
		bool KeyMagicEngine::ProcessKeyDown(System::Windows::Forms::TextBoxBase^ txtInput, System::Windows::Forms::KeyEventArgs^ e);
		bool ProcessKeyEvent(int keyval, int keycode, int modifier);
		void Reset();
		String ^ GetContextText();
		void SetContextText(String ^ textContext);
		KeyMagicKeyboard ^ GetKeyboard();
		int GetDifference(String ^ contextBefore, String ^% difference);

		short GetKeyState(int keycode);
		void SetKeyState(int keycode, byte state);
		void SetKeyStates(array<byte> ^ keystates);

		Dictionary<int, bool> ^ GetSwitchMap();
		void SetSwitchMap(Dictionary<int, bool> ^);

		property bool Verbose {
			bool get() {
				return m_engine->m_verbose;
			}
			void set(bool value) {
				m_engine->m_verbose = value;
			}
		}

		property LayoutOptions ^ Options {
			LayoutOptions ^ get() {
				if (m_layoutOptions == nullptr) {
					m_layoutOptions = GetKeyboard()->GetLayoutOptions();
				}
				return m_layoutOptions;
			}
		}
	private:
		libkm::KeyMagicEngine * m_engine;
		LayoutOptions ^ m_layoutOptions;
	};
}
