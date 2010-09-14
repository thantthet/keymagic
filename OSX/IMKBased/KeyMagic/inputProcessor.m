//
//  inputProcessor.m
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

#import "inputProcessor.h"
#import "misc.h"
#import "charClass.h"
#import "RegexKitLite.h"


@implementation inputProcessor

-(CFMachPortRef) KeyboardMachPort {
	return keyboardMachPort;
}
-(void) SetKeyboardMachPort:(CFMachPortRef)MachPort {
	keyboardMachPort = MachPort;
}
-(CFMachPortRef) MouseMachPort {
	return mouseMachPort;
}
-(void) SetMouseMachPort:(CFMachPortRef)MachPort {
	mouseMachPort = MachPort;
}
-(NSUInteger) CarbonModifierFlags {
	return carbonModFlags;
}
-(void) SetCarbonModifierFlags:(NSUInteger)Flags {
	carbonModFlags = 0;
	
	if (Flags & shiftKey) carbonModFlags |= shiftKey;
	if (Flags & controlKey) carbonModFlags |= controlKey;
	if (Flags & optionKey) carbonModFlags |= optionKey;
	if (Flags & cmdKey) carbonModFlags |= cmdKey;
	
	if (Flags & alphaLock && [m_LFR FileHeader].layout.trackCaps == YES) {
		carbonModFlags |= shiftKey;
	}
}
-(layoutFileReader*) LayoutFileReader {
	return m_LFR;
}
-(void) SetLayoutFileReader:(layoutFileReader*)LFR {
	m_LFR = LFR;
}
-(internalEditor*) InternalEditor {
	return m_internalEditor;
}
-(void) SetInternalEditor:(internalEditor*)Editor {
	m_internalEditor = Editor;
}

-init
{
	[super init];
	m_LFR = [layoutFileReader new];
	m_internalEditor = [internalEditor new];
	carbonModFlags = 0;
	
	initPreDefined();
	
	return self;
}

-(BOOL)loadKeyboard:(keyboard*)_keyboard {
	if ([m_LFR readFromFile:_keyboard.path] == NO) {
		[self setEnable:NO];
		return NO;
	}
	[self expendLHSPattern];
	[self setEnable:YES];
	return YES;
}

-(BOOL)Enable {
	return isEnable;
}

-(void)setEnable:(BOOL)flag {
	isEnable = flag;
	[m_internalEditor reset];
}

void KeyboardSetUnicodeString(CGEventRef event, NSString * string)
{
	NSUInteger length = [string length];
	CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 1);
	CGEventSetIntegerValueField(event, kCGKeyboardEventAutorepeat, NO);
	CGEventKeyboardSetUnicodeString(event, length, (const UniCharPtr)[string cStringUsingEncoding:NSUnicodeStringEncoding]);
}

void sendString(CGEventTapProxy proxy, CGEventRef event, NSString * string)
{
	KeyboardSetUnicodeString(event, string);
	//Post Event
	CGEventTapPostEvent(proxy, event);
}

NSInteger compareRules (id arg1, id arg2, void *arg3)
{
	ruleInfoLHS *ruleInfo1 = arg1, *ruleInfo2 = arg2;
	NSUInteger rule1Length = 0, rule2Length = 0;
	
	/* compare switches count */
	rule1Length = [ruleInfo1 Switches].count;
	rule2Length = [ruleInfo2 Switches].count;
	
	
	if (rule1Length != rule2Length) {
		return rule2Length - rule1Length;
	}
	
	/* if switches count are the same, compare vkeys count */
	rule1Length = [ruleInfo1 VKeys].count;
	rule2Length = [ruleInfo2 VKeys].count;
	
	if (rule1Length != rule2Length) {
		return rule2Length - rule1Length;
	}
	
	/* else, compare strLengthToMatch */
	rule1Length += [ruleInfo1 LengthOfStringToMatch];
	rule2Length += [ruleInfo2 LengthOfStringToMatch];
	
	if (rule1Length != rule2Length) {
		return rule2Length - rule1Length;
	}
	
	/* return 0, the same */
	return 0;
}

