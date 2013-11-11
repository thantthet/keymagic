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
        char * szName = new char[name.Size()+1];
        memset(szName, 0, name.Size()+1);
        
        memcpy(szName, name.Data(), name.Size());
        
		keyboardName = [NSString stringWithCString:szName encoding:NSUTF8StringEncoding];
        
        delete[] szName;
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
		NSData * data = [NSData dataWithBytes:icon.Data() length:icon.Size()];
		NSImage * image = [[NSImage new] initWithData:data];
		return image;
	}
	return nil;
}

@end
