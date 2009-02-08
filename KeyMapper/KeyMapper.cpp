//Keymap File generator for Keymagic.
//Copyright (C) 2008  KeyMagic Project
//http://keymagic.googlecode.com
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include "KeyMapper.h"
#include "resource.h"
#include "../KeyMagicDll/KeyMagicDll.h"
#include "KParser.h"
#include "RegularExpressionClass.h"

// Global Variables:
HINSTANCE hInst; // current instance
HWND hwndMain;
HWND hEdit;
LOGFONTW lf;
char szOpenedFileName[260];
char* lpCmd;
// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool OpenDialog(HWND hwnd, char* szFileName, DWORD nMaxFile);
bool SaveDialog(HWND hwnd, char* szFileName, DWORD nMaxFile, LPSTR lpFilter);
bool OpenScriptFile(char* szFileName);
bool SaveScriptFile(char* szFileName);

void StartComplie();
VOID Compile();
int SectionRead(wchar_t* Buffer, wchar_t* SecName);
void StringRead(wchar_t* Buffer, wchar_t* lpBufOut, size_t cbBufSize, wchar_t* SecName, UINT StringNum);
void EscapeSequence(wchar_t* toCheck);
bool CommandLine(LPTSTR lpCmdLine);

void Logger(HWND hWnd, char* fmt, ...);

char* szMainClassName = "Keymagic_KMPER";

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG messages;
    WNDCLASSEX wincl;

	hInst = hInstance;

	lpCmd = lpCmdLine;

	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, WndProc);

	return 0;
}

bool CommandLine(){
	if (!lstrlen(lpCmd))
		return false;
	
	if (lpCmd[1] == 'c'){
		OpenScriptFile(&lpCmd[3]);
		Compile();
		return true;
	}

	if (lpCmd[1] == 'o')
	{
		OpenScriptFile(&lpCmd[3]);
	}

	return false;
}



