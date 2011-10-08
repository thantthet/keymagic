#pragma once

using namespace System;

namespace KeyMagicDotNet {
	public ref class InfoList
	{
	public:
		InfoList(const libkm::InfoList& infoList);
		array<Byte>^ FindById(String ^ strId);
		array<Byte>^ operator[](String ^ strId);
		System::Drawing::Bitmap ^ GetIcon();
		String ^ GetName();
		String ^ GetHotkey();
		String ^ GetDescription();
		String ^ GetFontFamily();

	private:
		const libkm::InfoList * infoMap;
	};
}