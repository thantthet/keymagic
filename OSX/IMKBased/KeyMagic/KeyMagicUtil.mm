//
//  KeyMagicUtil.m
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 5/8/11.
//  Copyright 2011 N/A. All rights reserved.
//

#import "KeyMagicUtil.h"

@implementation KeyMagicUtil

+(NSString*) getKeyboardNameOrTitle:(const InfoList&)infos pathName:(NSString*) filePath
{
	NSString * keyboardName;
	if (infos.find('name') != infos.end()) {
		Info name = infos.find('name')->second;
		keyboardName = [NSString stringWithCString:name.data encoding:NSUTF8StringEncoding];
	} else {
		NSString * fileName = [filePath lastPathComponent];
		keyboardName = [fileName substringToIndex:[fileName length] - 4];
	}
	return keyboardName;
}

+(NSImage *) getIconImageFromKeyboard:(const InfoList&)infos
{
	if (infos.find('icon') != infos.end()) {
		Info icon = infos.find('icon')->second;
		NSData * data = [NSData dataWithBytes:icon.data length:icon.size];
		NSImage * image = [[[NSImage new] autorelease] initWithData:data];
		return image;
	}
	return nil;
}

@end