INT_PTR CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	char szFileName[260];

	switch (message)
	{
	case WM_INITDIALOG:
		hEdit = GetDlgItem(hWnd, IDC_SCRIPT);

		if (CommandLine()){
			EndDialog(hWnd, 0);
		}

		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_KEYMAPPER)));

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_OPEN:
			
			if (OpenDialog(hWnd, szFileName, 256) == false)
				break;
			OpenScriptFile(szFileName);
			SendDlgItemMessage(hWnd, IDC_SCRIPT, EM_SETMODIFY, false, 0);
			break;

		case ID_FILE_SAVE:
			if (szOpenedFileName[0] == NULL){
				if (SaveDialog(hWnd, szFileName, 256, "Keymagic Script File(*.kms)\0*.kms\0\0") == false)
					break;
			}

			SaveScriptFile(szOpenedFileName);
			SendDlgItemMessage(hWnd, IDC_SCRIPT, EM_SETMODIFY, false, 0);
			break;

		case ID_FILE_SAVEAS:

			if (SaveDialog(hWnd, szFileName, 256, "Keymagic Script File(*.kms)\0*.kms\0\0") == false)
				break;
			SaveScriptFile(szFileName);
			SendDlgItemMessage(hWnd, IDC_SCRIPT, EM_SETMODIFY, false, 0);
			break;

		case ID_EDIT_MAKEKEYBOARD:
			Compile();
			break;

		case ID_EDIT_FONT:
			CHOOSEFONTW cf;

			cf.lStructSize = sizeof(CHOOSEFONT);
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &lf;
			cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
			cf.hInstance = hInst;
			if (ChooseFontW(&cf))
				SendMessage(hEdit, WM_SETFONT, (WPARAM)CreateFontIndirectW(&lf), true);
			break;

		case ID_FILE_EXIT:
			EndDialog(hWnd, 0);
			break;

		case ID_HELP_ABOUT:
			DialogBoxA(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		break;
	case WM_SIZE:
		MoveWindow(GetDlgItem(hWnd, IDC_SCRIPT), 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;

	case WM_DROPFILES:
		HDROP hDrop;
		hDrop = (HDROP)wParam;
		DragQueryFile(hDrop, 0,(LPSTR)szFileName, MAX_PATH);
		DragFinish(hDrop);
		OpenScriptFile(szFileName);
		break;

	case WM_CLOSE:
		//if (SendDlgItemMessage(hWnd, IDC_SCRIPT, EM_GETMODIFY, 0, 0)){
		//	if (MessageBox(hWnd, "The file is not saved. Are you sure want to exit without saving?", "Keymagic", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		//		break;
		//}
		EndDialog(hWnd, 0);
		break;
	}

	return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool OpenDialog(HWND hwnd, char* szFileName,DWORD nMaxFile)
{
	OPENFILENAME ofn = {0};

	RtlZeroMemory(szFileName, nMaxFile);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = hInst;
	ofn.lpstrFilter = "Script File(*.kms)\0*.kms\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = nMaxFile;
	ofn.lpstrTitle = "Open File...";
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | 
		OFN_LONGNAMES | OFN_HIDEREADONLY;

	if(!GetOpenFileName(&ofn)) return false;
	return true;
}

bool SaveDialog(HWND hwnd, char* szFileName, DWORD nMaxFile, LPSTR lpFilter)
{
	OPENFILENAME ofn = {0};

	RtlZeroMemory(szFileName, nMaxFile);

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = hInst;
	ofn.lpstrFilter = lpFilter;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = nMaxFile;
	ofn.lpstrTitle = "Save File...";
	ofn.lpstrDefExt = "*kmk";
	ofn.Flags = OFN_EXPLORER | OFN_LONGNAMES | OFN_HIDEREADONLY;

	if(!GetSaveFileName(&ofn)) return false;
	return true;
}

bool OpenScriptFile(char* szFileName){

	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	size_t size = GetFileSize(hFile, NULL);
	HANDLE hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, NULL, NULL, NULL);

	if(hMapFile == NULL)
	{
		CloseHandle(hFile);
		return false;
	}

	LPCWSTR FilePtr = (LPCWSTR)MapViewOfFile(hMapFile, FILE_MAP_READ, NULL, NULL, NULL);

	if(FilePtr == NULL)
	{
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		return false;
	}

	lf.lfHeight = -MulDiv(11, GetDeviceCaps(GetDC(hEdit), LOGPIXELSY), 72);
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
	lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	lf.lfQuality = PROOF_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	int count = SectionRead((wchar_t*)FilePtr, L"Start");

	if (count == -1)
		goto NoFont;
	count++;

	wchar_t *Font;
	for (int i=0; i < count; i++)
	{
		wchar_t FontName[40],Name[5] ={0};
		StringRead((wchar_t*)FilePtr, FontName, 30, L"Start", i);
		wcsncpy(Name, FontName, 4);
		if (wcsicmp(Name, L"Font") != 0)
			continue;
		Font = wcspbrk(FontName, L" =");
		Font++;
		for (wchar_t *Finder=(wchar_t*)0xDEAD; Finder != NULL; Finder){
			Finder = wcspbrk(Font++, L" =");
			if (Finder == NULL)
				goto Font;
		}
		break;
	}

	goto NoFont;
Font:
	Font--;

	wcscpy(lf.lfFaceName, Font);

	SendMessage(hEdit, WM_SETFONT, (WPARAM)CreateFontIndirectW(&lf), true);

NoFont:

	CloseHandle(hMapFile);
	CloseHandle(hFile);

//	SendMessage(hEdit, EM_SETMODIFY, 0, 0);
	SetWindowTextW(hEdit, FilePtr);
	SendMessage(hEdit, EM_SETSEL, -1, 0);

	lstrcpy(szOpenedFileName, szFileName);

	return true;
}

bool isTRUE(wchar_t *pMem, wchar_t *Matcher)
{
	if (wcsstr(pMem, (const wchar_t*)Matcher))
		return true;
	return false;
}

