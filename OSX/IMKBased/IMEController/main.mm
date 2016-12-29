//
// main.m
//
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

#import <InputMethodKit/InputMethodKit.h>
#import "KeyMagicIMEController.h"

#include "KeyMagicEngine.h"
#include "KeyMagicLogger.h"
#include "KeyMagicString.h"

IMKServer *KeyMagicInputMethodServer = nil;

int main(int argc, char *argv[])
{
    @autoreleasepool {
		KeyMagicInputMethodServer = [[IMKServer alloc] initWithName:@"KeyMagic_1_Connection"
                                                   bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
        if (!KeyMagicInputMethodServer) {
			NSLog(@"input method server init failed!");
            return 1;
        }
		[[NSBundle mainBundle] loadNibNamed:@"MainMenu" owner:[NSApplication sharedApplication] topLevelObjects:NULL];
        
		[[NSApplication sharedApplication] run];
	}	
	return 0;
}