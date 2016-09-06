//
//  AcrolinxPluginTextEdit.m
//  AcrolinxPluginTextEdit
//
//  Created by Persistent on 22/08/16.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import "AcrolinxPluginTextEdit.h"
#import "TextEdit.h"

@interface AcrolinxPluginTextEdit () <AcrolinxPluginProtocol, AcrolinxSidebarDelegate>

@property (nonatomic, retain) NSString *ownFilePath;
@property (nonatomic, retain) AcrolinxPollingProxy *activeDocumentPollingProxy;

@end

static TextEditApplication *textEditApplication;

@implementation AcrolinxPluginTextEdit

- (instancetype)init {
    if (self = [super init]) {
        
    }
    return self;
}

- (TextEditApplication *)textEditApplication {
    @synchronized (self) {
        if (!textEditApplication) {
            textEditApplication = [SBApplication applicationWithBundleIdentifier:[[self class] targetBundleIdentifier]];
        }
    }
    return textEditApplication;
}

#pragma mark internal implementation

- (void)startPolling {
	[[self activeDocumentPollingProxy] startPolling];
}

- (void)stopPolling {
	[[self activeDocumentPollingProxy] stopPolling];
}


#pragma mark - AcrolinxPlugin overrides


#pragma mark - AcrolinxPluginProtocol

+ (NSString *)pluginName {
    return @"Acrolinx for TextEdit";
}

+ (NSString *)frontmostFilePath {
    @synchronized (self) {
        if (!textEditApplication) {
            textEditApplication = [SBApplication applicationWithBundleIdentifier:[[self class] targetBundleIdentifier]];
        }
    }
    
    if (![textEditApplication isRunning]) {
        textEditApplication = nil;
        return nil;
    }
    
    return [[[textEditApplication documents] objectAtIndex:0] path];
}

+ (NSArray *)supportedExtensions {
    return @[@"txt", @"rtf"];
}

+ (NSString *)clientSignature {
    //TBD: get new client signature for text edit sample
    return @"QWNyb2xpbnhQbHVnLWluRm9yT3V0bG9va01hYw";
}

+ (NSString *)targetBundleIdentifier {
    return @"com.apple.TextEdit";
}

+ (BOOL)checkCompatibility {
    //Put any check for plugin dependencies here, return no if it can't be run, Return NO so Acrolinx wont load this plugin
    return YES;
}

- (NSString *)openFileAtPath:(NSString *)filePath {
    LLog(@"NSWorkspace did open file.");
    [[self textEditApplication] activate];
    [[self sidebarController] showWindow:self];
    [NSApp unhide:self];
    _ownFilePath = filePath;
    
    _activeDocumentPollingProxy = [AcrolinxPollingProxy pollingProxyWithReponseHandler:^(AcrolinxPollingProxy *pollingProxy, id oldValue, id newValue) {
        
        LLog(@"Current file path '%@'" , [self ownFilePath]);
        
        LLog(@"--------------------\n%@\n%@\n--------------------", oldValue, newValue);
        
        if (newValue && ([newValue length] < 1)) {
            return;
        }
        if (oldValue && ([oldValue length] < 1)) {
            return;
        }
        
        LLog(@"polled new active document:\n'%@'\n'%@'", oldValue, newValue);
        if ((oldValue && (!newValue || ![newValue isEqualToString:oldValue]))
            || (newValue && (!oldValue || ![oldValue isEqualToString:newValue]))) {
            BOOL closed = YES;
            
            for (TextEditDocument *document in [[self textEditApplication] documents]) {
                if ([[document path] isEqualToString:[self ownFilePath]]) {
                    closed = NO;
                    break;
                }
            }
            
            if (closed) {
                LLog(@"fileClosed - close sidebar");
                [self fileClosed:[self ownFilePath]];
            }
            else if ([oldValue isEqualToString:[self ownFilePath]]) {
                if ([[self sidebarController] isWindowVisible]) {
                    LLog(@"hide sidebar");
                    [NSApp hide:self];
                }
            }
            else if ([newValue isEqualToString:[self ownFilePath]]) {
                if (![[self sidebarController] isWindowVisible]) {
                    LLog(@"show sidebar");
                    [NSApp unhide:self];
                }
            }
        }
        
    } andObject:self forKeyPath:@"activeDocumentPath" queryInitialValue:YES];
    
    [[self activeDocumentPollingProxy] startPollingWithTimeInterval:0.3];
    
    return filePath;
}


- (void)loadSidebarURL:(NSURL *)sidebarURL {
    [[self sidebarController] loadURL:sidebarURL];
}

- (void)startGlobalCheck {
    dispatch_async_main(^{
        NSError *error;
        NSString *fileContents = [NSString stringWithContentsOfFile:[self ownFilePath] encoding:NSUTF8StringEncoding error:&error];
        
        if (error) {
            NSLog(@"Error reading file: %@", error.localizedDescription);
            return;
        }

        [[[self sidebarController] JSInterface] performGlobalCheck:fileContents];
    });

}

- (void)fileDidSave {
    //If you want any acion on file save
}

#pragma mark - AcrolinxSidebarDelegate
- (void)sidebarLoaded {
    
    NSMutableDictionary *sidebarOptions = [self createSidebarOptionsForPlugin];
    
    // parameter for readonly sidebar
    [sidebarOptions setValue:@"true" forKey:@"readOnlySuggestions"];
    
    [[[self sidebarController] JSInterface] initializeSidebarWithOptions:sidebarOptions];
}

- (id)sidebarDidFindNewMatch:(id)match {
    return match;
}

- (void)sidebarDidSelectWithMatches:(NSArray *)matches {
    LLog(@"It's a read only side bar");
    return;
}

- (id)sidebarDidReplaceWithReplacements:(NSArray *)replacements {
    LLog(@"It's a read only side bar");
    return replacements;
}

- (NSString *)filePathOfDocument {
    return [self ownFilePath];
}

- (NSString *)formatOfDocument {
    return @"TEXT";
}


#pragma mark key-value coding getter method for activeDocumentName

- (NSString *)activeDocumentPath {
    if ([self textEditApplication] && [[self textEditApplication] isRunning]) {
        return [[[textEditApplication documents] objectAtIndex:0] path];
    }
    return nil;
}


@end

