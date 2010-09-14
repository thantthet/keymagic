//
//  misc.h
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

#ifndef _MISC_H
#define _MISC_H

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>

#import "preDefined.h"

extern NSMutableArray * aryPreDefined;

size_t _wcslen(const short* ws);

void initPreDefined();
preDefined* getPredefined(key _key);
void addNewDefine(key _key, const wchar_t * _name, const wchar_t * _value);

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