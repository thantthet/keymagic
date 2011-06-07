#include "stdafx.h"

#include "keymagic.h"
#include "Util.h"

namespace KeyMagicDotNet {

	bool Util::GetKeyCodeAndModifier(int keyval, int % keycode, int % modifier)
	{
		int nativeKeycode = 0, nativeModifier = 0;
		
		bool ret = libkm::GetKeyCodeAndModifier(
			(int)keyval,
			&nativeKeycode,
			&nativeModifier);

		keycode = nativeKeycode;
		modifier = nativeModifier;

		return ret;
	}

}