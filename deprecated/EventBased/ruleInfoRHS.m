//
//  ruleInfoRHS.m
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/10/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import "ruleInfoRHS.h"
#import "misc.h"
#import "RegexKitLite.h"
#import "charClass.h"

@implementation ruleInfoLHS

@synthesize ruleIndex;
@synthesize strRegex;
@synthesize strLengthToMatch;
@synthesize charClasses;
@synthesize	switches;
@synthesize vkeys;

-init
{
	return [self initWithRegex:nil];
}

-(id) initWithRegex:(NSString*)_strRegex
{	
	charClasses = nil;
	[_strRegex retain];
	strRegex = _strRegex;
	[self getRegexInfo];
	
	return self;
}

-(BOOL) getRegexInfo
{
	NSAutoreleasePool* pool = [NSAutoreleasePool new];
	
	int length = 0, refIndex = 0, startPos = -1;
	bool flagSquareBracket = false;
	charClass * _charClass = nil;
	
	if (charClasses) {
		[charClasses autorelease];
	}
	charClasses = [NSMutableArray new];
	
	for (int i = 0; i < [strRegex length]; i++) {
		switch ([strRegex characterAtIndex:i]) {
			case '[':
				if (!flagSquareBracket) {
					flagSquareBracket = true;
					length ++ ;
					_charClass = [charClass new];
					_charClass.index = refIndex;
					startPos = i;
				}
				break;
				
			case ']':
				if (flagSquareBracket) {
					flagSquareBracket = false;
					NSRange range = NSMakeRange(startPos+1, (i-1) - startPos);
					_charClass.strCharacters = [[[strRegex substringWithRange:range] stringByReplacingOccurrencesOfRegex:@"\\\\(.)" withString:@"$1"] retain];
					[charClasses addObject:_charClass];
					[_charClass autorelease];
				} else {
					length ++ ;
				}
				break;
				
			case '(':
				refIndex++;
				break;
			case ')':
				break;
				
			case '\\':
				i ++;
				if (!flagSquareBracket) {
					length++;
				}
				break;
			default:
				if (!flagSquareBracket) {
					length ++ ;
				}
		}
	}
	strLengthToMatch = length;
	[pool release];
	return YES;
}

-(NSString*)description
{
	return strRegex;
}

@end