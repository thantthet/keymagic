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

#import <Carbon/Carbon.h>
#import "KeymagicIMEController.h"
#import "keyboard.h"
#include "KeyMagicLogger.h"

#define RETURNVAL(b, c) \
case b: \
*winVK = c;\
return TRUE

@implementation KeyMagicIMEController

bool mapVK(int virtualkey, int * winVK)
{
	switch (virtualkey) {
		RETURNVAL(kVK_ANSI_A, 'A');
		RETURNVAL(kVK_ANSI_B, 'B');
		RETURNVAL(kVK_ANSI_C, 'C');
		RETURNVAL(kVK_ANSI_D, 'D');
		RETURNVAL(kVK_ANSI_E, 'E');
		RETURNVAL(kVK_ANSI_F, 'F');
		RETURNVAL(kVK_ANSI_G, 'G');
		RETURNVAL(kVK_ANSI_H, 'H');
		RETURNVAL(kVK_ANSI_I, 'I');
		RETURNVAL(kVK_ANSI_J, 'J');
		RETURNVAL(kVK_ANSI_K, 'K');
		RETURNVAL(kVK_ANSI_L, 'L');
		RETURNVAL(kVK_ANSI_M, 'M');
		RETURNVAL(kVK_ANSI_N, 'N');
		RETURNVAL(kVK_ANSI_O, 'O');
		RETURNVAL(kVK_ANSI_P, 'P');
		RETURNVAL(kVK_ANSI_Q, 'Q');
		RETURNVAL(kVK_ANSI_R, 'R');
		RETURNVAL(kVK_ANSI_S, 'S');
		RETURNVAL(kVK_ANSI_T, 'T');
		RETURNVAL(kVK_ANSI_U, 'U');
		RETURNVAL(kVK_ANSI_V, 'V');
		RETURNVAL(kVK_ANSI_W, 'W');
		RETURNVAL(kVK_ANSI_X, 'X');
		RETURNVAL(kVK_ANSI_Y, 'Y');
		RETURNVAL(kVK_ANSI_Z, 'Z');
		RETURNVAL(kVK_ANSI_1, '1');
		RETURNVAL(kVK_ANSI_2, '2');
		RETURNVAL(kVK_ANSI_3, '3');
		RETURNVAL(kVK_ANSI_4, '4');
		RETURNVAL(kVK_ANSI_5, '5');
		RETURNVAL(kVK_ANSI_6, '6');
		RETURNVAL(kVK_ANSI_7, '7');
		RETURNVAL(kVK_ANSI_8, '8');
		RETURNVAL(kVK_ANSI_9, '9');
		RETURNVAL(kVK_ANSI_Equal, 0xBC);
		RETURNVAL(kVK_ANSI_Minus, 0xBD);
		RETURNVAL(kVK_ANSI_LeftBracket, 0xDB);
		RETURNVAL(kVK_ANSI_RightBracket, 0xDD);
		RETURNVAL(kVK_ANSI_Quote, 0xDE);
		RETURNVAL(kVK_ANSI_Semicolon, 0xBA);
		RETURNVAL(kVK_ANSI_Backslash, 0xDC);
		RETURNVAL(kVK_ANSI_Comma, 0xBC);
		RETURNVAL(kVK_ANSI_Slash, 0xBF);
		RETURNVAL(kVK_ANSI_Period, 0xBE);
		RETURNVAL(kVK_ANSI_Grave, 0xC0);
		RETURNVAL(kVK_ANSI_KeypadDecimal, 0x6E);
		RETURNVAL(kVK_ANSI_KeypadMultiply, 0x6A);
		RETURNVAL(kVK_ANSI_KeypadPlus, 0x6B);
		RETURNVAL(kVK_ANSI_KeypadDivide, 0x6F);
		RETURNVAL(kVK_ANSI_KeypadMinus, 0x6D);
		RETURNVAL(kVK_ANSI_Keypad0, 0x60);
		RETURNVAL(kVK_ANSI_Keypad1, 0x61);
		RETURNVAL(kVK_ANSI_Keypad2, 0x62);
		RETURNVAL(kVK_ANSI_Keypad3, 0x63);
		RETURNVAL(kVK_ANSI_Keypad4, 0x64);
		RETURNVAL(kVK_ANSI_Keypad5, 0x65);
		RETURNVAL(kVK_ANSI_Keypad6, 0x66);
		RETURNVAL(kVK_ANSI_Keypad7, 0x67);
		RETURNVAL(kVK_ANSI_Keypad8, 0x68);
		RETURNVAL(kVK_ANSI_Keypad9, 0x69);
		RETURNVAL(kVK_Return, 0x0D);
		RETURNVAL(kVK_Tab, 0x09);
		RETURNVAL(kVK_Space, 0x20);
		RETURNVAL(kVK_Delete, 0x08);
		RETURNVAL(kVK_ForwardDelete, 0x2E);
		RETURNVAL(kVK_Escape, 0x1B);
		RETURNVAL(kVK_Shift, 0x10);
		RETURNVAL(kVK_CapsLock, 0x14);
		RETURNVAL(kVK_Option, 0x12);
		RETURNVAL(kVK_Control, 0x11);
		RETURNVAL(kVK_RightShift, 0xA1);
		RETURNVAL(kVK_RightOption, 0xA5);
		RETURNVAL(kVK_RightControl, 0xA3);
		default:
			break;
	}
	return FALSE;
}

