// libkeymagic.net.h

#pragma once

using namespace System;

namespace KeyMagicDotNet {

	public ref class NetKeyMagicEngine
	{
	public:
		NetKeyMagicEngine() : engine ( new KeyMagicEngine() ) {}
		~NetKeyMagicEngine() 
		{
			delete engine;
		}
	protected:
		!NetKeyMagicEngine() {
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
	private:
		KeyMagicEngine * engine;
	};
}
