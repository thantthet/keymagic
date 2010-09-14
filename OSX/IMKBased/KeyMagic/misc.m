//
//  misc.m
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

#import "misc.h"
#import "RegexKitLite.h"

size_t _wcslen(const short* ws)
{
	size_t length = 0;
	while (*ws++ != '\0') {
		length++;
	}
	return length;
}

NSMutableArray * aryPreDefined = NULL;

void addNewDefine(key _key, const wchar_t * _name, const wchar_t * _value){
	
	NSAutoreleasePool* pool = [NSAutoreleasePool new];
	
	preDefined * _preDefined = [preDefined new];
	[_preDefined setKey:_key];
	[_preDefined setName:_name];
	[_preDefined setValue:_value];
	
	if (aryPreDefined == NULL) {
		aryPreDefined = [NSMutableArray new];
	}
	[aryPreDefined addObject:_preDefined];
	[_preDefined autorelease];
	
	[pool release];
}

void initPreDefined()
{
	addNewDefine(pdNULL,		L"NULL",		L"");
	addNewDefine(pdNULL,		L"null",		L"");
	
	addNewDefine(pdVK_BACK,	L"VK_BACK",		L"\x0008");
	addNewDefine(pdVK_TAB,		L"VK_TAB",		L"\x0009");
	addNewDefine(pdVK_RETURN,	L"VK_RETURN",	L"\x000D");
	addNewDefine(pdVK_RETURN,	L"VK_ENTER",	L"\x000D");
	addNewDefine(pdVK_SHIFT,	L"VK_SHIFT",	L"\x0010");
	addNewDefine(pdVK_CONTROL,	L"VK_CONTROL",	L"\x0011");
	addNewDefine(pdVK_CONTROL,	L"VK_CTRL",		L"\x0011");
	addNewDefine(pdVK_MENU,	L"VK_MENU",		L"\x0012");
	addNewDefine(pdVK_MENU,	L"VK_ALT",		L"\x0012");
	addNewDefine(pdVK_PAUSE,	L"VK_PAUSE",	L"\x0013");
	addNewDefine(pdVK_CAPITAL,	L"VK_CAPITAL",	L"\x0014");
	addNewDefine(pdVK_CAPITAL,	L"VK_CAPSLOCK",	L"\x0014");
	addNewDefine(pdVK_KANJI,	L"VK_KANJI",	L"\x0019");
	addNewDefine(pdVK_ESCAPE,	L"VK_ESCAPE",	L"\x001B");
	addNewDefine(pdVK_ESCAPE,	L"VK_ESC",		L"\x001B");
	addNewDefine(pdVK_SPACE,	L"VK_SPACE",	L"\x0020");
	addNewDefine(pdVK_PRIOR,	L"VK_PRIOR",	L"\x0021");
	addNewDefine(pdVK_NEXT,	L"VK_NEXT",		L"\x0022");
	addNewDefine(pdVK_DELETE,	L"VK_DELETE",	L"\x002E");
	
	addNewDefine(pdVK_KEY_0,	L"VK_KEY_0",	L"0");
	addNewDefine(pdVK_KEY_1,	L"VK_KEY_1",	L"1");
	addNewDefine(pdVK_KEY_2,	L"VK_KEY_2",	L"2");
	addNewDefine(pdVK_KEY_3,	L"VK_KEY_3",	L"3");
	addNewDefine(pdVK_KEY_4,	L"VK_KEY_4",	L"4");
	addNewDefine(pdVK_KEY_5,	L"VK_KEY_5",	L"5");
	addNewDefine(pdVK_KEY_6,	L"VK_KEY_6",	L"6");
	addNewDefine(pdVK_KEY_7,	L"VK_KEY_7",	L"7");
	addNewDefine(pdVK_KEY_8,	L"VK_KEY_8",	L"8");
	addNewDefine(pdVK_KEY_9,	L"VK_KEY_9",	L"9");
	
	addNewDefine(pdVK_KEY_A,	L"VK_KEY_A",	L"A");
	addNewDefine(pdVK_KEY_B,	L"VK_KEY_B",	L"B");
	addNewDefine(pdVK_KEY_C,	L"VK_KEY_C",	L"C");
	addNewDefine(pdVK_KEY_D,	L"VK_KEY_D",	L"D");
	addNewDefine(pdVK_KEY_E,	L"VK_KEY_E",	L"E");
	addNewDefine(pdVK_KEY_F,	L"VK_KEY_F",	L"F");
	addNewDefine(pdVK_KEY_G,	L"VK_KEY_G",	L"G");
	addNewDefine(pdVK_KEY_H,	L"VK_KEY_H",	L"H");
	addNewDefine(pdVK_KEY_I,	L"VK_KEY_I",	L"I");
	addNewDefine(pdVK_KEY_J,	L"VK_KEY_J",	L"J");
	addNewDefine(pdVK_KEY_K,	L"VK_KEY_K",	L"K");
	addNewDefine(pdVK_KEY_L,	L"VK_KEY_L",	L"L");
	addNewDefine(pdVK_KEY_M,	L"VK_KEY_M",	L"M");
	addNewDefine(pdVK_KEY_N,	L"VK_KEY_N",	L"N");
	addNewDefine(pdVK_KEY_O,	L"VK_KEY_O",	L"O");
	addNewDefine(pdVK_KEY_P,	L"VK_KEY_P",	L"P");
	addNewDefine(pdVK_KEY_Q,	L"VK_KEY_Q",	L"Q");
	addNewDefine(pdVK_KEY_R,	L"VK_KEY_R",	L"R");
	addNewDefine(pdVK_KEY_S,	L"VK_KEY_S",	L"S");
	addNewDefine(pdVK_KEY_T,	L"VK_KEY_T",	L"T");
	addNewDefine(pdVK_KEY_U,	L"VK_KEY_U",	L"U");
	addNewDefine(pdVK_KEY_V,	L"VK_KEY_V",	L"V");
	addNewDefine(pdVK_KEY_W,	L"VK_KEY_W",	L"W");
	addNewDefine(pdVK_KEY_X,	L"VK_KEY_X",	L"X");
	addNewDefine(pdVK_KEY_Y,	L"VK_KEY_Y",	L"Y");
	addNewDefine(pdVK_KEY_Z,	L"VK_KEY_Z",	L"Z");
	
	addNewDefine(pdVK_NUMPAD0,	L"VK_NUMPAD0",	L"\x0060");
	addNewDefine(pdVK_NUMPAD1,	L"VK_NUMPAD1",	L"\x0061");
	addNewDefine(pdVK_NUMPAD2,	L"VK_NUMPAD2",	L"\x0062");
	addNewDefine(pdVK_NUMPAD3,	L"VK_NUMPAD3",	L"\x0063");
	addNewDefine(pdVK_NUMPAD4,	L"VK_NUMPAD4",	L"\x0064");
	addNewDefine(pdVK_NUMPAD5,	L"VK_NUMPAD5",	L"\x0065");
	addNewDefine(pdVK_NUMPAD6,	L"VK_NUMPAD6",	L"\x0066");
	addNewDefine(pdVK_NUMPAD7,	L"VK_NUMPAD7",	L"\x0067");
	addNewDefine(pdVK_NUMPAD8,	L"VK_NUMPAD8",	L"\x0068");
	addNewDefine(pdVK_NUMPAD9,	L"VK_NUMPAD9",	L"\x0069");
	
	addNewDefine(pdVK_MULTIPLY,L"VK_MULTIPLY",	L"\x006A");
	addNewDefine(pdVK_ADD,		L"VK_ADD",		L"\x006B");
	addNewDefine(pdVK_SEPARATOR,L"VK_SEPARATOR",L"\x006C");
	addNewDefine(pdVK_SUBTRACT,L"VK_SUBTRACT",	L"\x006D");
	addNewDefine(pdVK_DECIMAL,	L"VK_DECIMAL",	L"\x006E");
	addNewDefine(pdVK_DIVIDE,	L"VK_DIVIDE",	L"\x006F");
	
	addNewDefine(pdVK_F1,		L"VK_F1",		L"\x0070");
	addNewDefine(pdVK_F2,		L"VK_F2",		L"\x0071");
	addNewDefine(pdVK_F3,		L"VK_F3",		L"\x0072");
	addNewDefine(pdVK_F4,		L"VK_F4",		L"\x0073");
	addNewDefine(pdVK_F5,		L"VK_F5",		L"\x0074");
	addNewDefine(pdVK_F6,		L"VK_F6",		L"\x0075");
	addNewDefine(pdVK_F7,		L"VK_F7",		L"\x0076");
	addNewDefine(pdVK_F8,		L"VK_F8",		L"\x0077");
	addNewDefine(pdVK_F9,		L"VK_F9",		L"\x0078");
	addNewDefine(pdVK_F10,		L"VK_F10",		L"\x0079");
	addNewDefine(pdVK_F11,		L"VK_F11",		L"\x007A");
	addNewDefine(pdVK_F12,		L"VK_F12",		L"\x007B");
	
	addNewDefine(pdVK_LSHIFT,	L"VK_LSHIFT",	L"\x00A0");
	addNewDefine(pdVK_RSHIFT,	L"VK_RSHIFT",	L"\x00A1");
	
	addNewDefine(pdVK_LCONTROL,L"VK_LCONTROL",	L"\x00A2");
	addNewDefine(pdVK_LCONTROL,L"VK_LCTRL",	L"\x00A2");
	addNewDefine(pdVK_RCONTROL,L"VK_RCONTROL",	L"\x00A3");
	addNewDefine(pdVK_RCONTROL,L"VK_RCTRL",	L"\x00A3");
	
	addNewDefine(pdVK_LMENU,	L"VK_LMENU",	L"\x00A4");
	addNewDefine(pdVK_LMENU,	L"VK_LALT",		L"\x00A4");
	addNewDefine(pdVK_RMENU,	L"VK_RMENU",	L"\x00A5");
	addNewDefine(pdVK_RMENU,	L"VK_RALT",		L"\x00A5");
	addNewDefine(pdVK_RMENU,	L"VK_ALT_GR",	L"\x00A5");
	
	addNewDefine(pdVK_OEM_1,	L"VK_OEM_1",	L"\x003b"); //0x00BA (WIN)
	addNewDefine(pdVK_OEM_1,	L"VK_COLON",	L"\x003b");
	
	addNewDefine(pdVK_OEM_PLUS,	L"VK_OEM_PLUS",		L"\x003d");
	addNewDefine(pdVK_OEM_COMMA,	L"VK_OEM_COMMA",	L"\x002c");
	addNewDefine(pdVK_OEM_MINUS,	L"VK_OEM_MINUS",	L"\x002d");
	addNewDefine(pdVK_OEM_PERIOD,	L"VK_OEM_PERIOD",	L"\x002e");
	
	addNewDefine(pdVK_OEM_2,	L"VK_OEM_2",	L"\x002f"); //0x00BF
	addNewDefine(pdVK_OEM_2,	L"VK_QUESTION",	L"\x002f");
	
	addNewDefine(pdVK_OEM_3,	L"VK_OEM_3",	L"\x0060");
	addNewDefine(pdVK_OEM_3,	L"VK_CFLEX",	L"\x0060");
	
	addNewDefine(pdVK_OEM_4,	L"VK_OEM_4",	L"\x005b");
	addNewDefine(pdVK_OEM_4,	L"VK_LBRACKET",	L"\x005b");
	
	addNewDefine(pdVK_OEM_5,	L"VK_OEM_5",	L"\x005c");
	addNewDefine(pdVK_OEM_5,	L"VK_BACKSLASH",L"\x005c");
	
	addNewDefine(pdVK_OEM_6,	L"VK_OEM_6",	L"\x005d");
	addNewDefine(pdVK_OEM_6,	L"VK_RBRACKET",	L"\x005d");
	
	addNewDefine(pdVK_OEM_7,	L"VK_OEM_7",	L"\x0027");
	addNewDefine(pdVK_OEM_7,	L"VK_QUOTE",	L"\x0027");
	
	addNewDefine(pdVK_OEM_8,	L"VK_OEM_8",	L"\x00DF");
	addNewDefine(pdVK_OEM_8,	L"VK_EXCM",		L"\x00DF");
	
	addNewDefine(pdVK_OEM_AX,	L"VK_OEM_AX",	L"\x00E1");
	
	addNewDefine(pdVK_OEM_102,	L"VK_OEM_102",	L"\x002c");
	addNewDefine(pdVK_OEM_102,	L"VK_LESSTHEN",	L"\x002c");
	
	addNewDefine(pdVK_ICO_HELP,L"VK_ICO_HELP",	L"\x00E3");
	addNewDefine(pdVK_ICO_00,	L"VK_ICO_00",	L"\x00E4");
}

