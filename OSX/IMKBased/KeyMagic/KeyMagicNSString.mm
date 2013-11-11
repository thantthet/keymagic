//
//  KeyMagicNSString.m
//
//Copyright (C) 2008  KeyMagic Project
//http://keymagic.googlecode.com
//
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#import "KeyMagicNSString.h"


@implementation NSString (KeyMagicString_Addition)

#if TARGET_RT_BIG_ENDIAN
const NSStringEncoding kEncoding_KeyMagicString =
CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32BE);
#else
const NSStringEncoding kEncoding_KeyMagicString =
CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
#endif

+ (NSString*)stringWithKeyMagicString:(const KeyMagicString&)kString
{
	return [self stringWithKeyMagicString:kString maximumLength:kString.length() fromEnd:NO];
}

+ (NSString*)stringWithKeyMagicString:(const KeyMagicString&)kString maximumLength:(NSUInteger)length
{
	return [self stringWithKeyMagicString:kString maximumLength:kString.length() fromEnd:NO];
}

+ (NSString*)stringWithKeyMagicString:(const KeyMagicString&)kString maximumLength:(NSUInteger)length fromEnd:(BOOL)fromEnd {
	if (length > kString.length()) {
		length = kString.length();
	}
	
	NSUInteger beginFrom = 0;
	if (fromEnd) {
		beginFrom = kString.length() - length;
	}
	
	char* data = (char*) (kString.data() + beginFrom);
	unsigned size = length * sizeof(wchar_t);
	
	NSString* result = [[NSString alloc] initWithBytes:data length:size
											   encoding:kEncoding_KeyMagicString];
	return result;
}

- (KeyMagicString)getKeyMagicString
{
	NSData* asData = [self dataUsingEncoding:kEncoding_KeyMagicString];
	return KeyMagicString((wchar_t*)[asData bytes], [asData length] /
						sizeof(wchar_t));
}

@end