-(void) expendLHSPattern
{	
	NSMutableArray* strings = [m_LFR Strings];
	NSMutableArray* rules = [m_LFR Rules];
	
	expendedRules = [NSMutableArray new];
	NSEnumerator *e = [rules objectEnumerator];
	classRule *rule;
	
	while (rule = [e nextObject]) {
		NSMutableDictionary * expendedRule = [NSMutableDictionary dictionaryWithCapacity:3];
		ruleInfoLHS * ruleInfo = nil;
		
		if ([self expendRule:expendedRule bRule:(short*)rule.ruleBinaryIn strings:strings doCapture:YES matched:nil charClasses:nil] == NO) {
			continue;
		}
		
		ruleInfo = [[ruleInfoLHS alloc] initWithRegex:[expendedRule objectForKey:@"regexString"]];
		[ruleInfo setIndex:[expendedRules count]];
		[ruleInfo setSwitches:[expendedRule objectForKey:@"switches"]];
		[ruleInfo setVKeys:[expendedRule objectForKey:@"virutalkeys"]];
		
		[expendedRules addObject:ruleInfo];
		//[expendedRule autorelease];
		//[ruleInfo autorelease];
	}
	
	[expendedRules sortUsingFunction:compareRules context:nil];

	/*e = [expendedRules objectEnumerator];
	ruleInfoLHS *ruleInfo;
	while (ruleInfo = [e nextObject]) {
		NSLog(@"index:%3d vkCount:%d strLengthToMatch:%d switchCount:%d",
			  [ruleInfo Index], [ruleInfo VKeys].count, [ruleInfo LengthOfStringToMatch], [ruleInfo Switches].count);
	}*/

}

-(NSString*) expendRHSPattern:(NSUInteger)ruleIndex matches:(NSArray*)matches charClasses:(NSArray*)charClasses
{	
	
	NSMutableArray* strings = [m_LFR Strings];
	NSMutableArray* rules = [m_LFR Rules];
	
	/* get rule from list of rules */
	classRule * rule = [rules objectAtIndex:ruleIndex];
	NSMutableDictionary * expendedRule = [NSMutableDictionary dictionaryWithCapacity:3];
	//[expendedRule autorelease];
	
	/* expendRule. expending output rule need matches and charClass form matched(input rule). No caputuring is required */
	if ([self expendRule:expendedRule bRule:(short*)rule.ruleBinaryOut strings:strings doCapture:NO matched:matches charClasses:charClasses] == NO) {
		return nil;
	}
	/* pick only regexString from expendedRule */
	NSString* regexString = [[expendedRule objectForKey:@"regexString"] retain];

	return regexString;
}

-(BOOL)processInput:(short)keycode character:(char)character
{
	NSString * mstring;
	NSUInteger toDelete;
	BOOL ret = NO;
	
	if (isEnable == NO) {
		return NO;
	}
	
	/* start rule matching */
	mstring = [self matchRules:keycode character:character toDelete:&toDelete];
		
	if (mstring) {
		ret = YES;
		/* Do we have string to match? */
		while ([mstring length]){
			// Prevent from ...
			if (mstring.length == 1 && [mstring characterAtIndex:0] > 0x20 && [mstring characterAtIndex:0] < 0x7F) {
				return YES;
			}
			[mstring release];
			/* Try rule matching again */
			mstring = [self matchRules:'\0' character:'\0' toDelete:&toDelete];
		}
	}
	
	return ret;
}

