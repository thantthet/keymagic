//
//  preDefined.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/10/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef enum _key {
	pdNULL = 1,
	
	pdVK_BACK,
	pdVK_TAB,
	pdVK_RETURN,
	pdVK_SHIFT,
	pdVK_CONTROL,
	pdVK_MENU,
	pdVK_PAUSE,
	pdVK_CAPITAL,
	pdVK_KANJI,
	pdVK_ESCAPE,
	pdVK_SPACE,
	pdVK_PRIOR,
	pdVK_NEXT,
	pdVK_DELETE,
	
	pdVK_KEY_0,
	pdVK_KEY_1,
	pdVK_KEY_2,
	pdVK_KEY_3,
	pdVK_KEY_4,
	pdVK_KEY_5,
	pdVK_KEY_6,
	pdVK_KEY_7,
	pdVK_KEY_8,
	pdVK_KEY_9,
	
	pdVK_KEY_A,
	pdVK_KEY_B,
	pdVK_KEY_C,
	pdVK_KEY_D,
	pdVK_KEY_E,
	pdVK_KEY_F,
	pdVK_KEY_G,
	pdVK_KEY_H,
	pdVK_KEY_I,
	pdVK_KEY_J,
	pdVK_KEY_K,
	pdVK_KEY_L,
	pdVK_KEY_M,
	pdVK_KEY_N,
	pdVK_KEY_O,
	pdVK_KEY_P,
	pdVK_KEY_Q,
	pdVK_KEY_R,
	pdVK_KEY_S,
	pdVK_KEY_T,
	pdVK_KEY_U,
	pdVK_KEY_V,
	pdVK_KEY_W,
	pdVK_KEY_X,
	pdVK_KEY_Y,
	pdVK_KEY_Z,
	
	pdVK_NUMPAD0,
	pdVK_NUMPAD1,
	pdVK_NUMPAD2,
	pdVK_NUMPAD3,
	pdVK_NUMPAD4,
	pdVK_NUMPAD5,
	pdVK_NUMPAD6,
	pdVK_NUMPAD7,
	pdVK_NUMPAD8,
	pdVK_NUMPAD9,
	
	pdVK_MULTIPLY,
	pdVK_ADD,
	pdVK_SEPARATOR,
	pdVK_SUBTRACT,
	pdVK_DECIMAL,
	pdVK_DIVIDE,
	
	pdVK_F1,
	pdVK_F2,
	pdVK_F3,
	pdVK_F4,
	pdVK_F5,
	pdVK_F6,
	pdVK_F7,
	pdVK_F8,
	pdVK_F9,
	pdVK_F10,
	pdVK_F11,
	pdVK_F12,
	
	pdVK_LSHIFT,
	pdVK_RSHIFT,
	
	pdVK_LCONTROL,
	pdVK_RCONTROL,
	pdVK_LMENU,
	pdVK_RMENU,
	
	pdVK_OEM_1,
	pdVK_OEM_PLUS,
	pdVK_OEM_COMMA,
	pdVK_OEM_MINUS,
	pdVK_OEM_PERIOD,
	pdVK_OEM_2,
	pdVK_OEM_3,
	
	pdVK_OEM_4,
	pdVK_OEM_5,
	pdVK_OEM_6,
	pdVK_OEM_7,
	pdVK_OEM_8,
	
	pdVK_OEM_AX,
	pdVK_OEM_102,
	pdVK_ICO_HELP,
	pdVK_ICO_00,
} key;


@interface preDefined : NSObject
{
	@public
	key key;
	const wchar_t * name;
	const wchar_t * value;
}

@property(assign) key key;
@property(assign) const wchar_t *name;
@property(assign) const wchar_t *value;

@end