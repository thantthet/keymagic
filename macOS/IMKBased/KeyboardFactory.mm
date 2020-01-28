//
//  KeyboardFactory.m
//  KeyMagic (IMK-based)
//
//  Created by THANT THET KHIN ZAW on 28/01/2020.
//

#import "KeyboardFactory.h"
#import "KeyMagicUtil.h"
#import "Keyboard.h"
#import "KeyMagicEngine.h"

@interface KeyboardFactory ()

@property (nonatomic, strong) NSMutableArray *keyboards;

@end

@implementation KeyboardFactory

+ (instancetype)shared
{
    static KeyboardFactory *factory = nil;
    if (factory == nil) {
        factory = [[KeyboardFactory alloc] init];
    }
    return factory;
}

- (instancetype)init
{
    self = [super init];
    
    if (self) {
        _keyboards = [[NSMutableArray alloc] init];
        [self installToUserHomeDirectoryIfEmpty];
    }
    
    return self;
}

- (void)installToUserHomeDirectoryIfEmpty
{
    NSString *layoutDir = [self userKeyboardDirectory];
    if ([self getKeyboardPathsFrom:layoutDir].count == 0) {
        
        [NSFileManager.defaultManager createDirectoryAtURL:[NSURL fileURLWithPath:layoutDir]
                               withIntermediateDirectories:false attributes:nil error:nil];

        NSBundle * mainBundle = [NSBundle mainBundle];
        NSArray * paths = [mainBundle URLsForResourcesWithExtension:@"km2" subdirectory:nil];
        
        for (NSURL *path in paths) {
            NSString *toPath = [layoutDir stringByAppendingPathComponent:path.lastPathComponent];
            NSURL *toURL = [NSURL fileURLWithPath:toPath];
            [NSFileManager.defaultManager copyItemAtURL:path toURL:toURL error:nil];
        }
    }
}

- (NSString *)userKeyboardDirectory
{
    return [NSHomeDirectory() stringByAppendingPathComponent:@".keymagic"];
}

- (NSArray *)getKeyboardPathsFrom:(NSString*)directory
{
    NSMutableArray * paths = [NSMutableArray new];
    NSFileManager *localFileManager = [[NSFileManager alloc] init];
    NSDirectoryEnumerator *dirEnum = [localFileManager enumeratorAtPath:directory];
    
    NSString *file;
    while (file = [dirEnum nextObject]) {
        if ([[file pathExtension] isEqualToString: @"km2"]) {
            [paths addObject:[directory stringByAppendingPathComponent:file]];
        }
    }
    
    return paths;
}

- (void)rescanKeyboards
{
    [_keyboards removeAllObjects];
    
    NSMutableArray * allPaths = [NSMutableArray new];
    NSArray * paths;
    
    NSString *layoutDir = [self userKeyboardDirectory];
    paths = [self getKeyboardPathsFrom:layoutDir];
    [allPaths addObjectsFromArray:paths];

    for (NSString *path in allPaths) {
        const char * szPath = [path cStringUsingEncoding:NSUTF8StringEncoding];
        InfoList * infos = KeyMagicKeyboard::getInfosFromKeyboardFile(szPath);
        
        if (infos == NULL) {
            continue;
        }
        
        NSString * keyboardName = [KeyMagicUtil getKeyboardNameOrTitle:*infos pathName:path];

        Keyboard * keyboard = [Keyboard new];
        [keyboard setTitle:keyboardName];
        [keyboard setPath:path];
        
        [_keyboards addObject:keyboard];
        
        delete infos;
    }
}

- (NSArray *)keyboards
{
    return _keyboards;
}

@end
