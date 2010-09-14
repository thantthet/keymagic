//
//  ruleInfoRHS.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/10/10.
//  Copyright 2010 N/A. All rights reserved.
//

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

@property(assign) NSUInteger ruleIndex;
@property(readonly) NSString* strRegex;
@property(readonly) NSUInteger strLengthToMatch;
@property(readonly) NSArray* charClasses;
@property(retain) NSArray* switches;
@property(retain) NSArray* vkeys;

-init;
-(id) initWithRegex:(NSString*)_strRegex;
-(BOOL) getRegexInfo;

@end