-(CGEventRef) handleEventKeyDown:(CGEventTapProxy)proxy type:(CGEventType)type event:(CGEventRef)event
{
	CGKeyCode keyCode;
	NSUInteger toDelete;
	NSString * mstring;
	CGEventRef eventToReturn = event;
	
	if (isEnable == NO) {
		return event;
	}
	
	/* get keycode from event */
	keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
	
	/* Is arrow key? */
	if (keyCode >= 0x7b && keyCode <= 0x7e) {
		
		/* reset the editor */
		[m_internalEditor reset];
		
		/* didnt eat, return original event */
		return event;
	}
	
	/* get char from keycode */
	char c = keyCodeToChar(keyCode, carbonModFlags);
	
	/* get virtualkey from keycode */
	char vk = keyCodeToChar(keyCode, 0);
	
	//NSLog(@"%c=%d", vk, keyCode);
	
	if (vk >= 'a' && vk <= 'z') {
		vk += 'A' - 'a';
	}

	//NSLog(@"type=%d, keyCode=%x, char=%c vk=%x", type, keyCode, c, vk);

	/* start rule matching */
	mstring = [self matchRules:vk character:c toDelete:&toDelete];
	
	CGEventSetIntegerValueField(event, kCGKeyboardEventAutorepeat, NO);
	
	/* have any match? */
	while (mstring || toDelete) {
		
		/* got match, set NO to eat */
		eventToReturn = nil;
		
		/* Need any to send deleKeyMagicte key? */
		while (toDelete--) {
			CGEventKeyboardSetUnicodeString(event, 0, nil);
			CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 51);
			CGEventTapPostEvent(proxy, event);
		}
		
		/* Do we have string to match? */
		if ([mstring length]){
			sendString(proxy, event, mstring);
			// Prevent from ...
			if (mstring.length == 1 && [mstring characterAtIndex:0] > 0x20 && [mstring characterAtIndex:0] < 0x7F) {
				break;
			}
			[mstring release];
			/* Try rule matching again */
			mstring = [self matchRules:'\0' character:'\0' toDelete:&toDelete];
		} else {
			mstring = nil;
			toDelete = 0;
		}

	}
	
	/* No rule is matched? */
	if (eventToReturn) {
		
		/* Is any mod key is pressed? */
		if ( (carbonModFlags&controlKey) || (carbonModFlags&optionKey) || (carbonModFlags&cmdKey) ) {
			
			/* reset the editor */
			[m_internalEditor reset];
		}
	}
	
	return eventToReturn;
}

-(BOOL) checkVirtualKey:(char)vk vkeys:(NSArray*)vkeys
{
	BOOL cmd=(carbonModFlags&cmdKey) ? YES : NO,
	option=(carbonModFlags&optionKey) ? YES : NO,
	control=(carbonModFlags&controlKey) ? YES : NO,
	shift=(carbonModFlags&shiftKey) ? YES : NO;
	
	if ( [vkeys count] == 0 ) {
		if ( cmd || control || option ){
			return NO;
		} else {
			return YES;
		}

	}
	
	NSEnumerator *e = [vkeys objectEnumerator];
	NSNumber *vks;
	
	while (vks = [e nextObject]) {
		//NSLog(@"vk_input=%x, vk=%x", vk, [vks shortValue]);
#define CHECK_MOD(a) \
if (a == NO) { \
	return NO; \
} else { \
	a = NO; \
}
		NSUInteger iVKS = [vks shortValue];
		switch (iVKS) {
			case cmdKey:	case 0xA2:	case 0xA3:	case 0x11: // (windows control)
				CHECK_MOD(cmd);
				break;
			case controlKey:
				CHECK_MOD(control);
				break;
			case optionKey:	case 0xA4:	case 0xA5:	case 0x12: // (windows alt)
				CHECK_MOD(option);
				break;
			case shiftKey:	case 0xA1:	case 0xA0:	case 0x10: // (windows shift)
				CHECK_MOD(shift);
				break;
			default:
				if (iVKS != vk) {
					return NO;
				}
				break;
		}
	}
#undef CHECK_MOD
	// If one of the keys is pressed and no virutalkey(to be matched) is matched
	if ( cmd || control || option || shift ) {
		return NO;
	}
	return YES;
}

