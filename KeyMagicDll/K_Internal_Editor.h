#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <Commdlg.h>


void Logger(HWND hWnd, char* fmt, ...)
{
#ifdef DEBUG
	char Memory[100];
	RECT rc;

	va_list list;

	va_start(list, fmt);
	//Format
	wvsprintf(Memory, fmt,list);

	SetWindowText(hWnd, Memory);
	
	//Cleanup
	va_end(list);
#else
	return;
#endif
}

class K_Internal_Editor {
	public:
		K_Internal_Editor()
		{
			CaretLocation = 0;
			TextLength = 0;
		}
#ifdef DEBUG
		void SetHandle(HWND hWnd){
			Commander_hWnd = hWnd;
		}
#endif
		~K_Internal_Editor(){}

		virtual void Restart()
		{
			RtlZeroMemory(Text, 50);
			CaretLocation = TextLength = 0;
		}

		virtual UINT GetCaretLocation()
		{
			return CaretLocation;
		}

		virtual bool SetCaretLocation(UINT Location)
		{
			if (Location > TextLength)
				return false;
			CaretLocation = Location;
			return true;
		}

		virtual wchar_t* GetText(UINT Length)
		{
			int Loc = CaretLocation-Length;

			if ( (Loc < 0) || (Loc+Length > 50) )
				return NULL;

	 		return &Text[Loc];
		}

		virtual bool AppendText(wchar_t* TextToAppend, UINT length)
		{
			if (TextLength >= 50)
			{
				TextLength = 0;
				RtlZeroMemory(Text, 50);
			}
			wcsncat(Text, TextToAppend, length);
			CaretLocation = TextLength = wcslen(Text);
#ifdef DEBUG
			Logger(Commander_hWnd, "%x %x %x", Text[TextLength], Text[TextLength-1], Text[TextLength-2]);
#endif
			return true;
		}

		virtual bool Delete(){
			if (TextLength <= 0)
				return false;

			Text[--TextLength] = NULL;
			CaretLocation = TextLength;
#ifdef DEBUG
			Logger(Commander_hWnd, "%x %x %x", Text[TextLength], Text[TextLength-1], Text[TextLength-2]);
#endif
			return true;
		}

		virtual UINT GetTextLength(void)
		{
			return TextLength;
		}

		virtual void CtrlKeyDown()
		{
			isControlDown = true;
		}

		virtual void CtrlKeyUp()
		{
			isControlDown = false;
		}

		virtual void KeyDown()
		{
			if (isControlDown)
				Restart();
		}

	private:
#ifdef DEBUG
		HWND Commander_hWnd;
#endif

		UINT CaretLocation;
		UINT TextLength;
		wchar_t Text[50];
		bool isControlDown;
};
