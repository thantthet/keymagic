//
//  WideCharNSString.h
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 2/5/11.
//  Copyright 2011 TTKZ. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "KeyMagicTypes.h"

@interface NSString (KeyMagicString_Addition)
+(NSString*) stringWithKeyMagicString:(const KeyMagicString*)string;
-(KeyMagicString) getKeyMagicString;
@end
