//
// KeymagicIMEController.m
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

#import "KeymagicIMEController.h"

#import "keyboard.h"
#import "misc.h"

@implementation KeyMagicIMEController

- (id)initWithServer:(IMKServer*)server delegate:(id)delegate client:(id)inputClient
{
	if (self = [super initWithServer:server delegate:delegate client:inputClient]) {
        InputProcessor = [inputProcessor new];		
		configDictionary = [NSMutableDictionary new];
		
		[InputProcessor setEnable:NO];
		ActivePath = @"";
		
		[self LoadConfigurationFile];
		NSString *title = [configDictionary objectForKey:@"DefaultKeyboardTitle"];
		NSString *path = [configDictionary objectForKey:@"DefaultKeyboardPath"];
		if (title && path) {
			ActivePath = path;
			keyboard *Keyboard = [keyboard new];
			[Keyboard setTitle:title];
			[Keyboard setPath:path];
			[InputProcessor loadKeyboard:Keyboard];
		}
	}

	return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)activateServer:(id)sender
{
}

- (void)deactivateServer:(id)sender
{
}

- (void)commitComposition:(id)sender 
{    
	NSString * _composingBuffer = [[InputProcessor InternalEditor] getWholeString];
    [sender insertText:_composingBuffer replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
	[[InputProcessor InternalEditor] reset];
}

- (BOOL)handleEvent:(NSEvent*)event client:(id)sender
{    
    if ([event type] != NSKeyDown) {
		return NO;
	}
	
    BOOL handled = NO;	
	NSString *chars = [event characters];
	unsigned int cocoaModifiers = [event modifierFlags];
	unsigned short virtualKeyCode = [event keyCode];

	UniChar unicharCode = [chars characterAtIndex:0];
	
	//NSLog(@"cocoaModifiers=%d, virtualKeyCode=%d, unicharCode=%d, chars=%@", cocoaModifiers, virtualKeyCode, unicharCode, chars);
	
	char vk = keyCodeToChar(virtualKeyCode, 0);
	
	//NSLog(@"%c=%d", vk, virtualKeyCode, unicharCode);
	
	if (vk >= 'a' && vk <= 'z') {
		vk += 'A' - 'a';
	}
	
	[InputProcessor SetCarbonModifierFlags:CocoaToCarbonFlags(cocoaModifiers)];
	
	if (virtualKeyCode >= 123 && virtualKeyCode <= 126) {
		return NO;
	}
	
	else if ([InputProcessor processInput:vk character:unicharCode]) {
		//NSLog(@"processInput:return=YES,markedText=%@",[InputProcessor.m_internalEditor getWholeString]);
		handled = YES;
		//NSLog(@"processInput:YES");
	}
	else {
		//NSLog(@"processInput:NO");
		int i;
		//NSLog(@"processInput:return=NO");
		switch (unicharCode) {
			case 32: // Space
				if ([[InputProcessor InternalEditor] getLength]) {
					[self commitComposition:sender];
					return YES;
				}
				break;
			case 13: // Enter,Return
				[[InputProcessor InternalEditor] delete:1];
				if ([[InputProcessor InternalEditor] getLength]) {
					[self commitComposition:sender];
					return YES;
				}
				return NO;
			case 8: // Backspace
				if ([[InputProcessor InternalEditor] getLength]) {
					[[InputProcessor InternalEditor] delete:1];
					handled = YES;
				}
				break;
				
			default:
				handled = NO;
				break;
		}
	}
	
	//NSLog(@"%@ %d", [[InputProcessor InternalEditor] getWholeString], [[InputProcessor InternalEditor] getLength]);
	
	NSString * _composingBuffer = [[InputProcessor InternalEditor] getWholeString];
	

	NSMutableAttributedString *attrString = [[[NSMutableAttributedString alloc] initWithString:_composingBuffer attributes:[NSDictionary dictionary]] autorelease];

    #if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
		NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
								  [NSNumber numberWithInt:0], NSKernAttributeName,
								  [NSNumber numberWithInt:NSUnderlineStyleSingle], NSUnderlineStyleAttributeName,
								  [NSNumber numberWithInt:0], NSMarkedClauseSegmentAttributeName, nil];
    #else
		NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
								  [NSNumber numberWithInt:NSUnderlineStyleSingle], @"UnderlineStyleAttribute",
								  [NSNumber numberWithInt:0], @"MarkedClauseSegmentAttribute", nil];
    #endif		

	[attrString setAttributes:attrDict range:NSMakeRange(0, [_composingBuffer length])];  
	
	// selectionRange means "cursor position index"
	NSRange selectionRange = NSMakeRange([_composingBuffer length], 0); 
	[sender setMarkedText:attrString selectionRange:selectionRange replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
	
	/*
    // update caret position
    NSPoint caretPosition;
    NSRect lineHeightRect;
    [sender attributesForCharacterIndex:0 lineHeightRectangle:&lineHeightRect];
    caretPosition = [self _fixCaretPosition:lineHeightRect.origin];          
    */
	
    return handled;
}