void CALLBACK CallbackMatch(RegularExpression * cre,const REMatchInfo & Matches,unsigned LineIndex,unsigned StartOfLine)
{
	OutputDebugString("Match Found\r\n");

	OutputDebugString(Matches[0].string.c_str());

	OutputDebugString("\r\n");

	for (REMatchInfo::const_iterator cit = Matches.begin() + 1; cit != Matches.end() - 2; ++cit)
	{
		OutputDebugString(cit->string.c_str());
		OutputDebugString("\r\n");
	}
}

VOID Compile()
{
	int iChar = GetWindowTextLength(hEdit);
	CHAR* cScript = new CHAR[iChar+2];
	int iFetched = GetWindowText(hEdit, cScript, iChar+1);
	KParser kp(cScript);
	RegularExpression regex;
	regex.SetExpression("\\$.*\\(.*\"(.+)\".*\\).*:.*\\$.*\\(.*\"(.+)\".*\\)");
	regex.SetStringToMatch(kp.GetLine(1));
	regex.OnMatchingGlobal = CallbackMatch;
	bool success = regex.Match();
}

void StartComplie(){
	int cbCount, Length;
	wchar_t String[256];
	wchar_t* Buffer;
	wchar_t szSingle[] = L"SingleInput";
	wchar_t szMulti[] = L"MultiOutput";
	wchar_t szCustomize[] = L"Customize";
	wchar_t szDelete[] = L"CustomizeOnDelete";
	wchar_t tokens[] = L" =";
	wchar_t* token;
	One2One Normal;
	KbFileHeader *Header;
	File_Custom *Custom;
	File_One2Multi *Multi;
	File_Delete *DeletePattern;

	int count;
	size_t memsize;

	Header = (KbFileHeader*)LocalAlloc(LPTR, 5000);
	Header->Magic = 'FKMK';

	Length = GetWindowTextLengthW(hEdit);

	if (Length == NULL)
		return;

	Buffer = (wchar_t*)VirtualAlloc(NULL, Length, MEM_COMMIT, PAGE_READWRITE);

	memsize = sizeof(KbFileHeader);

	GetWindowTextW(hEdit, Buffer, Length);

	// "Normal"

	count = SectionRead(Buffer, szSingle);

	if (count == -1)
		goto Multi;

	StringRead(Buffer, String, 256, szSingle, 0);

	token = wcstok(String, tokens);

	if (token == NULL)
		goto Multi;

	Normal.Input = (wchar_t*)((LPBYTE)Header + sizeof (KbFileHeader));
	wcscpy((wchar_t*)Normal.Input, token);
	EscapeSequence((wchar_t*)Normal.Input);

	Header->One_Count = wcslen(token);

	memsize += Header->One_Count * 4;

	token = wcstok(NULL, tokens);
	if (token == NULL)
			goto Multi;

	Normal.Output = (wchar_t*)((LPBYTE)Header + sizeof (KbFileHeader) + Header->One_Count * 2);
	wcscpy((wchar_t*)Normal.Output, token);
	EscapeSequence((wchar_t*)Normal.Output);

Multi:
	Multi = (File_One2Multi*)((LPBYTE)Header + sizeof (KbFileHeader) + Header->One_Count *4);

	count = SectionRead(Buffer, szMulti);

	if (count == -1){
		count = 0;
		goto Customize;
	}
	count++;

	Header->Multi_Count = count;
	int i=0;
	
	while (i < count){
		StringRead (Buffer, String, 256, szMulti, i);

		token = wcstok(String, tokens);
		if (token == NULL)
			continue;

		Multi->size = sizeof(short) + sizeof (DWORD);

		wchar_t *vKey = wcsrchr(token, '+');
		if (vKey)
			wcscpy((wchar_t*)&Multi->One2Multi.Input_Key, vKey + 1);
		else
			wcscpy((wchar_t*)&Multi->One2Multi.Input_Key, token);

		EscapeSequence((wchar_t*)&Multi->One2Multi.Input_Key);

		Multi->One2Multi.SHIFT = isTRUE(token,L"SHIFT");
		Multi->One2Multi.CTRL = isTRUE(token,L"CTRL");
		Multi->One2Multi.L_ALT = isTRUE(token,L"L_ALT");
		Multi->One2Multi.R_ALT = isTRUE(token,L"R_ALT");

		token = wcstok(NULL, tokens);
		if (token == NULL)
			continue;
		wcscpy((wchar_t*)&Multi->One2Multi.Output, token);
		EscapeSequence((wchar_t*)&Multi->One2Multi.Output);
		
		Multi->size += wcslen(Multi->One2Multi.Output) * sizeof(wchar_t) + sizeof(short);
		memsize += Multi->size;

		Multi = (File_One2Multi*) ((LPBYTE)Multi + Multi->size);

		i++;
	}

	memsize += 6;

Customize:
	Custom  = (File_Custom*) ((LPBYTE)Multi + Multi->size);

	count = SectionRead(Buffer, szCustomize);

	if (count == -1){
		count = 0;
		goto Delete;
	}
	count++;

	Header->Customize_Count = count;

	int length=0;
	for (int i=0; i < Header->Customize_Count; i++)
	{
		Custom = (File_Custom*) ( (LPBYTE)Custom + length );

		StringRead (Buffer, String, 256, szCustomize, i);

		token = wcstok(String, tokens);
		if (token == NULL)
			break;

		short *MP_length = (short*)( (LPBYTE)&Custom->size_MatchPattern);
		wchar_t *MP = (wchar_t*)((LPBYTE)&Custom->size_MatchPattern + sizeof(short));

		*MP_length = wcslen(token);

		wcscpy((wchar_t*)MP, token);
		EscapeSequence((wchar_t*)MP);

		token = wcstok(NULL, tokens);

		if (token == NULL)
			continue;

		short *OP_length = (short*)( (LPBYTE) (MP + *MP_length) );
		wchar_t *OP = (wchar_t*) ((LPBYTE) ( MP + *MP_length )+ sizeof(short)); 

		*OP_length = wcslen(token);

		wcscpy((wchar_t*)OP, token);
		EscapeSequence((wchar_t*)OP);

		length = ( *MP_length + *OP_length + sizeof(short) ) * 2;
		memsize += length;
	}


Delete:

	DeletePattern  = (File_Delete*) ( (LPBYTE)Custom + length );
	length=0;

	count = SectionRead(Buffer, szDelete);

	if (count == -1){
		count = 0;
		goto WriteFile;
	}

	count++;

	Header->Back_Count = count;

	for (int i=0; i < Header->Back_Count; i++)
	{
		DeletePattern = (File_Delete*) ( (LPBYTE)DeletePattern + length );

		StringRead (Buffer, String, 256, szDelete, i);

		token = wcstok(String, tokens);
		if (token == NULL)
			break;

		short *MP_length = (short*)( (LPBYTE)&DeletePattern->size_MatchPattern);
		wchar_t *MP = (wchar_t*)((LPBYTE)&DeletePattern->size_MatchPattern + sizeof(short));

		*MP_length = wcslen(token);

		wcscpy((wchar_t*)MP, token);
		EscapeSequence((wchar_t*)MP);

		token = wcstok(NULL, tokens);

		if (token == NULL)
			continue;

		short *OP_length = (short*)( (LPBYTE) (MP + *MP_length) );
		wchar_t *OP = (wchar_t*) ((LPBYTE) ( MP + *MP_length )+ sizeof(short)); 

		*OP_length = wcslen(token);
		wcscpy((wchar_t*)OP, token);

		EscapeSequence((wchar_t*)OP);

		length = ( *MP_length + *OP_length + sizeof(short) ) * 2;
		memsize += length;
	}

	// ===----------------------------===
WriteFile:

	char szFileName[256];

	if (!SaveDialog(GetFocus(), szFileName, 256, "Keymagic's Key Map(*.km2)\0*.km2\0\0"))
		goto End;

	HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		LocalFree(Header);
		VirtualFree(Buffer, Length, MEM_DECOMMIT);
		return;
	}

	DWORD cbWritten;
	WriteFile(hFile, Header, memsize, &cbWritten, NULL);
	CloseHandle(hFile);

