//
//  KeyboardMenu.h
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 4/27/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "EventHandler.h"


@interface KeyboardMenu : NSObject {
	EventHandler * eHandler;
	
	NSStatusItem * statusItem;
	NSMenu * menu;
}

-(id) initWithHandler:(EventHandler*)_handler;
-(void) menuFromKeyboardFiles;

@end
