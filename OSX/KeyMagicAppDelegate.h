//
//  KeyMagicAppDelegate.h
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 4/20/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EventHandler.h"

@interface KeyMagicAppDelegate : NSObject {
	NSStatusItem * statusItem;
	NSString * preFix;
	
	EventHandler *eHandler;
	inputProcessor * inProcessor;
}

@end
