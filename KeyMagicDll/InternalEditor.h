#ifndef INTERNALEDITOR_H_
#define INTERNALEDITOR_H_

#pragma once

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>

#define MAX_STORELEN 200

class classInternalEditor {

	public:
		classInternalEditor()
		{
			CaretLocation = 0;
			TextLength = 0;
		}

		~classInternalEditor(){}

		void Restart()
		{
			RtlZeroMemory(Text, MAX_STORELEN);
			CaretLocation = TextLength = 0;
		}

		UINT GetCaretLocation()
		{
			return CaretLocation;
		}

		bool SetCaretLocation(UINT Location)
		{
			if (Location > TextLength)
				return false;
			CaretLocation = Location;
			return true;
		}

		wchar_t* GetTextBackward(UINT Length)
		{
			int Loc = CaretLocation - Length;

			if ( (Loc < 0) || (Loc+Length > MAX_STORELEN) )
				return NULL;

	 		return &Text[Loc];
		}

		bool AppendText(const wchar_t* TextToAppend, UINT length)
		{
			if (TextLength >= MAX_STORELEN)
			{
				TextLength = 0;
				RtlZeroMemory(Text, MAX_STORELEN);
			}
			wcsncat(Text, TextToAppend, length);
			CaretLocation = TextLength = wcslen(Text);
			return true;
		}

		bool Delete(){
			if (TextLength <= 0)
				return false;

			Text[--TextLength] = NULL;
			CaretLocation = TextLength;
			return true;
		}

		UINT GetTextLength(void)
		{
			return TextLength;
		}

		void KeyDown(int vKey)
		{
			bool isCTRL, isALT;

			if (vKey == VK_CONTROL || vKey == VK_MENU)
				return;

			BYTE KeyStatus[256];
			GetKeyboardState(KeyStatus);

			isCTRL = KeyStatus[VK_CONTROL] & 0x80;
			isALT = KeyStatus[VK_MENU] & 0x80;

			if (isCTRL || isALT)
				Restart();
		}
		
		void dump()
		{
			OutputDebugString("\"");
			OutputDebugStringW(Text);
			OutputDebugString("\"\n");
		}

	private:
		UINT CaretLocation;
		UINT TextLength;
		wchar_t Text[MAX_STORELEN];
		bool isControlDown;
};

#endif