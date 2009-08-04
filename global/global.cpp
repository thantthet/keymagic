
#include "global.h"

std::vector<structPREdef> vTpreDef;

void push_pre_define_value( emPreDef preDef, const wchar_t * name, const wchar_t * value )
{
	structPREdef preDefstruct;

	preDefstruct.preDef = preDef;
	preDefstruct.name = name;
	preDefstruct.value = value;

	vTpreDef.push_back(preDefstruct);
}

void pre_define()
{
	push_pre_define_value(pdNULL,		L"NULL",		L"");

	push_pre_define_value(pdVK_BACK,	L"VK_BACK",		L"\x0008");
	push_pre_define_value(pdVK_TAB,		L"VK_TAB",		L"\x0009");
	push_pre_define_value(pdVK_RETURN,	L"VK_RETURN",	L"\x000D");
	push_pre_define_value(pdVK_SHIFT,	L"VK_SHIFT",	L"\x0010");
	push_pre_define_value(pdVK_CONTROL,	L"VK_CONTROL",	L"\x0011");
	push_pre_define_value(pdVK_MENU,	L"VK_ALT",		L"\x0012");
	push_pre_define_value(pdVK_PAUSE,	L"VK_PAUSE",	L"\x0013");
	push_pre_define_value(pdVK_CAPITAL,	L"VK_CAPITAL",	L"\x0014");
	push_pre_define_value(pdVK_KANJI,	L"VK_KANJI",	L"\x0019");
	push_pre_define_value(pdVK_ESCAPE,	L"VK_ESCAPE",	L"\x001B");
	push_pre_define_value(pdVK_SPACE,	L"VK_SPACE",	L"\x0020");
	push_pre_define_value(pdVK_PRIOR,	L"VK_PRIOR",	L"\x0021");
	push_pre_define_value(pdVK_NEXT,	L"VK_NEXT",		L"\x0022");
	push_pre_define_value(pdVK_DELETE,	L"VK_DELETE",	L"\x002E");

	push_pre_define_value(pdVK_KEY_0,	L"VK_KEY_0",	L"0");
	push_pre_define_value(pdVK_KEY_1,	L"VK_KEY_1",	L"1");
	push_pre_define_value(pdVK_KEY_2,	L"VK_KEY_2",	L"2");
	push_pre_define_value(pdVK_KEY_3,	L"VK_KEY_3",	L"3");
	push_pre_define_value(pdVK_KEY_4,	L"VK_KEY_4",	L"4");
	push_pre_define_value(pdVK_KEY_5,	L"VK_KEY_5",	L"5");
	push_pre_define_value(pdVK_KEY_6,	L"VK_KEY_6",	L"6");
	push_pre_define_value(pdVK_KEY_7,	L"VK_KEY_7",	L"7");
	push_pre_define_value(pdVK_KEY_8,	L"VK_KEY_8",	L"8");
	push_pre_define_value(pdVK_KEY_9,	L"VK_KEY_9",	L"9");

	push_pre_define_value(pdVK_KEY_A,	L"VK_KEY_A",	L"A");
	push_pre_define_value(pdVK_KEY_B,	L"VK_KEY_B",	L"B");
	push_pre_define_value(pdVK_KEY_C,	L"VK_KEY_C",	L"C");
	push_pre_define_value(pdVK_KEY_D,	L"VK_KEY_D",	L"D");
	push_pre_define_value(pdVK_KEY_E,	L"VK_KEY_E",	L"E");
	push_pre_define_value(pdVK_KEY_F,	L"VK_KEY_F",	L"F");
	push_pre_define_value(pdVK_KEY_G,	L"VK_KEY_G",	L"G");
	push_pre_define_value(pdVK_KEY_H,	L"VK_KEY_H",	L"H");
	push_pre_define_value(pdVK_KEY_I,	L"VK_KEY_I",	L"I");
	push_pre_define_value(pdVK_KEY_J,	L"VK_KEY_J",	L"J");
	push_pre_define_value(pdVK_KEY_K,	L"VK_KEY_K",	L"K");
	push_pre_define_value(pdVK_KEY_L,	L"VK_KEY_L",	L"L");
	push_pre_define_value(pdVK_KEY_M,	L"VK_KEY_M",	L"M");
	push_pre_define_value(pdVK_KEY_N,	L"VK_KEY_N",	L"N");
	push_pre_define_value(pdVK_KEY_O,	L"VK_KEY_O",	L"O");
	push_pre_define_value(pdVK_KEY_P,	L"VK_KEY_P",	L"P");
	push_pre_define_value(pdVK_KEY_Q,	L"VK_KEY_Q",	L"Q");
	push_pre_define_value(pdVK_KEY_R,	L"VK_KEY_R",	L"R");
	push_pre_define_value(pdVK_KEY_S,	L"VK_KEY_S",	L"S");
	push_pre_define_value(pdVK_KEY_T,	L"VK_KEY_T",	L"T");
	push_pre_define_value(pdVK_KEY_U,	L"VK_KEY_U",	L"U");
	push_pre_define_value(pdVK_KEY_V,	L"VK_KEY_V",	L"V");
	push_pre_define_value(pdVK_KEY_W,	L"VK_KEY_W",	L"W");
	push_pre_define_value(pdVK_KEY_X,	L"VK_KEY_X",	L"X");
	push_pre_define_value(pdVK_KEY_Y,	L"VK_KEY_Y",	L"Y");
	push_pre_define_value(pdVK_KEY_Z,	L"VK_KEY_Z",	L"Z");

	push_pre_define_value(pdVK_NUMPAD0,	L"VK_NUMPAD0",	L"\x0060");
	push_pre_define_value(pdVK_NUMPAD1,	L"VK_NUMPAD1",	L"\x0061");
	push_pre_define_value(pdVK_NUMPAD2,	L"VK_NUMPAD2",	L"\x0062");
	push_pre_define_value(pdVK_NUMPAD3,	L"VK_NUMPAD3",	L"\x0063");
	push_pre_define_value(pdVK_NUMPAD4,	L"VK_NUMPAD4",	L"\x0064");
	push_pre_define_value(pdVK_NUMPAD5,	L"VK_NUMPAD5",	L"\x0065");
	push_pre_define_value(pdVK_NUMPAD6,	L"VK_NUMPAD6",	L"\x0066");
	push_pre_define_value(pdVK_NUMPAD7,	L"VK_NUMPAD7",	L"\x0067");
	push_pre_define_value(pdVK_NUMPAD8,	L"VK_NUMPAD8",	L"\x0068");
	push_pre_define_value(pdVK_NUMPAD9,	L"VK_NUMPAD9",	L"\x0069");

	push_pre_define_value(pdVK_MULTIPLY,L"VK_MULTIPLY",	L"\x006A");
	push_pre_define_value(pdVK_ADD,		L"VK_ADD",		L"\x006B");
	push_pre_define_value(pdVK_SEPARATOR,L"VK_SEPARATOR",L"\x006C");
	push_pre_define_value(pdVK_SUBTRACT,L"VK_SUBTRACT",	L"\x006D");
	push_pre_define_value(pdVK_DECIMAL,	L"VK_DECIMAL",	L"\x006E");
	push_pre_define_value(pdVK_DIVIDE,	L"VK_DIVIDE",	L"\x006F");

	push_pre_define_value(pdVK_F1,		L"VK_F1",		L"\x0070");
	push_pre_define_value(pdVK_F2,		L"VK_F2",		L"\x0071");
	push_pre_define_value(pdVK_F3,		L"VK_F3",		L"\x0072");
	push_pre_define_value(pdVK_F4,		L"VK_F4",		L"\x0073");
	push_pre_define_value(pdVK_F5,		L"VK_F5",		L"\x0074");
	push_pre_define_value(pdVK_F6,		L"VK_F6",		L"\x0075");
	push_pre_define_value(pdVK_F7,		L"VK_F7",		L"\x0076");
	push_pre_define_value(pdVK_F8,		L"VK_F8",		L"\x0077");
	push_pre_define_value(pdVK_F9,		L"VK_F9",		L"\x0078");
	push_pre_define_value(pdVK_F10,		L"VK_F10",		L"\x0079");
	push_pre_define_value(pdVK_F11,		L"VK_F11",		L"\x007A");
	push_pre_define_value(pdVK_F12,		L"VK_F12",		L"\x007B");

	push_pre_define_value(pdVK_OEM_1,		L"VK_OEM_1",		L"\x00BA");
	push_pre_define_value(pdVK_OEM_PLUS,	L"VK_OEM_PLUS",		L"\x00BB");
	push_pre_define_value(pdVK_OEM_COMMA,	L"VK_OEM_COMMA",	L"\x00BC");
	push_pre_define_value(pdVK_OEM_MINUS,	L"VK_OEM_MINUS",	L"\x00BD");
	push_pre_define_value(pdVK_OEM_PERIOD,	L"VK_OEM_PERIOD",	L"\x00BE");
	push_pre_define_value(pdVK_OEM_2,		L"VK_OEM_2",		L"\x00BF");
	push_pre_define_value(pdVK_OEM_3,		L"VK_OEM_3",		L"\x00C0");

	push_pre_define_value(pdVK_OEM_4,	L"VK_OEM_4",		L"\x00DB");
	push_pre_define_value(pdVK_OEM_5,	L"VK_OEM_5",		L"\x00DC");
	push_pre_define_value(pdVK_OEM_6,	L"VK_OEM_6",		L"\x00DD");
	push_pre_define_value(pdVK_OEM_7,	L"VK_OEM_7",		L"\x00DE");
	push_pre_define_value(pdVK_OEM_8,	L"VK_OEM_8",		L"\x00DF");

	push_pre_define_value(pdVK_OEM_AX,	L"VK_OEM_AX",		L"\x00E1");
	push_pre_define_value(pdVK_OEM_102,	L"VK_OEM_102",		L"\x00E2");
	push_pre_define_value(pdVK_ICO_HELP,L"VK_ICO_HELP",		L"\x00E3");
	push_pre_define_value(pdVK_ICO_00,	L"VK_ICO_00",		L"\x00E4");
}

