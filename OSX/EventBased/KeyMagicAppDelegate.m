//
//  KeyMagicAppDelegate.m
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 4/20/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import "KeyMagicAppDelegate.h"

@implementation KeyMagicAppDelegate

-(void) awakeFromNib {
	
	eHandler = [EventHandler new];
	[eHandler HookKeyEvents];
	
	keyMenu = [[KeyboardMenu alloc] initWithHandler:eHandler];
	[keyMenu menuFromKeyboardFiles];
}

@end
