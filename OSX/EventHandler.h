//
//  EventHandler.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/2/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "layoutFileReader.h"
#import "internalEditor.h"
#import "inputProcessor.h"

@interface EventHandler : NSObject {
	inputProcessor * inProcessor;
}

-(void) HookKeyEvents;
-(BOOL) loadKeyboard:(NSString*)path;
-(void) setEnable:(BOOL)enable;

@end
