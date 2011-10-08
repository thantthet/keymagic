#pragma once
using namespace System;

namespace KeyMagicDotNet {

	/// <summary>
	/// Summary for KeyMagicTextBox
	/// </summary>
	public ref class KeyMagicTextBox : public System::Windows::Forms::TextBox
	{
	private:
		KeyMagicEngine^ engine;
	public:
		KeyMagicTextBox();
		KeyMagicTextBox(KeyMagicEngine^ engine);

		property KeyMagicEngine^ Engine {
			KeyMagicEngine^ get() {
				return engine;
			}
			void set(KeyMagicEngine^ value) {
				engine = value;
			}
		}
	protected:
		virtual System::Void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;
	};
}
