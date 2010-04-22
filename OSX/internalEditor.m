//
//  internalEditor.m
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/8/10.
//  Copyright 2010 N/A. All rights reserved.
//

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

-(NSString*) getLastString:(NSUInteger)length
{	
	if ([strContext length] >= length) {
		return [strContext substringFromIndex:[strContext length] - length];
	} else {
		return nil;
	}
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
	if (length < count){
		range = NSMakeRange(0, length);
	} else {
		range = NSMakeRange(length-count, count);
	}
	[strContext deleteCharactersInRange:range];
	//NSLog(@"internalEditor::delete count:%d strContext:%@ length:%d", count, strContext, [strContext length]);
}

-(void) reset
{
	[strContext release];
	strContext = [NSMutableString new];
	NSLog(@"internalEditor::reset");
}

-(NSString*) description
{
	return [strContext retain];
}

@end