-(NSString*) matchRules:(char)vk character:(char)c toDelete:(NSUInteger*)toDelete
{
	//NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	*toDelete = 0;
	
	[m_internalEditor appendCharacter:(UniChar)c];
	
	NSEnumerator *e = [expendedRules objectEnumerator];
	ruleInfoLHS * ruleInfo;
	
	while (ruleInfo = [e nextObject]) {
		
		if ([ruleInfo RegexString] == nil) {
			//NSLog(@"%@", [ruleInfo strRegex]);
			continue;
		}
		NSString * strContext = [m_internalEditor getLastString:[ruleInfo LengthOfStringToMatch]];
		
		if (strContext == nil) {
			continue;
		}
		
		NSEnumerator *ee = [[ruleInfo Switches] objectEnumerator];
		id Switch;
		
		while (Switch = [ee nextObject]) {
			if ([m_internalEditor stateOfSwitch:[Switch intValue]] == NO) {
				goto NEXT;
			}
		}
		
		// check for virutal keys
		if ( [self checkVirtualKey:vk vkeys:[ruleInfo VKeys] ] == NO ) {
			continue;
		}
		
		if ([[ruleInfo VKeys] count] && c) {
			[m_internalEditor delete:1];
			strContext = [m_internalEditor getLastString:[ruleInfo LengthOfStringToMatch]];
			//NSLog(@"%d", [strContext length]);
		}
		
		NSArray * matchedArray = [strContext captureComponentsMatchedByRegex:[ruleInfo RegexString]];
		if ([matchedArray count] || ![[ruleInfo RegexString] length]) {
			//NSLog(@"ruleGotMatched:%@", ruleInfo.strRegex);
			if (c) {
				[m_internalEditor setAllSwitchOff];
			}
			NSUInteger ruleIndex = [ruleInfo Index];
			NSString * str = [self expendRHSPattern:ruleIndex matches:matchedArray charClasses:[ruleInfo CharClasses]];
			if (str == nil) {
				NSLog(@"ruleBroken- strRegex:%@ ruleIndex:%d", [ruleInfo RegexString], ruleIndex);
				continue;
			}
			
			if (strContext.length && str.length) {
				str = [strContext stringByReplacingOccurrencesOfRegex:[ruleInfo RegexString] withString:str];
				[str retain];
			} else if (str.length) {
				str = [@" " stringByReplacingOccurrencesOfRegex:@"." withString:str];
				[str retain];
			}
			if (![ruleInfo VKeys].count && c) {
				[m_internalEditor delete:1];
			}
			
#define CALC_LENGTH [ruleInfo LengthOfStringToMatch]-((![ruleInfo VKeys].count && c)?1:0)
			if ([ruleInfo LengthOfStringToMatch] && -CALC_LENGTH) {
				*toDelete = CALC_LENGTH;
				[m_internalEditor delete:*toDelete];
#undef CALC_LENGTH
			} else {
				*toDelete = 0;
			}
			if (str.length) {
				[m_internalEditor appendString:str];
			}
			//   [pool release];
			return str;
		} else if ([[ruleInfo VKeys] count] && c) {
			[m_internalEditor appendCharacter:c];
		}
	NEXT:;
	}
	
	if (c) {
		[m_internalEditor setAllSwitchOff];
	}
	
	if (c == 0x08) {
		if ([m_internalEditor getLength] == 2) {
			[m_internalEditor delete:2];
			return @"";
			
		} else {
			[m_internalEditor delete:2];
			
			if ([m_internalEditor getLength] > 0) {
				return @"";
			}
		}
	}
	//[pool release];
	return nil;
}

-(int) findLastOpenBracket:(NSString *)s
{
	for (int rit = [s length] - 1; rit >= 0; rit --){
		if ([s characterAtIndex:rit] == '('){
			if (rit && [s characterAtIndex:(rit-1)] == '\\')
				continue;
			return rit;
		}
	}
	return -1;
}

