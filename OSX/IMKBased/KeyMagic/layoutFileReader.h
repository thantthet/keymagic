//
//  layoutFileReader.h
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

//@property(assign) short* ruleBinaryIn;
//@property(assign) short* ruleBinaryOut;
-(short*) ruleBinaryIn;
-(void) setRuleBindaryIn:(short*)_ruleBinaryIn;
-(short*) ruleBinaryOut;
-(void) setRuleBindaryOut:(short*)_ruleBinaryOut;

@end

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

@interface layoutFileReader : NSObject {
	@public
	NSMutableArray* m_strings;
	NSMutableArray* m_rules;
	fileHeader fh;
}

//@property(assign) NSMutableArray* m_strings;
//@property(assign) NSMutableArray* m_rules;
-(NSMutableArray*) Strings;
-(NSMutableArray*) Rules;
-(fileHeader) FileHeader;

- (BOOL) readFromFile:(NSString *)fileName;

@end
