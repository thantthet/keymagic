//
//  misc.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/8/10.
//  Copyright 2010 N/A. All rights reserved.
//

#ifndef _MISC_H
#define _MISC_H

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>

#import "preDefined.h"

extern NSMutableArray * aryPreDefined;

size_t _wcslen(const short* ws);

void initPreDefined();
preDefined* getPredefined(key _key);
void newPreDefined(key _key, const wchar_t * _name, const wchar_t * _value);

CGKeyCode charToKeyCode(const char c);
char keyCodeToChar (int keyCode, int carbonModFlags);
NSString* keyCodeToString(int keyCode, int carbonModFlags);

NSUInteger CocoaToCarbonFlags( NSUInteger cocoaFlags );

void KeyboardSetUnicodeString(CGEventRef event, NSString * string);
void sendString(CGEventTapProxy proxy, CGEventRef event, NSString * string);

@interface misc : NSObject {
	
}

@end

#endif