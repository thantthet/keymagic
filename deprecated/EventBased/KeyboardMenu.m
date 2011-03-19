//
//  KeyboardMenu.m
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 4/27/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import "KeyboardMenu.h"


@implementation KeyboardMenu

-(id) initWithHandler:(EventHandler*)_handler {
	eHandler = [_handler retain];
	return [super init];
}

-(IBAction) selectionChanged:(id)sender {
	[eHandler loadKeyboard:[sender representedObject]];
	NSString * title = [sender title];
	[statusItem setTitle:title];
	[eHandler setEnable:YES];
}

-(IBAction) turnOffKeyboard:(id)sender {
	[eHandler setEnable:NO];
	NSString * title = [sender title];
	[statusItem setTitle:title];
}

-(IBAction) quit:(id)sender {
	[NSApp terminate:self];
}

-(void) menuFromKeyboardFiles {
	menu = [NSMenu new];
	
	NSBundle * mainBundle = [NSBundle mainBundle];
	NSArray * paths = [mainBundle pathsForResourcesOfType:@"km2" inDirectory:nil];
	for (NSString * path in paths) {
		NSMenuItem * menuItem = [NSMenuItem new];
		NSString * fileName = [path lastPathComponent];
		NSString * fileTitle = [fileName substringToIndex:[fileName length] - 4];
		[menuItem setTitle:fileTitle];
		[menuItem setTarget:self];
		[menuItem setAction:@selector(selectionChanged:)];
		[menuItem setRepresentedObject:path];
		[menu addItem:menuItem];
	}
	
	[menu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem * offMenuItem = [NSMenuItem new];
	[offMenuItem setTitle:@"Disable"];
	[offMenuItem setTarget:self];
	[offMenuItem setAction:@selector(turnOffKeyboard:)];
	[menu addItem:offMenuItem];
	
	[menu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem * quitItem = [NSMenuItem new];
	[quitItem setTitle:@"Quit"];
	[quitItem setTarget:self];
	[quitItem setAction:@selector(quit:)];
	[menu addItem:quitItem];
	
	statusItem =  [[[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength] retain];
	[statusItem setHighlightMode:YES];
	[statusItem setEnabled:YES];
	
	[statusItem setTitle:@"Disable"];
	[statusItem setImage:[NSImage imageNamed:@"tray.png"]];
	[statusItem setMenu:menu];
}

@end