- (void)_aboutAction:(id)sender
{
	[NSApp orderFrontStandardAboutPanel:sender];
}

-(void) selectionChanged:(id)sender {
	NSMenuItem *menuItem = [sender objectForKey:@"IMKCommandMenuItem"];
	keyboard * Keyboard = [menuItem representedObject];
	if (Keyboard.path != nil) {
		[InputProcessor loadKeyboard:Keyboard];
		[configDictionary setObject:[Keyboard title] forKey:@"DefaultKeyboardTitle"];
		[configDictionary setObject:[Keyboard path] forKey:@"DefaultKeyboardPath"];
		ActivePath = [Keyboard path];
		[self WriteConfigurationFile];
	} else {
		[InputProcessor setEnable:NO];
	}

}

- (NSMenu *)menu
{
	NSMenu *menu = [[NSMenu new] autorelease];
	
	NSBundle * mainBundle = [NSBundle mainBundle];
	NSArray * paths = [mainBundle pathsForResourcesOfType:@"km2" inDirectory:nil];
	NSEnumerator *e = [paths objectEnumerator];
	NSString *path;
	while (path = [e nextObject]) {
		NSMenuItem * menuItem = [NSMenuItem new];
		NSString * fileName = [path lastPathComponent];
		NSString * fileTitle = [fileName substringToIndex:[fileName length] - 4];
		[menuItem setTitle:fileTitle];
		[menuItem setTarget:self];
		[menuItem setAction:@selector(selectionChanged:)];
		
		keyboard * Keyboard;
		Keyboard = [keyboard new];
		[Keyboard setTitle:fileTitle];
		[Keyboard setPath:path];
		[Keyboard setLFR:[layoutFileReader new]];
		[[Keyboard LFR] readFromFile:path];
		
		[menuItem setRepresentedObject:Keyboard];
		
		if ([[Keyboard path] compare:ActivePath] == NSOrderedSame) {
			[menuItem setState:NSOnState];
		}
		
		[menu addItem:menuItem];
		
		[Keyboards addObject:Keyboard];
	}
	
	[menu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem *menuItem = [[NSMenuItem new] autorelease];
	[menuItem setTarget:self];
	[menuItem setAction:@selector(_aboutAction:)];
	[menuItem setTitle:@"About KeyMagic"];
	[menu addItem:menuItem];
	return menu;
}

-(NSString*) configFilePath {
	NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	NSString *prefPath = [[dirs objectAtIndex:0] stringByAppendingPathComponent:@"Preferences"];	
	return [prefPath stringByAppendingPathComponent:@"org.keymagic.plist"];
}

- (void) WriteConfigurationFile {
	NSData *data = [NSPropertyListSerialization dataFromPropertyList:configDictionary format:NSPropertyListXMLFormat_v1_0 errorDescription:nil];
	if (data) {
		[data writeToFile:[self configFilePath] atomically:YES];
	}
}

-(void) LoadConfigurationFile {
	NSData *data = [NSData dataWithContentsOfFile:[self configFilePath]];
	if (data) {
		NSPropertyListFormat format;
		id plist = [NSPropertyListSerialization propertyListFromData:data mutabilityOption:NSPropertyListMutableContainersAndLeaves format:&format errorDescription:nil];
		if ([plist isKindOfClass:[NSDictionary class]]) {
			[configDictionary removeAllObjects];
			[configDictionary addEntriesFromDictionary:plist];
		}		
	}
}

@end
