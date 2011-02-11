//
//  WideCharNSString.m
//  KeyMagic
//
//  Created by Thant Thet Khin Zaw on 2/5/11.
//  Copyright 2011 TTKZ. All rights reserved.
//

#import "KeyMagicNSString.h"


@implementation NSString (wstring_additions)

#if TARGET_RT_BIG_ENDIAN
const NSStringEncoding kEncoding_KeyMagicString =
CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
const NSStringEncoding kEncoding_KeyMagicString =
CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif

+(NSString*) stringWithKeyMagicString:(const KeyMagicString*)ws
{
	char* data = (char*)ws->data();
	unsigned size = ws->size() * sizeof(wchar_t);
	
	NSString* result = [[[NSString alloc] initWithBytes:data length:size
											   encoding:kEncoding_KeyMagicString] autorelease];
	return result;
}

-(KeyMagicString) getKeyMagicString
{
	NSData* asData = [self dataUsingEncoding:kEncoding_KeyMagicString];
	return std::wstring((wchar_t*)[asData bytes], [asData length] /
						sizeof(wchar_t));
}

@end