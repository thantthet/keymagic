#include "StdAfx.h"

#include "keymagic.h"
#include "KeyMagicKeyboard.h"
#include "KeyMagicEngine.h"
#include "KeyMagicTextBox.h"

namespace KeyMagicDotNet {

	KeyMagicTextBox::KeyMagicTextBox(KeyMagicEngine^ e) : TextBox()
	{
		engine = e;
	}

	KeyMagicTextBox::KeyMagicTextBox() : TextBox()
	{
		engine = nullptr;
	}

	System::Void KeyMagicTextBox::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
	{
		if (engine == nullptr) {
			return TextBox::OnKeyDown(e);
		}

		if (!engine->ProcessKeyDown(this, e))
		{
			if (e->KeyCode != System::Windows::Forms::Keys::Control || e->KeyCode != System::Windows::Forms::Keys::Alt) {
				if (e->Control || e->Alt) {
					engine->Reset();
				}
			}
		}

		TextBox::OnKeyDown(e);
	}
}