End:
	LocalFree(Header);

	VirtualFree(Buffer, Length, MEM_DECOMMIT);
}

int SectionRead(wchar_t* Buffer, wchar_t* SecName){
	wchar_t *pdest, *start, *end;

	wchar_t SecToFind[50];

	wsprintfW(SecToFind, L"[%s]", SecName);

	end = pdest = wcsstr(Buffer, SecToFind);

	if (pdest == NULL)
		return -1;

	int i=-1;

	start = (wchar_t*) wcsstr( end, L"\r\n");
	start+=2;
	end = (wchar_t*) wcsstr( start, L"\r\n" );
	end--;

	while ( start[0] != '[' || end[0] != L']')
	{
		i++;
		start = (wchar_t*) wcsstr( end, L"\r\n");
		if (start == NULL)
			break;
		start+=2;
		end = (wchar_t*) wcsstr( start, L"\r\n" );
		if (end == NULL)
			break;
		end--;
	}

	return i;
}

void StringRead(wchar_t* Buffer, wchar_t* lpBufOut, size_t cbBufSize, wchar_t* SecName, UINT StringNum){

	wchar_t *pdest, *start, *end;
	wchar_t SecToFind[50];

	wsprintfW(SecToFind, L"[%s]", SecName);

	end = pdest = wcsstr(Buffer, SecToFind);

	int i=-1;

	do{
		start = (wchar_t*) wcsstr( end, L"\r\n");
		start+=2;
		end = (wchar_t*) wcsstr( start, L"\r\n" );
		end--;
		i++;
	}while (i < StringNum);

	int len = end-start;
	len++;

	if (len > cbBufSize)
		return;

	wcsncpy(lpBufOut, start, len);

	lpBufOut[len] = 0;

}

