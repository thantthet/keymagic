#pragma once

using namespace System;

namespace KeyMagicDotNet {

	public ref class Util
	{
	public:
		static bool GetKeyCodeAndModifier(int keyval, int % keycode, int % modifier);
	};
}
