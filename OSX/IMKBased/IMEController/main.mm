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
#import <Growl/Growl.h>

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
	#import <InputMethodKit/InputMethodKit.h>
#else
	#import "InputMethodKitTiger.h"
#endif
#import "KeyMagicIMEController.h"
#import "Growl.h"

#include "KeyMagicEngine.h"
#include "KeyMagicLogger.h"
#include "KeyMagicString.h"

IMKServer *KeyMagicInputMethodServer = nil;

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
	
	Growl * growl = [[Growl alloc] init];
	
	KeyMagicInputMethodServer = [[IMKServer alloc] initWithName:@"KeyMagic_1_Connection" bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];	
    if (!KeyMagicInputMethodServer) {
		NSLog(@"input method server init failed!");
        return 1;
    }	
	[NSBundle loadNibNamed:@"MainMenu" owner:[NSApplication sharedApplication]];	

	[[NSApplication sharedApplication] run];
	[pool drain];	
	return 0;
}