void EscapeSequence(wchar_t* toCheck){

	wchar_t Checked[300]={0};

	for (int i=0, j=0; toCheck[j] != NULL; i++, j++){

		if (toCheck[j] != '\\')
			Checked[i] = toCheck[j];

		else if ((toCheck[j] == '\\') && (toCheck[j+1] == '\\')){
			j++;
			Checked[i] = '\\';
		}

		else if (toCheck[j] == '\\'){
			j++;
			Checked[i] = _wtoi(&toCheck[j++]);
			if (Checked[i] > 99)
				j+=2;
		}
	}

	wcscpy(toCheck, Checked);

}

bool SaveScriptFile(char* szFileName){
	HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE){
		CloseHandle(hFile);
		return false;
	}

	int size = GetWindowTextLengthW(hEdit);

	if (size == NULL){
		return false;
	}

	size *= 3;

	LPVOID Buffer = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

	int readsize = GetWindowTextW(hEdit,(LPWSTR) Buffer, size/2);

	if (readsize == NULL){
		CloseHandle(hFile);
		VirtualFree(Buffer, size, MEM_DECOMMIT);
		return false;
	}

	DWORD cbWritten;

	WriteFile(hFile, Buffer, size, &cbWritten, NULL);

	CloseHandle(hFile);
	VirtualFree(Buffer, size, MEM_DECOMMIT);

	if (cbWritten != size)
		return false;

	return true;
}

void Logger(HWND hWnd, char* fmt, ...)
{
#ifdef DEBUG
	char Memory[100];
	RECT rc;

	va_list list;

	va_start(list, fmt);
	//Format
	wvsprintf(Memory, fmt,list);

	GetClientRect(hWnd, &rc);

	rc.top += 50;
	SetWindowText(hWnd, Memory);
	
	//Cleanup
	va_end(list);
#else
	return;
#endif
}