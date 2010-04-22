//
//  layoutFileReader.m
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/6/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import "layoutFileReader.h"

typedef struct _layoutOptions {
	BOOL trackCaps;
	BOOL autoBksp;
	BOOL eat;
	BOOL posBased;
} layoutOptions;

typedef struct _fileHeader {
	char cMagicCode[4];
	char bMajorVersion;
	char bMinorVersion;
	short shNumOfString;
	short shNumOfRules;
	layoutOptions layout;
} fileHeader;

typedef struct _string
{
	short sLength;
	wchar_t wcString[1];
} String;

@implementation classRule

@synthesize ruleBinaryIn;
@synthesize ruleBinaryOut;

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

@synthesize m_strings;
@synthesize m_rules;

- (BOOL) readFromFile:(NSString *)fileName
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
	fileHeader fh;
	layoutOptions layout;
	
	NSInputStream * nis = [NSInputStream inputStreamWithFileAtPath:fileName];
	//[nis autorelease];
	[nis open];
	if ([nis hasBytesAvailable] == NO){
		NSLog(@"%d", [nis streamStatus]);
		[pool release];
		return NO;
	}
	
	[nis read:(uint8_t *)&fh maxLength:sizeof(fileHeader)];
	// check if it is keymagic keyboard layout file
	if (fh.cMagicCode[0] != 'K' || fh.cMagicCode[1] != 'M' || 
		fh.cMagicCode[2] != 'K' || fh.cMagicCode[3] != 'L') {
		[pool release];
		return NO;
	}
	
	layout = fh.layout;
	
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
		
		rule.ruleBinaryIn = ruleBinaryIn;
		rule.ruleBinaryOut = ruleBinaryOut;
		
		[m_rules addObject:rule];
	}
	
	[pool release];
	return YES;
}
@end
