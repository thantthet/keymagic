//
//  ruleInfoRHS.h
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


@interface ruleInfoLHS : NSObject
{
@public
	NSUInteger ruleIndex;
	NSString * strRegex;
	NSUInteger strLengthToMatch;
	NSMutableArray* charClasses;
	NSArray * switches;
	NSArray * vkeys;
}
/*
@property(assign) NSUInteger ruleIndex;
@property(readonly) NSString* strRegex;
@property(readonly) NSUInteger strLengthToMatch;
@property(readonly) NSArray* charClasses;
@property(retain) NSArray* switches;
@property(retain) NSArray* vkeys;
*/
-(NSUInteger) Index;
-(void) setIndex:(NSUInteger)Index;
-(NSString*) RegexString;
-(NSUInteger) LengthOfStringToMatch;
-(NSArray*) CharClasses;
-(NSArray*) Switches;
-(void) setSwitches:(NSArray*)Switches;
-(NSArray*) VKeys;
-(void) setVKeys:(NSArray*)VKeys;

-init;
-(id) initWithRegex:(NSString*)_strRegex;
-(BOOL) getRegexInfo;

@end