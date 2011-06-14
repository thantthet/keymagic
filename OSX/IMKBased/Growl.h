//
//  Growl.h
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 6/8/11.
//  Copyright 2011 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Growl/Growl.h>


@interface Growl : NSObject <GrowlApplicationBridgeDelegate> {
	

}

- (NSDictionary *) registrationDictionaryForGrowl;

@end
