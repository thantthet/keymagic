//
//  inputProcessor.h
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
#import <Carbon/Carbon.h>
#import "internalEditor.h"
#import "layoutFileReader.h"
#import "ruleInfoLHS.h"
#import "keyboard.h"

@interface inputProcessor : NSObject {
	CFMachPortRef keyboardMachPort;
	CFMachPortRef mouseMachPort;
	
	NSMutableArray* expendedRules;
	NSUInteger carbonModFlags;
	internalEditor* m_internalEditor;
	layoutFileReader* m_LFR;
	
	BOOL isEnable;
}

/*@property(assign) CFMachPortRef keyboardMachPort;
@property(assign) CFMachPortRef mouseMachPort;
@property(assign) NSUInteger carbonModFlags;
@property(assign) layoutFileReader* m_LFR;
@property(assign) internalEditor* m_internalEditor;*/
-(CFMachPortRef) KeyboardMachPort;
-(void) SetKeyboardMachPort:(CFMachPortRef)MachPort;
-(CFMachPortRef) MouseMachPort;
-(void) SetMouseMachPort:(CFMachPortRef)MachPort;
-(NSUInteger) CarbonModifierFlags;
-(void) SetCarbonModifierFlags:(NSUInteger)Flags;
-(layoutFileReader*) LayoutFileReader;
-(void) SetLayoutFileReader:(layoutFileReader*)LFR;
-(internalEditor*) InternalEditor;
-(void) SetInternalEditor:(internalEditor*)Editor;


-init;
-(BOOL)processInput:(short)keycode character:(char)character;
-(CGEventRef) handleEventKeyDown:(CGEventTapProxy)proxy type:(CGEventType)type event:(CGEventRef)event;
-(NSString*) matchRules:(char)vk character:(char)c toDelete:(NSUInteger*)toDelete;
-(BOOL) expendRule:(NSMutableDictionary*)expendedRule bRule:(short*)rule strings:(NSMutableArray*)strings
		 doCapture:(BOOL)doCapture matched:(NSArray*)matches charClasses:(NSArray*)charClasses;
-(void) expendLHSPattern;
-(NSString*) expendRHSPattern:(NSUInteger)ruleIndex matches:(NSArray*)matches charClasses:(NSArray*)charClasses;
-(void) setEnable:(BOOL)flag;
-(BOOL)loadKeyboard:(keyboard*)_keyboard;
-(BOOL)Enable;

@end
