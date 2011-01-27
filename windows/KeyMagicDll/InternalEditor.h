#ifndef INTERNALEDITOR_H_
#define INTERNALEDITOR_H_

#pragma once

#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>

#define MAX_STORELEN 200

#ifndef _DEBUG
#define RESTART_IE 1 // Reset internal editor? (TRUE for release build), When false, this prevent from resetting when modifier keys are pressed
#endif

class classInternalEditor {

	public:
		classInternalEditor()
		{
			CaretLocation = 0;
			TextLength = 0;
		}

		~classInternalEditor(){}

		const wchar_t* GetAll()
		{
			return Text;
		}

		void Reset()
		{
			strUndo.clear();
			LastContext[0] = 0;
#ifdef RESTART_IE
			Debug(L"InternalEditor::Restart\n");
			RtlZeroMemory(Text, MAX_STORELEN);
			CaretLocation = TextLength = 0;
			setAllSwitchOff();
#endif
		}

		// Get virtual-caret location
		// This function not used for now
		UINT GetCaretLocation()
		{
			return CaretLocation;
		}

		// Set virtual caret location
		// This function not used for now
		bool SetCaretLocation(UINT Location)
		{
			if (Location > TextLength)
				return false;
			CaretLocation = Location;
			return true;
		}

		// Get text left-most
		// return NULL when there is no enough length to return
		const wchar_t* GetTextBackward(UINT Length)
		{
			int Loc = CaretLocation - Length;

			if ( (Loc < 0) || (Loc+Length > MAX_STORELEN) )
				return NULL;

	 		return &Text[Loc];
		}

		bool AppendText(const wchar_t* TextToAppend, UINT length)
		{
			Debug(L"InternalEditor::AppendText\n");
			dump();
			if (!*TextToAppend)
				return false;

			if (TextLength + length >= MAX_STORELEN)
			{
				CaretLocation = 0;
			}
			//wcsncat(Text, TextToAppend, length);
			// As we knew text length, we could use wcsncpy insted of wcsncat
			// may be, this can make faster
			wcsncpy(&Text[CaretLocation], TextToAppend, length);
			CaretLocation += length;
			TextLength = CaretLocation;
			Text[TextLength] = 0;
			return true;
		}

		bool AddInput(UINT Char)
		{
			//Debug(L"InternalEditor::AddInput\n");
			if (Char)
				return AppendText((wchar_t*)&Char, 1);
			return false;
		}

		bool Delete(int count = 1)
		{
			Debug(L"InternalEditor::Delete\n");	

			if (TextLength < count)
			{
				Text[0] = NULL;
				return false;
			}

			TextLength = TextLength - count;
			Text[TextLength] = NULL;
			CaretLocation = TextLength;

			dump();	

			return true;
		}

		UINT GetTextLength(void)
		{
			return TextLength;
		}

		void KeyDown(WPARAM wParam, LPARAM lParam)
		{
			Debug(L"Internal Editor::KeyDown - lParam = 0x%.8x %d wParam = 0x%.8x %d\n", lParam, lParam, wParam, wParam);

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
					Reset();
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
				Reset();

		}

		void KeyUp(WPARAM wParam, LPARAM lParam);

		bool SwitchOn(int id) {
			switch_map[id] = true;
			return true;
		}

		bool SwitchOff(int id) {
			if (switch_map[id] != false){
				switch_map[id] = false;
				return false;
			}
			switch_map[id] = false;
			return true;
		}

		bool isSwitchOn(int id) {
			return switch_map[id];
		}

		bool invertSwitch(int id) {
			switch_map[id] = !switch_map[id];
			return !switch_map[id];
		}

		void setAllSwitchOff() {
			switch_map.clear();
		}
		
		void dump()
		{
			for (int i = 0; i < TextLength; ++i) {
				if (Text[i] <= 0xFF) {
					Debug(L"%c", Text[i]);
				} else {
					Debug(L"\\u%x", Text[i]);
				}
			}
			Debug(L"\n", Text);
		}

		void undoBegin();
		void undoEnd();
		void undoReset();
		bool undo();

	private:
		UINT CaretLocation;
		UINT TextLength;
		std::map<int,bool> switch_map;
		std::list<std::wstring> strUndo;
		wchar_t LastContext[MAX_STORELEN];
		wchar_t Text[MAX_STORELEN];
		bool isControlDown;
		bool isCTRL, isALT, isSHIFT;
};

#endif