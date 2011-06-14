//
//  Growl.m
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 6/8/11.
//  Copyright 2011 N/A. All rights reserved.
//

#import "Growl.h"


@implementation Growl

- (void) init
{
	[super init];
	
	NSBundle *myBundle = [NSBundle mainBundle];
	NSString *growlPath = [[myBundle privateFrameworksPath] stringByAppendingPathComponent:@"Growl.framework"];
	NSBundle *growlBundle = [NSBundle bundleWithPath:growlPath];
	
	if (growlBundle && [growlBundle load]) { // Register ourselves as a Growl delegate 
		[GrowlApplicationBridge setGrowlDelegate:self];
	} else {
		NSLog(@"Could not load Growl.framework");
	}
}

- (NSDictionary *) registrationDictionaryForGrowl
{
	NSArray *notifications = [NSArray arrayWithObject: @"Layout Switched"];
	NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
						  notifications, GROWL_NOTIFICATIONS_ALL,
						  notifications, GROWL_NOTIFICATIONS_DEFAULT, nil];
	
	return dict;	
}

@end
