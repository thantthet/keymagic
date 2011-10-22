#include "common.h"
#include "InternalEditor.h"

void classInternalEditor::KeyUp(WPARAM wParam, LPARAM lParam)
{
	Debug(L"Internal Editor::KeyUp - lParam = 0x%.8x %d wParam = 0x%.8x %d\n", lParam, lParam, wParam, wParam);
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

void classInternalEditor::undoBegin()
{
	Debug(L"UndoBegin:LC=%s | T=%s\n", LastContext, Text);
	wcscpy(LastContext, Text);
}

void classInternalEditor::undoEnd()
{
	Debug(L"undoEnd:LC=%s | T=%s\n", LastContext, Text);
	if (wcscmp(LastContext, Text) != 0)
	{
		strUndo.push_back(std::wstring(LastContext));
	}
	LastContext[0] = 0;
}

void classInternalEditor::undoReset()
{
	strUndo.clear();
}

bool classInternalEditor::undo()
{
	int s = strUndo.size();
	if (s == 0) {
		return false;
	}

	wcscpy(Text, strUndo.back().c_str());
	TextLength = wcslen(Text);
	CaretLocation = wcslen(Text);
	strUndo.pop_back();
	return true;
}