preDefined* getPredefined(key _key)
{
	for (int i = 0; i < [aryPreDefined count]; i++) {
		preDefined * _preDefined = [aryPreDefined objectAtIndex:i];
		if ([_preDefined Key] == _key)
			return _preDefined;
	}
	return NULL;
}

NSString* keyCodeToString(int keyCode, int carbonModFlags){
	
	//TISInputSourceRef tisSource = TISCopyCurrentKeyboardInputSource();
	TISInputSourceRef tisSource = TISCopyInputSourceForLanguage((CFStringRef)@"en-US");
	if(!tisSource) return '\0';
	
	CFDataRef layoutData;
	UInt32 keysDown = 0;
	layoutData = (CFDataRef)TISGetInputSourceProperty(tisSource, kTISPropertyUnicodeKeyLayoutData);
	if(!layoutData) return '\0';
	
	const UCKeyboardLayout *keyLayout = (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);
	
	UniCharCount length = 4, realLength;
	UniChar chars[4];
	
	OSStatus err = UCKeyTranslate( keyLayout, 
								  keyCode,
								  kUCKeyActionDisplay,
								  (carbonModFlags >> 8) & 0xFF,
								  LMGetKbdType(),
								  kUCKeyTranslateNoDeadKeysBit,
								  &keysDown,
								  length,
								  &realLength,
								  chars);
	
	if ( err != noErr ) return '\0';
	return [[NSString stringWithCharacters:chars length:realLength] autorelease];
}

