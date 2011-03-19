//
//  charClass.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/10/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface charClass : NSObject
{
	@public
	NSUInteger index;
	NSString * strCharacters;
}

@property(assign) NSUInteger index;
@property(assign) NSString* strCharacters;

@end