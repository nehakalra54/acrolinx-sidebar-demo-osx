//
//  LocalStorageManager.h
//  Acrolinx Application
//
//  Created by Puneet Sanchiher on 12/07/16.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import <WebKit/WebKit.h>

@interface LocalStorageManager : NSObject

+ (instancetype)localStorageManagerWithScriptObject:(WebScriptObject *)scriptObject forURL:(NSURL *)url;
- (instancetype)initWithWithScriptObject:(WebScriptObject *)scriptObject forURL:(NSURL *)url;

@end


