//
//  ruleInfoRHS.m
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

#import "ruleInfoLHS.h"
#import "misc.h"
#import "RegexKitLite.h"
#import "charClass.h"

@implementation ruleInfoLHS

/*@synthesize ruleIndex;
@synthesize strRegex;
@synthesize strLengthToMatch;
@synthesize charClasses;
@synthesize	switches;
@synthesize vkeys;*/

-(NSUInteger) Index {
	return ruleIndex;
}
-(void) setIndex:(NSUInteger)Index {
	ruleIndex = Index;
}

-(NSString*) RegexString {
	return strRegex;
}

-(NSUInteger) LengthOfStringToMatch {
	return strLengthToMatch;
}

-(NSArray*) CharClasses {
	return charClasses;
}

-(NSArray*) Switches {
	return switches;
}
-(void) setSwitches:(NSArray*)Switches {
	switches = Switches;
}

-(NSArray*) VKeys {
	return vkeys;
}
-(void) setVKeys:(NSArray*)VKeys {
	vkeys = VKeys;
}

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
					NSString* characters = [[[strRegex substringWithRange:range] stringByReplacingOccurrencesOfRegex:@"\\\\(.)" withString:@"$1"] retain];
					[_charClass setStrCharacters:characters];
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