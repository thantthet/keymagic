//
//  main.m
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 4/20/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>

int main(int argc, const char *argv[])
{	
	if (argc > 1) {
		if (strcmp("startup", argv[1]) == 0) {
			NSLog(@"Creating process");
			NSString * executablePath = [[NSBundle mainBundle] pathForAuxiliaryExecutable:@"KeyMagic"];
			[NSTask launchedTaskWithLaunchPath:executablePath arguments:[NSArray new]];
			NSLog(@"Terminating");
			return noErr;
		}
	}
	return NSApplicationMain(argc, argv);
}

