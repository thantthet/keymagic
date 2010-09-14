//
//  EventHandler.m
//  keyboardEvent
//
//  Created by Thant Thet Khin Zaw on 4/2/10.
//  Copyright 2010 N/A. All rights reserved.
//

#import <wchar.h>

#import "EventHandler.h"
#import "RegexKitLite.h"
#import "ruleInfoRHS.h"
#import "charClass.h"
#import "inputProcessor.h"
#import "misc.h"

@implementation EventHandler

static pid_t curPID;

CGEventRef MyEventTapCallBack (CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{	
	inputProcessor * inProcessor = refcon;
	
	if (CGEventGetIntegerValueField(event, kCGEventTargetUnixProcessID) == curPID) {
		NSLog(@"self event");
		return event;
	}
	
	switch (type) {
		case kCGEventKeyDown:
			return [inProcessor handleEventKeyDown:proxy type:type event:event];
			break;
		case kCGEventFlagsChanged:
			inProcessor.carbonModFlags = CocoaToCarbonFlags(CGEventGetFlags(event));
			if (inProcessor.carbonModFlags == 768) {
				[inProcessor setEnable:![inProcessor Enable]];
			}
			NSLog(@"type=%d, mod=%d", type, inProcessor.carbonModFlags);
			break;
		case kCGEventTapDisabledByTimeout:
			CGEventTapEnable(inProcessor.keyboardMachPort, TRUE);
			break;
		case kCGEventRightMouseDown:
		case kCGEventLeftMouseDown:
			[inProcessor.m_internalEditor reset];
			break;
		default:
			//NSLog(@"type=%d", type);
			break;
	}
	return event;
}

CGEventRef MouseHandler (CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
	inputProcessor * inProcessor = refcon;
	
	[inProcessor.m_internalEditor reset];
	
	return noErr;
}

-(void) setEnable:(BOOL)enable {
	CGEventTapEnable(inProcessor.keyboardMachPort, enable);
}

-(BOOL) loadKeyboard:(NSString *)path {
	return [inProcessor loadKeyboard:path];
}

-(void) HookKeyEvents
{
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	
	ProcessSerialNumber curPSN;
	GetCurrentProcess(&curPSN);
	GetProcessPID(&curPSN, &curPID);
	
	inProcessor = [inputProcessor new];
	
	CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);	
							//CGEventMaskBit(kCGEventRightMouseDown) |
							//CGEventMaskBit(kCGEventLeftMouseDown);
	
#define TEXT_EDIT_ONLY_
#ifdef TEXT_EDIT_ONLY
	
	NSMutableString * strListOfProcess = [NSMutableString new];
	[strListOfProcess autorelease];
	
	char buffer[1024];
	FILE* f = popen("ps -e", "r");
	while (!feof(f)) {
		fgets(buffer, 1024, f);
		[strListOfProcess appendFormat:@"%s", buffer];
	}
	pclose(f);
	
	NSString * strRegexSearchPSI = @"(\\d+).+/Applications/TextEdit.app/Contents/MacOS/TextEdit\\s-psn_(\\d+)_(\\d+)";
	
	NSArray * arrayPSI = [strListOfProcess captureComponentsMatchedByRegex:strRegexSearchPSI];
	
	if ([arrayPSI count] == 0){
		NSLog(@"TextEdit is not running.");
		//[pool release];
		exit(0);
		return;
	}
	
	ProcessSerialNumber psn;
	psn.highLongOfPSN = 
		strtod(CFStringGetCStringPtr((CFStringRef)[arrayPSI objectAtIndex:2], kCFStringEncodingMacRoman), nil);
	psn.lowLongOfPSN = 
		strtod(CFStringGetCStringPtr((CFStringRef)[arrayPSI objectAtIndex:3], kCFStringEncodingMacRoman), nil);
	
	inProcessor.keyboardMachPort = CGEventTapCreateForPSN(&psn, kCGHeadInsertEventTap,
													kCGEventTapOptionDefault, eventMask,
													MyEventTapCallBack, inProcessor);
	
	inProcessor.mouseMachPort = CGEventTapCreateForPSN(&psn, kCGTailAppendEventTap,
												 kCGEventTapOptionListenOnly,
												 CGEventMaskBit(kCGEventRightMouseDown) | CGEventMaskBit(kCGEventLeftMouseDown),
												 MyEventTapCallBack, inProcessor);
#else
	
	inProcessor.keyboardMachPort = CGEventTapCreate(kCGAnnotatedSessionEventTap, kCGTailAppendEventTap,
													kCGEventTapOptionDefault, eventMask, MyEventTapCallBack, inProcessor);
	
	inProcessor.mouseMachPort = CGEventTapCreate(kCGAnnotatedSessionEventTap, kCGTailAppendEventTap,
												 kCGEventTapOptionListenOnly,
												 CGEventMaskBit(kCGEventRightMouseDown) | CGEventMaskBit(kCGEventLeftMouseDown),
												 MyEventTapCallBack, inProcessor);
#endif
	
	if (inProcessor.keyboardMachPort == nil) {
		NSLog(@"Cannot create EventTap");
		//[pool release];
		return;
	}
	
	CFRunLoopSourceRef runLoopSource;
	
	runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, inProcessor.keyboardMachPort, 0);
	CFRunLoopAddSource(CFRunLoopGetMain(), runLoopSource, kCFRunLoopDefaultMode);
	CGEventTapEnable(inProcessor.keyboardMachPort, true);
	
	runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, inProcessor.mouseMachPort, 0);
	CFRunLoopAddSource(CFRunLoopGetMain(), runLoopSource, kCFRunLoopDefaultMode);
	CGEventTapEnable(inProcessor.mouseMachPort, true);
	
	EventTypeSpec eventType;
	eventType.eventClass = kEventClassMouse;
	eventType.eventKind = kEventMouseDown;
	
	[pool release];
}

@end