structPREdef * getPreDef(emPreDef preDef)
{
	if (!vTpreDef.size())
		pre_define();

	for (std::vector<structPREdef>::iterator it = vTpreDef.begin();
		it != vTpreDef.end(); it++)
	{
		if ((*it).preDef == preDef)
			return &(*it);
	}
	return NULL;
}

structPREdef * getPreDef(wchar_t * wcSrc, int length,  bool isName)
{
	if (!vTpreDef.size())
		pre_define();

	wchar_t * word = new wchar_t [length];
	wcsncpy(word, wcSrc, length);
	word[length] = 0;

	for (std::vector<structPREdef>::iterator it = vTpreDef.begin();
		it != vTpreDef.end(); it++)
	{
		if (!wcscmp(word, isName ? (*it).name : (*it).value))
		{
			return &(*it);
		}
	}
	return NULL;
}

wchar_t * Type2Str(emType Type)
{
	switch (Type)
	{
	case T_ADDOP:
		return L"ADD_OPERATOR";
		break;
	case T_ANDOP:
		return L"AND_OPERATOR";
		break;
	case T_ASSIGN:
		return L"ASSIGN";
		break;
	case T_UNICODE:
		return L"UNICODE";
		break;
	case T_STRING:
		return L"STRING";
		break;
	case T_REFERENCE:
		return L"REFERENCE";
		break;
	case T_NEWLINE:
		return L"NEWLINE";
		break;
	case T_IDENTIFIER:
		return L"IDENTIFIER";
		break;
	case T_MODIFIER:
		return L"MODIFIER";
		break;
	case T_PRINT:
		return L"PRINT";
		break;
	case T_PREDEFINED:
		return L"PREDEFINED";
		break;
	case T_COMBINE_START:
		return L"T_COMBINE_START";
		break;
	case T_COMBINE_END:
		return L"T_COMBINE_END";
		break;
	}
	return L"UNKNOWN";
}

void Debug(LPCWSTR fmt, ...)
{
    WCHAR pBuffer[1024];

    va_list args = NULL;
    va_start(args, fmt);

    wvsprintfW(pBuffer, fmt, args);

    va_end(args);

	OutputDebugStringW(pBuffer);
}


void DumpToken(wchar_t * d,structToken kToken)
{
	Debug(
		L"%s => Length = %d, StartIndex = %d, Type = %s, Value = '%s'\n", 
		d, kToken.iLength, kToken.iStartIndex, 
		Type2Str(kToken.Type), 
		kToken.Value
		);
}

void Exit(int _Code, LPCWSTR fmt, ...)
{
    WCHAR pBuffer[1024];

    va_list args = NULL;
    va_start(args, fmt);

    wvsprintfW(pBuffer, fmt, args);

    va_end(args);

	wprintf(pBuffer);

	Debug(L"Application is ending\n");
	system("pause");
	exit(_Code);
}

void PrintLastError() {
	LPSTR errBuf = NULL;

	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPSTR)&errBuf, 
		0, 
		NULL);

	std::cout << errBuf;

	LocalFree(errBuf);
}