char keyCodeToChar (int keyCode, int carbonModFlags)
{
	NSString* string = keyCodeToString(keyCode, carbonModFlags);
	const char * ch;
	ch = [string cStringUsingEncoding:NSMacOSRomanStringEncoding];
	return ch[0];
}

/* Returns key code for given character via the above function, or UINT16_MAX
 * on error. */
CGKeyCode charToKeyCode(const char c)
{
    static CFMutableDictionaryRef charToCodeDict = NULL;
    CGKeyCode code;
    UniChar character = c;
    CFStringRef charStr = NULL;
	
    /* Generate table of keycodes and characters. */
    if (charToCodeDict == NULL) {
        size_t i;
        charToCodeDict = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                   128,
                                                   &kCFCopyStringDictionaryKeyCallBacks,
                                                   NULL);
        if (charToCodeDict == NULL) return UINT16_MAX;
		
        /* Loop through every keycode (0 - 127) to find its current mapping. */
        for (i = 0; i < 128; ++i) {
            NSString* string = keyCodeToString((CGKeyCode)i,0);
            if (string != NULL) {
                CFDictionaryAddValue(charToCodeDict, string, (const void *)i);
                [string release];
            }
        }
    }
	
    charStr = CFStringCreateWithCharacters(kCFAllocatorDefault, &character, 1);
	
    /* Our values may be NULL (0), so we need to use this function. */
    if (!CFDictionaryGetValueIfPresent(charToCodeDict, charStr,
                                       (const void **)&code)) {
        code = UINT16_MAX;
    }
	
    CFRelease(charStr);
    return code;
}

NSUInteger CocoaToCarbonFlags( NSUInteger cocoaFlags )
{
	NSUInteger carbonFlags = 0;
	
	if (cocoaFlags & NSCommandKeyMask) carbonFlags |= cmdKey;
	if (cocoaFlags & NSAlternateKeyMask) carbonFlags |= optionKey;
	if (cocoaFlags & NSControlKeyMask) carbonFlags |= controlKey;
	if (cocoaFlags & NSShiftKeyMask) carbonFlags |= shiftKey;
	if (cocoaFlags & NSAlphaShiftKeyMask) carbonFlags |= alphaLock;
	if (cocoaFlags & NSFunctionKeyMask) carbonFlags |= NSFunctionKeyMask;
	
	return carbonFlags;
}

@implementation misc


@end
