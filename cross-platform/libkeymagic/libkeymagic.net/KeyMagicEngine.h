#pragma once

using namespace System;

namespace KeyMagicDotNet {

	public ref class KeyMagicEngine
	{
	public:
		KeyMagicEngine() : engine ( new libkm::KeyMagicEngine() ) {}
		~KeyMagicEngine() 
		{
			delete engine;
		}
	protected:
		!KeyMagicEngine() {
			delete engine;
		}
	public:
		enum class ModifierMask {
			SHIFT_MASK = 1 << 0,
			CTRL_MASK = 1 << 1,
			ALT_MASK = 1 << 2,
			CAPS_MASK = 1 << 3
		};

		bool loadKeyboardFile(String ^ filename);
		bool processKeyEvent(int keyval, int keycode, int modifier);
		void reset();
		String ^ getContextText();
		void setContextText(String ^ textContext);
		KeyMagicKeyboard ^ getKeyboard();

		property bool Verbose {
			bool get() {
				return engine->m_verbose;
			}
			void set(bool value) {
				engine->m_verbose = value;
			}
		}
	private:
		libkm::KeyMagicEngine * engine;
	};
}
