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
#import	"keymagic.h"

@implementation KeyMagicIMEController

#define RETURNVAL(b, c) \
case b: \
*winVK = c;\
return TRUE
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
		RETURNVAL(kVK_ANSI_0, '0');
		RETURNVAL(kVK_ANSI_1, '1');
		RETURNVAL(kVK_ANSI_2, '2');
		RETURNVAL(kVK_ANSI_3, '3');
		RETURNVAL(kVK_ANSI_4, '4');
		RETURNVAL(kVK_ANSI_5, '5');
		RETURNVAL(kVK_ANSI_6, '6');
		RETURNVAL(kVK_ANSI_7, '7');
		RETURNVAL(kVK_ANSI_8, '8');
		RETURNVAL(kVK_ANSI_9, '9');
		RETURNVAL(kVK_ANSI_Equal, VK_OEM_PLUS);
		RETURNVAL(kVK_ANSI_Minus, VK_OEM_MINUS);
		RETURNVAL(kVK_ANSI_LeftBracket, VK_OEM_4);
		RETURNVAL(kVK_ANSI_RightBracket, VK_OEM_6);
		RETURNVAL(kVK_ANSI_Quote, VK_OEM_7);
		RETURNVAL(kVK_ANSI_Semicolon, VK_OEM_1);
		RETURNVAL(kVK_ANSI_Backslash, VK_OEM_5);
		RETURNVAL(kVK_ANSI_Comma, VK_OEM_COMMA);
		RETURNVAL(kVK_ANSI_Slash, VK_OEM_2);
		RETURNVAL(kVK_ANSI_Period, VK_OEM_PERIOD);
		RETURNVAL(kVK_ANSI_Grave, VK_OEM_3);
		RETURNVAL(kVK_ANSI_KeypadDecimal, VK_DECIMAL);
		RETURNVAL(kVK_ANSI_KeypadMultiply, VK_MULTIPLY);
		RETURNVAL(kVK_ANSI_KeypadPlus, VK_ADD);
		RETURNVAL(kVK_ANSI_KeypadDivide, VK_DIVIDE);
		RETURNVAL(kVK_ANSI_KeypadMinus, VK_SUBTRACT);
		RETURNVAL(kVK_ANSI_Keypad0, VK_NUMPAD0);
		RETURNVAL(kVK_ANSI_Keypad1, VK_NUMPAD1);
		RETURNVAL(kVK_ANSI_Keypad2, VK_NUMPAD2);
		RETURNVAL(kVK_ANSI_Keypad3, VK_NUMPAD3);
		RETURNVAL(kVK_ANSI_Keypad4, VK_NUMPAD4);
		RETURNVAL(kVK_ANSI_Keypad5, VK_NUMPAD5);
		RETURNVAL(kVK_ANSI_Keypad6, VK_NUMPAD6);
		RETURNVAL(kVK_ANSI_Keypad7, VK_NUMPAD7);
		RETURNVAL(kVK_ANSI_Keypad8, VK_NUMPAD8);
		RETURNVAL(kVK_ANSI_Keypad9, VK_NUMPAD9);
		RETURNVAL(kVK_Return, VK_RETURN);
		RETURNVAL(kVK_Tab, VK_TAB);
		RETURNVAL(kVK_Space, VK_SPACE);
		RETURNVAL(kVK_Delete, VK_BACK);
		RETURNVAL(kVK_ForwardDelete, VK_DELETE);
		RETURNVAL(kVK_Escape, VK_ESCAPE);
		RETURNVAL(kVK_Shift, VK_SHIFT);
		RETURNVAL(kVK_CapsLock, VK_CAPITAL);
		RETURNVAL(kVK_Option, VK_MENU);
		RETURNVAL(kVK_Control, VK_CONTROL);
		RETURNVAL(kVK_RightShift, VK_RSHIFT);
		RETURNVAL(kVK_RightOption, VK_RMENU);
		RETURNVAL(kVK_RightControl, VK_RCONTROL);
		default:
			break;
	}
	return FALSE;
}

#undef RETURNVAL

- (id)initWithServer:(IMKServer*)server delegate:(id)delegate client:(id)inputClient
{
	
	m_logFile = fopen("/Users/thantthetkz/Library/Logs/KeyMagic.log", "w");
	logger = KeyMagicLogger::getInstance();
	if (m_logFile != 0) logger->setFile(m_logFile);
	
	//kme.m_verbose = true;
	//kme.getKeyboard()->m_verbose = true;
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

/*- (NSAttributedString*) lastAttributedString:(NSAttributedString *)attString
{
	NSUInteger length = [[attString string] length];
	
	if (length <= 0) {
		return NULL;
	}
	
	NSRange effectiveRange;

	NSDictionary * lastAttDict = [attString attributesAtIndex:length-1 effectiveRange:&effectiveRange];
	return [attString attributedSubstringFromRange:effectiveRange];
}

- (NSDictionary*) lastAttribute:(NSAttributedString *)attString
{
	NSUInteger length = [[attString string] length];
	
	if (length <= 0) {
		return NULL;
	}
	
	NSRange effectiveRange;
	
	NSDictionary * lastAttDict = [attString attributesAtIndex:length-1 effectiveRange:&effectiveRange];
	return lastAttDict;
}*/

- (BOOL)handleEvent:(NSEvent*)event client:(id)sender
{
	/*BOOL commit = NO;
	NSRange replacementRange = NSMakeRange(NSNotFound, NSNotFound);*/
    if ([event type] != NSKeyDown || m_success == NO) {
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
	if (cocoaModifiers & NSCommandKeyMask) {
		return NO;
	}
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

	/*if (kme.getContextText()->length() == 0 && virtualKeyCode == kVK_Delete) {
		NSRange range = [sender selectedRange];
		if (range.location != NSNotFound && range.length != NSNotFound) {
			NSInteger location, length;
			
			location = range.location < 50 ? 0 : range.location - 50;
			length = location == 0 ? range.location : 50;
			
			NSAttributedString * attString = [sender attributedSubstringFromRange:NSMakeRange(location, length)];
			if (attString != NULL) {
				attString = [self lastAttributedString:attString];
				KeyMagicString kms = [[attString string] getKeyMagicString];
				kme.setContextText(&kms);
				replacementRange = NSMakeRange(range.location - attString.string.length, attString.string.length);
				commit = YES;
			}
		}
	}*/
	
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
	
	/*if (commit) {
		[self commitComposition:sender];
	}*/
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
		if (m_success = kme.loadKeyboardFile([Keyboard.path cStringUsingEncoding:NSUTF8StringEncoding])) {
			[configDictionary setObject:[Keyboard title] forKey:@"DefaultKeyboardTitle"];
			[configDictionary setObject:[Keyboard path] forKey:@"DefaultKeyboardPath"];
			ActivePath = [Keyboard path];
			[self WriteConfigurationFile];
		}
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
