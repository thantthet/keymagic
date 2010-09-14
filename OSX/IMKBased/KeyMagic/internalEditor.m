//
//  internalEditor.m
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

#import "internalEditor.h"

#define MAX_INTERNAL_BUFFER 150

@implementation internalEditor

-(id) init
{	
	[super init];
	strContext = [NSMutableString new];
	dictSwitches = [NSMutableDictionary new];
	
	return self;
}

-(void) setString:(NSString *)string
{
	[strContext setString:string];
}

-(void) appendCharacter:(UniChar)CHAR
{	
	if (CHAR == '\0') {
		return;
	}
	[strContext appendFormat:@"%C", CHAR];
	//NSLog(@"internalEditor::appendCharacter CHAR:%x strContext:%@ length:%d", CHAR, strContext, [strContext length]);
}

-(void) appendString:(NSString*)str
{
	if ([str length] < 1) {
		return;
	}
	[strContext appendString:str];
	//NSLog(@"internalEditor::appendString str:%@ length:%d strContext:%@ length:%d", str, [str length], strContext, [strContext length]);
}

-(NSUInteger) getLength
{
	return [strContext length];
}

-(NSString*) getLastString:(NSUInteger)length
{	
	if ([strContext length] >= length) {
		return [strContext substringFromIndex:[strContext length] - length];
	} else {
		return nil;
	}
}

-(NSString*) getWholeString
{
	return strContext;
}

-(BOOL) invertSwitch:(NSUInteger)switchIndex
{	
	BOOL bOnOff = [[dictSwitches objectForKey:[NSNumber numberWithInt:switchIndex]] boolValue];
	[dictSwitches setObject:[NSNumber numberWithBool:!bOnOff] forKey:[NSNumber numberWithInt:switchIndex]];
	return bOnOff;
}

-(BOOL) stateOfSwitch:(NSUInteger)switchIndex
{
	return [[dictSwitches objectForKey:[NSNumber numberWithInt:switchIndex]] boolValue];
}

-(void) setAllSwitchOff
{
	[dictSwitches release];
	dictSwitches = [NSMutableDictionary new];
}

-(void) delete:(NSUInteger)count
{
	NSUInteger length;
	NSRange range;
	
	if (count < 1) {
		return;
	}
	
	length = [strContext length];
	if (length == 0) {
		return;
	}
	else if (length < count) {
		range = NSMakeRange(0, length);
	} else {
		range = NSMakeRange(length-count, count);
	}
	[strContext deleteCharactersInRange:range];
	//NSLog(@"internalEditor::delete count:%d strContext:%@ length:%d", count, strContext, [strContext length]);
}

-(void) reset
{
	[strContext setString:@""];
#ifdef DEBUG
	NSLog(@"internalEditor::reset");
#endif
}

-(NSString*) description
{
	return [strContext retain];
}

@end
