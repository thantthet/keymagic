//
//  layoutFileReader.m
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

#import "layoutFileReader.h"

@implementation classRule

-(short*) ruleBinaryIn {
	return ruleBinaryIn;
}
-(void) setRuleBindaryIn:(short*)_ruleBinaryIn {
	ruleBinaryIn = _ruleBinaryIn;
}

-(short*) ruleBinaryOut {
	return ruleBinaryOut;
}
-(void) setRuleBindaryOut:(short*)_ruleBinaryOut {
	ruleBinaryOut = _ruleBinaryOut;
}

-(NSString *) description
{
	NSMutableString * des = [NSMutableString new];
	
	short* rIn = ruleBinaryIn;
	short* rOut = ruleBinaryOut;
	
	[des appendString:@"IN:"];
	while (*rIn) {
		[des appendFormat:@"%x ", *rIn++];
	}
	[des appendString:@"OUT:"];
	while (*rOut) {
		[des appendFormat:@"%x ", *rOut++];
	}
	return [des autorelease];
}

@end



@implementation layoutFileReader

-(NSMutableArray*) Strings {
	return m_strings;
}

-(NSMutableArray*) Rules {
	return m_rules;
}

-(fileHeader) FileHeader {
	return fh;
}

- (BOOL) readFromFile:(NSString *)fileName
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	if (fileName == nil)
	{
		NSLog(@"Keyboard Layout File name cannot be empty" );
		return NO;
	}
	
	NSInputStream * nis = [NSInputStream inputStreamWithFileAtPath:fileName];
	//[nis autorelease];
	[nis open];
	if ([nis hasBytesAvailable] == NO){
		NSLog(@"Cannot open the Keyboard Layout File: %@", [[nis streamError] localizedFailureReason]);
		[pool release];
		return NO;
	}
	
	[nis read:(uint8_t *)&fh maxLength:sizeof(fileHeader)];
	// check if it is keymagic keyboard layout file
	if (fh.cMagicCode[0] != 'K' || fh.cMagicCode[1] != 'M' || 
		fh.cMagicCode[2] != 'K' || fh.cMagicCode[3] != 'L') {
		[pool release];
		NSLog(@"Invalid Keyboard Layout File: %@", fileName);
		return NO;
	}
	
	/* get total count of strings */
	m_strings = [[NSMutableArray alloc] init];
	
	for (int i = 0; i < fh.shNumOfString; i++)
	{
		short sLength;
		
		/* get string length */
		[nis read:(uint8_t*)&sLength maxLength:sizeof(short)];
		
		if (sLength < 1) {
			continue;
		}
		
		/* allocate buffer with length */
		void * data = malloc(sLength * WCHAR_SIZE);
		/* read string into allocated buffer */
		[nis read:data maxLength:sLength * WCHAR_SIZE];
		
		/* Store as NSString */
		NSString * str = [[NSString alloc] initWithBytes:data length:sLength * WCHAR_SIZE encoding:NSUTF16LittleEndianStringEncoding];
		//NSLog(@"newString:%@ index:%d", str, [m_strings count]);
		/* Add new string into string array */
		[m_strings addObject:str];
		[str autorelease];
		/* free allocated buffer */
		free(data);
	}
	
	/* get count of rules */
	m_rules = [[NSMutableArray alloc] init];
	
	for (int i = 0; i < fh.shNumOfRules; i++)
	{
		short sLength;

		classRule *rule = [classRule new];
		
		/* get binary rule length */
		[nis read:(uint8_t*)&sLength maxLength:sizeof(short)];
		/* allocate buffer */
		short *ruleBinaryIn = malloc((sLength+1) * WCHAR_SIZE);
		/* read into buffer */
		[nis read:(uint8_t*)ruleBinaryIn maxLength:sLength * WCHAR_SIZE];
		/* make sure to terminate with zero */
		ruleBinaryIn[sLength] = '\0';
		
		[nis read:(uint8_t*)&sLength maxLength:sizeof(short)];
		short *ruleBinaryOut = malloc((sLength+1) * WCHAR_SIZE);
		[nis read:(uint8_t*)ruleBinaryOut	maxLength:sLength * WCHAR_SIZE];
		ruleBinaryOut[sLength] = '\0';
		
		//rule.ruleBinaryIn = ruleBinaryIn;
		[rule setRuleBindaryIn:ruleBinaryIn];
		//rule.ruleBinaryOut = ruleBinaryOut;
		[rule setRuleBindaryOut:ruleBinaryOut];
		
		[m_rules addObject:rule];
	}
	
	[pool release];
	return YES;
}
@end
