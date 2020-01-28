//
//  KeyboardFactory.h
//  KeyMagic (IMK-based)
//
//  Created by THANT THET KHIN ZAW on 28/01/2020.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface KeyboardFactory : NSObject

+ (instancetype)shared;
- (void)rescanKeyboards;
- (NSArray *)keyboards;

@end

NS_ASSUME_NONNULL_END
