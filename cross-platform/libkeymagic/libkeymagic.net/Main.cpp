#include "stdafx.h"

#include "keymagic.h"
#include "KeyMagicEngine.h"

using namespace System;
using namespace KeyMagicDotNet;

int main(array<String ^> ^args)
{
	String ^ path = "./../../../LayoutScripts/Zawgyi L - Unicode.km2";

	KeyMagicEngine ^ engine = gcnew KeyMagicEngine();
	engine->LoadKeyboardFile(path);

	engine->ProcessKeyEvent(0x60, 0xC0, 0);

	KeyMagicEngine ^ e2 = gcnew KeyMagicEngine(engine);

	Dictionary<int, bool> ^ d1 = engine->GetSwitchMap();
	Dictionary<int, bool> ^ d2 = e2->GetSwitchMap();

	System::Console::WriteLine(d1);
	System::Console::WriteLine(d2);

    return 0;
}
