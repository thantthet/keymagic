//
//  internalEditor.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/8/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface internalEditor : NSObject {
	NSMutableString * strContext;
	NSMutableDictionary * dictSwitches;
}

-(id) init;
-(void) appendCharacter:(UniChar)CHAR;
-(void) appendString:(NSString*)str;
-(NSString*) getLastString:(NSUInteger)length;
-(BOOL) invertSwitch:(NSUInteger)switchIndex;
-(BOOL) stateOfSwitch:(NSUInteger)switchIndex;
-(void) setAllSwitchOff;
-(void) delete:(NSUInteger)count;
-(void) reset;
-(NSString*) description;

@end