-(BOOL) appendVariableValue:(const NSMutableArray *) strings varIndex:(int)varIndex strReceiver:(NSMutableString *) strReceiver
{
	//NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	if (--varIndex > [strings count]){
		//[pool release];
		return NO;
	}
	
	NSString * str = [strings objectAtIndex:varIndex];
	int length = [str length];
	if (length < 1){
		//[pool release];
		return NO;
	}
	for (int i = 0; i < length; i++) {
		if ([str characterAtIndex:i] == OP_VARIABLE) {
			i++;
			if ([self appendVariableValue:strings  varIndex:[str characterAtIndex:i] strReceiver:strReceiver] == FALSE){
				//[pool release];
				return NO;
			}
		}
		else {
			[strReceiver appendFormat:@"%C", [str characterAtIndex:i]];
		}
		
	}
	
	//[pool release];
	return TRUE;
}

-(int) getIndexToReplace:(short) refIndex matches:(NSArray*) matches charClasses:(NSArray*) charClasses{
	NSEnumerator *e = [charClasses objectEnumerator];
	charClass *_charClass;
	while (_charClass = [e nextObject]) {
		if (_charClass.index == refIndex) {
			CFRange range = CFStringFind((CFStringRef)_charClass.strCharacters, (CFStringRef)[matches objectAtIndex:refIndex], 0);
			if (range.location > [_charClass.strCharacters length]) {
				return -1;
			}
			return range.location;
		}
	}
	return -1;
}

