//
//  charClass.h
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


@interface charClass : NSObject
{
	@public
	NSUInteger index;
	NSString * strCharacters;
}

-(NSUInteger) index;
-(void) setIndex:(NSUInteger)_index;
-(NSString*) strCharacters;
-(NSString*) setStrCharacters:(NSString *)_strCharacters;

@end