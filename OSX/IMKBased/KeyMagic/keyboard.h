//
//  keyboard.h
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

#import <Cocoa/Cocoa.h>
#import "layoutFileReader.h"

struct Hotkey {
	int modkey;
	int vkey;
};

typedef struct Hotkey Hotkey;

@interface keyboard : NSObject {
	@public
	NSString * title;
	NSString * path;
	layoutFileReader * LFR;
	Hotkey hotkey;
}

//@property(assign) NSString * title;
-(NSString*) title;
-(void) setTitle:(NSString*)_title;
-(NSString*) path;
-(void) setPath:(NSString*)_path;
-(layoutFileReader*) LFR;
-(void) setLFR:(layoutFileReader*)_LFR;
-(Hotkey) hotkey;
-(void) setHotkey:(Hotkey)_hotkey;
@end
