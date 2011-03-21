#ifndef KEYMAGICUTIL_H
#define KEYMAGICUTIL_H

#include <windows.h>
#include <string>
#include <vector>
#include <list>
#include <GdiPlus.h>
#include <keymagic.h>

using namespace std;

typedef vector<wstring> KEYBOARD_LIST;

KEYBOARD_LIST * GetKeyboards();
BOOL MapVK (UINT inVK, UINT * VKey, UINT * Char);
Gdiplus::Bitmap* LoadGdiBitmapFromBuffer (const char * buffer, size_t size, HGLOBAL * hBuffer);
HICON LoadIconFromBuffer (const char * buffer, size_t size);
HBITMAP LoadBitmapFromBuffer (const char * buffer, size_t size);
HICON LoadIconFromKeyboard(const InfoList& infos);
HBITMAP LoadBitmapFromKeyboard(const InfoList& infos);

wstring * FromMultiByteString(string str);
wstring * GetKeyboardName(const InfoList& infos);
wstring * GetKeyboardNameOrFileTitle(const InfoList& infos, const wstring& path);
wstring * GetDescription(const InfoList& infos);

void DebugStr (char * fmt, ...);
void DebugStr (WCHAR * fmt, ...);
#endif //KEYMAGICUTIL_H