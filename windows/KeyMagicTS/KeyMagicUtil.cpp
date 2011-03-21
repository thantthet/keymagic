#include <ShlObj.h>
#include "KeyMagicUtil.h"
using namespace std;

KEYBOARD_LIST * SearchKeyboards(const wstring& dir) {
	wstring findPattern = wstring(dir);
	findPattern += L"*.km2";

	KEYBOARD_LIST * keyboards = new KEYBOARD_LIST();

	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW(findPattern.c_str(), &fd);
	while (hFind  != INVALID_HANDLE_VALUE) {
		
		wstring keyboardLayout = wstring(dir) + wstring(fd.cFileName);
		keyboards->push_back(keyboardLayout);

		if (FindNextFileW(hFind, &fd) == FALSE) {
			break;
		}
	}
	return keyboards;
}

KEYBOARD_LIST * GetKeyboards() {
	wstring findDir;
	WCHAR keymagicDir[] = L"\\KeyMagic\\KeyboardLayouts\\";
	WCHAR path[MAX_PATH];

	KEYBOARD_LIST *keyboards,
		*allKeyboards = new KEYBOARD_LIST();

	SHGetFolderPathW(0, CSIDL_APPDATA, 0, 0, path);
	findDir = wstring(path) + wstring(keymagicDir);
	keyboards = SearchKeyboards(findDir);
	for (KEYBOARD_LIST::iterator i = keyboards->begin(); i != keyboards->end(); i++) {
		allKeyboards->push_back(*i);
	}
	delete keyboards;

	SHGetFolderPathW(0, CSIDL_COMMON_APPDATA, 0, 0, path);
	findDir = wstring(path) + wstring(keymagicDir);
	keyboards = SearchKeyboards(findDir);
	for (KEYBOARD_LIST::iterator i = keyboards->begin(); i != keyboards->end(); i++) {
		allKeyboards->push_back(*i);
	}
	delete keyboards;

	return allKeyboards;
}

BOOL MapVK (UINT inVK, UINT * VKey, UINT * Char) {

	BYTE keyboardState[256];
	UINT scanCode;
	WCHAR translatedChar[1];

	//HKL usHKL = GetKeyboardLayout(0);
	scanCode = MapVirtualKey(inVK, MAPVK_VK_TO_VSC);
	*VKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK, (HKL)0x4090409);

	if (scanCode == 0) {
		return false;
	}

	GetKeyboardState(keyboardState);

	int Return = ToUnicodeEx(*VKey, scanCode, keyboardState, translatedChar, 1, 0, (HKL)0x4090409);

	if (Return == 0) {
		*Char = '\0';
		return false;
	}

	*Char = translatedChar[0];

	return true;
}

Gdiplus::Bitmap* LoadGdiBitmapFromBuffer (const char * buffer, size_t size, HGLOBAL * hBuffer) {
	HGLOBAL m_hBuffer;
	Gdiplus::Bitmap* m_pBitmap;

	m_hBuffer = GlobalAlloc(GMEM_MOVEABLE, size);

	if (m_hBuffer)
	{
		void* pBuffer = GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, buffer, size);

			IStream* pStream = NULL;
			if (CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (m_pBitmap)
				{ 
					if (m_pBitmap->GetLastStatus() != Gdiplus::Ok)
					{
						delete m_pBitmap;
						m_pBitmap = NULL;
					}
				}
			}
			GlobalUnlock(m_hBuffer);
		}
		//GlobalFree(m_hBuffer);
	}

	*hBuffer = m_hBuffer;
	return m_pBitmap;
}

HICON LoadIconFromBuffer (const char * buffer, size_t size) {
	HGLOBAL hBuffer;
	Gdiplus::Bitmap* m_pBitmap;
	HICON hIcon = NULL;

	m_pBitmap = LoadGdiBitmapFromBuffer(buffer, size, &hBuffer);
	if (m_pBitmap) {
		if (m_pBitmap->GetHICON(&hIcon) != S_OK) {
			hIcon = NULL;
			DebugStr("Failed to load HICON. %d\n", m_pBitmap->GetLastStatus());
		}
		delete m_pBitmap;
	}

	//GlobalUnlock(hBuffer);
	GlobalFree(hBuffer);

	return hIcon;
}

HBITMAP LoadBitmapFromBuffer (const char * buffer, size_t size) {
	HGLOBAL hBuffer;
	Gdiplus::Bitmap* m_pBitmap;
	HBITMAP hBitmap = NULL;

	m_pBitmap = LoadGdiBitmapFromBuffer(buffer, size, &hBuffer);
	if (m_pBitmap) {
		if (m_pBitmap->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap) != S_OK) {
			hBitmap = NULL;
			DebugStr("Failed to load HBITMAP. %d\n", m_pBitmap->GetLastStatus());
		}
		delete m_pBitmap;
	}

	//GlobalUnlock(hBuffer);
	GlobalFree(hBuffer);

	return hBitmap;
}

HICON LoadIconFromKeyboard(const InfoList& infos)
{
	HICON hIcon = NULL;
	if (infos.find('icon') != infos.end()) {
		Info icon = infos.find('icon')->second;
		hIcon = LoadIconFromBuffer(icon.data, icon.size);
	}
	return hIcon;
}

HBITMAP LoadBitmapFromKeyboard(const InfoList& infos)
{
	HBITMAP hBitmap = NULL;
	if (infos.find('icon') != infos.end()) {
		Info icon = infos.find('icon')->second;
		hBitmap = LoadBitmapFromBuffer(icon.data, icon.size);
	}
	return hBitmap;
}

wstring * FromMultiByteString(string str) {
	int len = str.length();
	WCHAR * wc = new WCHAR[len+1];
	wc[MultiByteToWideChar(CP_ACP, 0, str.c_str(), len, wc, len)] = 0;
	wstring * wstr = new wstring(wc);
	delete[] wc;
	return wstr;
}

wstring * GetKeyboardName(const InfoList& infos) {
	if (infos.find('name') != infos.end()) {
		wstring * wstr = FromMultiByteString(infos.find('name')->second.data);
		return wstr;
	}
	return NULL;
}

wstring * GetKeyboardNameOrFileTitle(const InfoList& infos, const wstring& path) {
	wstring * wstr = GetKeyboardName(infos);
	if (wstr) {
		return wstr;
	}

	WCHAR keyboardName[MAX_PATH] = {0};
	GetFileTitleW(path.c_str(), keyboardName, MAX_PATH);
	int length = wcslen(keyboardName);
	if (length) {
		length -= 4;
		keyboardName[length] = '\0';
	}
	wstr = new wstring(keyboardName);

	return wstr;
}

wstring * GetDescription(const InfoList& infos) {
	wstring * wstr;
	if (infos.find('desc') != infos.end()) {
		wstr = FromMultiByteString(infos.find('desc')->second.data);
	} else {
		wstr = new wstring(L"No Description");
	}
	return wstr;
}

void DebugStr (char * fmt, ...) {
	va_list vl;
	va_start (vl, fmt);
	char msg[1001];
	vsnprintf(msg, 1000, fmt, vl);
	OutputDebugStringA(msg);
	va_end(vl);
}

void DebugStr (WCHAR * fmt, ...) {
	va_list vl;
	va_start (vl, fmt);
	WCHAR msg[1001];
	vswprintf(msg, 1000, fmt, vl);
	OutputDebugStringW(msg);
	va_end(vl);
}