-(BOOL) expendRule:(NSMutableDictionary*)expendedRule bRule:(short*)rule strings:(NSMutableArray*)strings
		 doCapture:(BOOL)doCapture matched:(NSArray*)matches charClasses:(NSArray*)charClasses
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	BOOL outputRule = NO;
	
	NSMutableString * expendedRegex = [NSMutableString new];
	NSMutableArray * switches = [NSMutableArray new];
	NSMutableArray * vkeys = [NSMutableArray new];
	
	[expendedRegex autorelease];
	[switches autorelease];
	[vkeys autorelease];
	
	if (matches || doCapture==NO) {
		outputRule = YES;
	}
	
	NSUInteger bRuleLength = _wcslen(rule); // Binary rule length
	while (bRuleLength > 0) {
		
		int size, varIndex, refIndex, lastLength, lastBracket, charIndex;
		short modKind;
		preDefined * _preDefined;
		NSMutableString * strTemp;
		
		bRuleLength--;
		
#define RELEASE_AND_RETURN_NO [pool release]; return NO;
		
		switch (*rule++) {
			case OP_SWITCH:
				if (outputRule) {
					[m_internalEditor invertSwitch:*rule++];
					bRuleLength--;
				} else {
					[switches addObject:[NSNumber numberWithInt:*rule++]];
					bRuleLength--;
				}
				
				break;
			case OP_STRING:
				size = *rule++;
				bRuleLength--;
				if (doCapture){
					[expendedRegex appendString:@"("];
				}
				strTemp = [[NSMutableString alloc] initWithBytes:rule length:size * WCHAR_SIZE encoding:NSUTF16LittleEndianStringEncoding];
				[strTemp autorelease];
				[strTemp replaceOccurrencesOfRegex:@"[$^\\]\\[?(){}*+?.\\-\\\\|]" withString:@"\\\\$0"];
				[expendedRegex appendString:strTemp];
				
				if (doCapture) {
					[expendedRegex appendString:@")"];
				}
				rule += size;
				bRuleLength -= size;
				break;
			case OP_VARIABLE:
				varIndex = *rule++; bRuleLength--;
				lastLength = [expendedRegex length];
				if (doCapture){
					[expendedRegex appendString:@"("];
				}
				strTemp = [NSMutableString new];
				[strTemp autorelease];
				[self appendVariableValue:strings  varIndex:varIndex strReceiver:strTemp];
				[strTemp replaceOccurrencesOfRegex:@"[$^\\]\\[?(){}*+?.\\-\\\\|]" withString:@"\\\\$0"];
				[expendedRegex appendString:strTemp];
				
				if (doCapture){
					[expendedRegex appendString:@")"];
				}
				break;
			case OP_MODIFIER:
				modKind = *rule++; bRuleLength--;
				switch (modKind) {
					case OP_ANYOF:
						if ((lastBracket = [self findLastOpenBracket:expendedRegex]) != -1){
							[expendedRegex insertString:@"[" atIndex:(lastBracket+1)];
							[expendedRegex insertString:@"]" atIndex:([expendedRegex length]-1)];
						}
						break;
					case OP_NOTANYOF:
						if ((lastBracket = [self findLastOpenBracket:expendedRegex]) != -1){
							[expendedRegex insertString:@"[^" atIndex:(lastBracket+1)];
							[expendedRegex insertString:@"]" atIndex:([expendedRegex length]-1)];
						}						
						break;
					default:
						/* modKind seem to be back reference
						 modKind become index of back reference */
						if (matches == nil){
							RELEASE_AND_RETURN_NO
						}
						// reference is lager than max match count ?
						if (modKind > [matches count]) {
							RELEASE_AND_RETURN_NO
						}
						charIndex = [self getIndexToReplace:modKind matches:matches charClasses:charClasses];
						if (lastLength + charIndex >= [expendedRegex length]) {
							RELEASE_AND_RETURN_NO
						}
						NSString * charClassToBeReplaced = [expendedRegex substringFromIndex:lastLength];
						charClassToBeReplaced = [charClassToBeReplaced stringByReplacingOccurrencesOfRegex:@"\\\\(.)" withString:@"$1"];
						UniChar uChar = [charClassToBeReplaced characterAtIndex:charIndex];
						[expendedRegex setString:[expendedRegex substringToIndex:lastLength]];
						[expendedRegex appendFormat:@"%C", uChar];
						break;
				}
				break;
			case OP_PREDEFINED:
				if (doCapture){
					[expendedRegex appendString:@"("];
				}
				
				if (_preDefined = getPredefined(*rule++)) {
					[expendedRegex appendFormat:@"%s", [_preDefined Value]];
					bRuleLength--;
				}
				
				if (doCapture){
					[expendedRegex appendString:@")"];
				}
				break;
			case OP_REFERENCE:
				refIndex = *rule++; bRuleLength--;
				[expendedRegex appendFormat:@"$%d", refIndex];
				break;
			case OP_ANY:
				if (doCapture){
					[expendedRegex appendString:@"("];
				}
				
				[expendedRegex appendString:@"[\\x{21}-\\x{7d}\\x{ff}-\\x{ffff}]"];
				
				if (doCapture){
					[expendedRegex appendString:@")"];
				}
				break;
			case OP_AND:
				if (*rule++ != OP_PREDEFINED){
					RELEASE_AND_RETURN_NO
				}
				bRuleLength--;
				if (_preDefined = getPredefined(*rule++)) {
					[vkeys addObject:[NSNumber numberWithInt:[_preDefined Value][0]]];
					bRuleLength--;
				} else {
					RELEASE_AND_RETURN_NO
				}
				
				while (bRuleLength--) {
					if (*rule++ != OP_PREDEFINED){
						RELEASE_AND_RETURN_NO
					}
					if (_preDefined = getPredefined(*rule++)) {
						[vkeys addObject:[NSNumber numberWithInt:[_preDefined Value][0]]];
						bRuleLength--;
					} else {
						RELEASE_AND_RETURN_NO
					}
				}
				goto FINISH;
				break;
			default:
				NSLog(@"default: %x", *rule);
				break;
		}
	}
FINISH:
#undef RELEASE_AND_RETURN_NO
	[expendedRule setObject:expendedRegex forKey:@"regexString"];
	[expendedRule setObject:switches forKey:@"switches"];
	[expendedRule setObject:vkeys forKey:@"virutalkeys"];
	
	[pool release];
	return YES;
}


@end
