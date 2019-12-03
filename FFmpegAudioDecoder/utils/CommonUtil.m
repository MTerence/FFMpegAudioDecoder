//
//  CommonUtil.m
//  FFmpegAudioDecoder
//
//  Created by Ternence on 2019/11/22.
//  Copyright © 2019 machao. All rights reserved.
//

#import "CommonUtil.h"

@implementation CommonUtil

+ (NSString *)bundlePath:(NSString *)fileName{
    return [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:fileName];
}

+ (NSString *)documentsPath:(NSString *)fileName{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectiory = [paths objectAtIndex:0];
    
    return [documentsDirectiory stringByAppendingPathComponent:fileName];
}

@end
