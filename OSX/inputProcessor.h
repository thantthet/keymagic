//
//  inputProcessor.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/10/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>
#import "internalEditor.h"
#import "layoutFileReader.h"
#import "ruleInfoRHS.h"


@interface inputProcessor : NSObject {
	CFMachPortRef keyboardMachPort;
	CFMachPortRef mouseMachPort;
	
	NSMutableArray* expendedRules;
	NSUInteger carbonModFlags;
	internalEditor* m_internalEditor;
	layoutFileReader* m_LFR;
	
	BOOL isEnable;
}

@property(assign) CFMachPortRef keyboardMachPort;
@property(assign) CFMachPortRef mouseMachPort;
@property(assign) NSUInteger carbonModFlags;
@property(assign) layoutFileReader* m_LFR;
@property(assign) internalEditor* m_internalEditor;

-init;
-(CGEventRef) handleEventKeyDown:(CGEventTapProxy)proxy type:(CGEventType)type event:(CGEventRef)event;
-(NSString*) matchRules:(char)vk character:(char)c toDelete:(NSUInteger*)toDelete;
-(BOOL) expendRule:(NSMutableDictionary*)expendedRule bRule:(short*)rule strings:(NSMutableArray*)strings
		 doCapture:(BOOL)doCapture matched:(NSArray*)matches charClasses:(NSArray*)charClasses;
-(void) expendLHSPattern;
-(NSString*) expendRHSPattern:(NSUInteger)ruleIndex matches:(NSArray*)matches charClasses:(NSArray*)charClasses;
-(void) setEnable:(BOOL)flag;
-(BOOL)loadKeyboard:(NSString*)path;
-(BOOL)Enable;

@end