- (id)initWithServer:(IMKServer*)server delegate:(id)delegate client:(id)inputClient
{
	m_logFile = fopen("/Users/thantthetkz/Desktop/km.txt", "w");
	KeyMagicLogger * logger = KeyMagicLogger::getInstance();
	if (m_logFile != 0) logger->setFile(m_logFile);
	
	if ([super initWithServer:server delegate:delegate client:inputClient] == self) {	
		configDictionary = [NSMutableDictionary new];
		ActivePath = @"";
		
		[self LoadConfigurationFile];
		m_success = FALSE;
		NSString *title = [configDictionary objectForKey:@"DefaultKeyboardTitle"];
		NSString *path = [configDictionary objectForKey:@"DefaultKeyboardPath"];
		if (title && path) {
			ActivePath = path;
			m_success = kme.loadKeyboardFile([ActivePath cStringUsingEncoding:NSUTF8StringEncoding]);
			keyboard *Keyboard = [keyboard new];
			[Keyboard setTitle:title];
			[Keyboard setPath:path];
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
	NSString * _composingBuffer = [NSString stringWithKeyMagicString:kme.getContextText()];
	if ([_composingBuffer length]) {
		[sender insertText:_composingBuffer replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
		kme.reset();
	}
}

- (BOOL)handleEvent:(NSEvent*)event client:(id)sender
{    
    if ([event type] != NSKeyDown && m_success) {
		return NO;
	}

    BOOL handled = NO;
	
	NSString *chars = [event characters];
	unsigned int cocoaModifiers = [event modifierFlags];
	unsigned short virtualKeyCode = [event keyCode];
	
	switch (virtualKeyCode) {
		case kVK_LeftArrow:
		case kVK_UpArrow:
		case kVK_RightArrow:
		case kVK_DownArrow:
			
		case kVK_Home:
		case kVK_End:
			
		case kVK_PageUp:
		case kVK_PageDown:
			[self commitComposition:sender];
			return NO;
	}
	
	int modifier = 0;
	if (cocoaModifiers & NSAlphaShiftKeyMask) {
		modifier |= KeyMagicEngine::CAPS_MASK;
	}
	if (cocoaModifiers & NSShiftKeyMask) {
		modifier |= KeyMagicEngine::SHIFT_MASK;
	}
	if (cocoaModifiers & NSControlKeyMask) {
		modifier |= KeyMagicEngine::CTRL_MASK;
	}
	if (cocoaModifiers & NSAlternateKeyMask) {
		modifier |= KeyMagicEngine::ALT_MASK;
	}
	
	int winVK;
	if (mapVK(virtualKeyCode, &winVK) == NO) {
		return NO;
	}
	
	if (kme.processKeyEvent([chars characterAtIndex:0], winVK, modifier) == NO) {
		switch (virtualKeyCode) {
			case kVK_Space:
			case kVK_Return:
				[self commitComposition:sender];
				break;
		}
		
		if (cocoaModifiers & NSControlKeyMask) {
			[self commitComposition:sender];
		} else if (cocoaModifiers & NSAlternateKeyMask) {
			[self commitComposition:sender];
		} else if (cocoaModifiers & NSCommandKeyMask) {
			[self commitComposition:sender];
		}
		
		return NO;
	}
	
	NSString * _composingBuffer = [NSString stringWithKeyMagicString:kme.getContextText()];
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
	
    return YES;
}

- (void)_aboutAction:(id)sender
{
	[NSApp orderFrontStandardAboutPanel:sender];
}

-(void) selectionChanged:(id)sender {
	NSMenuItem *menuItem = [sender objectForKey:@"IMKCommandMenuItem"];
	keyboard * Keyboard = [menuItem representedObject];
	if (Keyboard.path != nil) {
		kme.loadKeyboardFile([Keyboard.path cStringUsingEncoding:NSUTF8StringEncoding]);
		[configDictionary setObject:[Keyboard title] forKey:@"DefaultKeyboardTitle"];
		[configDictionary setObject:[Keyboard path] forKey:@"DefaultKeyboardPath"];
		ActivePath = [Keyboard path];
		[self WriteConfigurationFile];
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
