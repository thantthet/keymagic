//
//  KeyMagicUtil.h
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 5/8/11.
//  Copyright 2011 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "KeyMagicTypes.h"

using namespace libkm;

@interface KeyMagicUtil : NSObject {

}

+(NSString*) getKeyboardNameOrTitle:(const InfoList&)infos pathName:(NSString*) filePath;
+(NSImage *) getIconImageFromKeyboard:(const InfoList&)infos;

@end
