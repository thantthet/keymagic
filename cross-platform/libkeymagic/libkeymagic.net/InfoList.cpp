#include "stdafx.h"
#include <msclr/marshal.h>

#include "keymagic.h"
#include "InfoList.h"

using namespace msclr::interop;
using namespace System::Runtime::InteropServices;

namespace KeyMagicDotNet {
	InfoList::InfoList(const libkm::InfoList& infoList)
	{
		this->infoMap = new libkm::InfoList(infoList);
	}

	array<Byte>^ InfoList::FindById(String ^ strId)
	{
		if (strId->Length != 4)
		{
			return nullptr;
		}
		
		int intId = strId[3] | strId[2] << 8 | strId[1] << 16 | strId[0] << 24;

		if (this->infoMap->find(intId) == this->infoMap->end())
		{
			return nullptr;
		}

		libkm::Info i = this->infoMap->find(intId)->second;

		array<Byte>^ data = gcnew array<Byte>(i.Size());

		Marshal::Copy((IntPtr)(char*)i.Data(), data, 0, (int)i.Size());

		return data;
	}

	array<Byte>^ InfoList::operator[](String ^ strId)
	{
		return this->FindById(strId);
	}

	System::Drawing::Bitmap ^ InfoList::GetIcon()
    {
        array<Byte>^ icon = this->FindById("icon");
        if (icon != nullptr)
        {
			System::IO::MemoryStream ^ ms = gcnew System::IO::MemoryStream(icon);
            System::Drawing::Bitmap ^ bm = gcnew System::Drawing::Bitmap(ms);
            return bm;
        }
        return nullptr;
    }

    String ^ InfoList::GetName()
    {
        array<Byte>^ name = this->FindById("name");
        if (name != nullptr)
        {
			return System::Text::Encoding::UTF8->GetString(name);
        }
        return nullptr;
    }

    String ^ InfoList::GetHotkey()
    {
        array<Byte>^ hotkey = FindById("htky");
        if (hotkey != nullptr)
        {
            return System::Text::Encoding::UTF8->GetString(hotkey);
        }
        return nullptr;
    }

    String ^ InfoList::GetDescription()
    {
        array<Byte>^ desc = this->FindById("desc");
        if (desc != nullptr)
        {
            return System::Text::Encoding::UTF8->GetString(desc);
        }
        return nullptr;
    }

    String ^ InfoList::GetFontFamily()
    {
        array<Byte>^ font = this->FindById("font");
        if (font != nullptr)
        {
            return System::Text::Encoding::UTF8->GetString(font);
        }
        return nullptr;
    }
}