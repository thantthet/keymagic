//
//  layoutFileReader.h
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/6/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#define OP_STRING 0x00f0
#define OP_VARIABLE 0x00f1
#define OP_REFERENCE 0x00f2
#define OP_PREDEFINED 0x00f3
#define OP_MODIFIER 0x00f4
#define OP_ANYOF 0x00f5
#define OP_AND 0x00f6
#define OP_NOTANYOF 0x00f7
#define OP_ANY 0x00f8
#define OP_SWITCH 0x00f9

#define WCHAR_SIZE 2

@interface classRule : NSObject
{
	@public
	short *ruleBinaryIn;
	short *ruleBinaryOut;
}

@property(assign) short* ruleBinaryIn;
@property(assign) short* ruleBinaryOut;

@end

@interface layoutFileReader : NSObject {
	@public
	NSMutableArray* m_strings;
	NSMutableArray* m_rules;
}

@property(assign) NSMutableArray* m_strings;
@property(assign) NSMutableArray* m_rules;

- (BOOL) readFromFile:(NSString *)fileName;

@end
