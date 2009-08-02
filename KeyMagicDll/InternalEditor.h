#ifndef INTERNALEDITOR_H_
#define INTERNALEDITOR_H_

#pragma once

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>

#define MAX_STORELEN 200
#define RESTART_IE 1 // Restart internal editor? (TRUE for release build)

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
#ifdef RESTART_IE
#ifdef _DEBUG
			Debug(L"InternalEditor::Restart\n");
#endif
			RtlZeroMemory(Text, MAX_STORELEN);
			CaretLocation = TextLength = 0;
#endif
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

#ifdef _DEBUG
			Debug(L"InternalEditor::AppendText\n");
			dump();
#endif

			if (TextLength >= MAX_STORELEN)
			{
				TextLength = 0;
				RtlZeroMemory(Text, MAX_STORELEN);
			}
			wcsncat(Text, TextToAppend, length);
			CaretLocation = TextLength = wcslen(Text);
			return true;
		}

		bool AddInput(UINT Char)
		{

#ifdef _DEBUG
			Debug(L"InternalEditor::AddInput\n");
#endif

//			if ((isCTRL == true && isALT == false) || (isCTRL == false && isALT == true))
//			{
//#ifdef _DEBUG
//			Debug(L"(isCTRL == true && isALT == false) || (isCTRL == false && isALT == true)\n");
//#endif
//				return false;
//			}
			return AppendText((wchar_t*)&Char, 1);
		}

		bool Delete(int count = 1)
		{

#ifdef _DEBUG
			Debug(L"InternalEditor::Delete\n");	
#endif

			if (TextLength < count)
			{
				Text[0] = NULL;
				return false;
			}

			TextLength = TextLength - count;
			Text[TextLength] = NULL;
			CaretLocation = TextLength;

#ifdef _DEBUG
			dump();	
#endif

			return true;
		}

		UINT GetTextLength(void)
		{
			return TextLength;
		}

		void KeyDown(WPARAM wParam, LPARAM lParam)
		{
#ifdef _DEBUG
			Debug(L"Internal Editor::KeyDown\nlParam = 0x%.8x %d wParam = 0x%.8x %d\n", lParam, lParam, wParam, wParam);
#endif
			switch (wParam)
			{
				case VK_LEFT:
				case VK_RIGHT:
				case VK_DOWN:
				case VK_UP:
				case VK_END:
				case VK_HOME:
				case VK_PRIOR:
				case VK_NEXT:
					Restart();
					break;
				case VK_CONTROL:
					isCTRL = true;
					return;
				case VK_MENU:
					isALT = true;
					return;
			}
/*
			BYTE KeyStatus[256];
			GetKeyboardState(KeyStatus);

			isCTRL = KeyStatus[VK_CONTROL] & 0x80;
			isALT = KeyStatus[VK_MENU] & 0x80;
*/
			if (isCTRL || isALT)
				Restart();

		}

		void KeyUp(WPARAM wParam, LPARAM lParam)
		{
#ifdef _DEBUG
			Debug(L"Internal Editor::KeyUp\nlParam = 0x%.8x %d wParam = 0x%.8x %d\n", lParam, lParam, wParam, wParam);
#endif
			if (wParam == VK_CONTROL)
			{
				isCTRL = false;
				return;
			}
			else if((wParam == VK_MENU))
			{
				isALT = false;
				return;
			}
		}
		
		void dump()
		{
#ifdef _DEBUG
			Debug(L"\"%s\"\n", Text);
#endif
		}

	private:
		UINT CaretLocation;
		UINT TextLength;
		wchar_t Text[MAX_STORELEN];
		bool isControlDown;
		bool isCTRL, isALT, isSHIFT;
};

#endif