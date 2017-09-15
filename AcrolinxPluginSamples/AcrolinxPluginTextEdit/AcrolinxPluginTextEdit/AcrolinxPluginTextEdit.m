//
//  AcrolinxPluginTextEdit.m
//  AcrolinxPluginTextEdit
//
//  Created by Puneet Sanchiher on 22/08/16.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import "AcrolinxPluginTextEdit.h"
#import "TextEdit.h"

NSString *const kHighlightTextScriptFormat = @"\
tell application \"TextEdit\" \n\
    activate \n\
    set color of front document to {0, 0, 0} \n\
    set color of characters %lu thru %lu of text of front document to {51834, 8835, 2732} \n\
end tell";

NSString *const kReplaceTextScriptFormat = @"\
tell application \"TextEdit\" \n\
    set startIndex to %lu + 1\n\
    set endIndex to %lu + 1 \n\
    set replacement to \"%@\" \n\
    activate \n\
    repeat with k from endIndex to startIndex + 1  by -1 \n\
        delete character k of text of front document \n\
    end repeat \n\
    set character startIndex of text of front document to replacement \n\
    set color of front document to {0, 0, 0} \n\
end tell";

@interface AcrolinxPluginTextEdit () <AcrolinxPluginProtocol, AcrolinxSidebarDelegate>

@property (nonatomic, retain) NSString *ownFilePath;
@property (nonatomic, retain) AcrolinxPollingProxy *activeDocumentPollingProxy;
@property (nonatomic, retain) IndexStore *indexStore;

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

- (IndexStore *)indexStore {
    @synchronized (self) {
        if (!_indexStore) {
            _indexStore = [IndexStore indexStore];
        }
        return _indexStore;
    }
}


#pragma mark internal implementation

- (void)startPolling {
	[[self activeDocumentPollingProxy] startPolling];
}

- (void)stopPolling {
	[[self activeDocumentPollingProxy] stopPolling];
}


#pragma mark - AcrolinxPlugin overrides

- (BOOL)shouldUnhideSidebar {
    if ([NSApp isHidden] && [[[self class] frontmostFilePath] isEqualToString:[self ownFilePath]]) {
        return TRUE;
    }
    return FALSE;
}


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
    return @"SW50ZWdyYXRpb25EZXZlbG9wbWVudERlbW9Pbmx5";
}

+ (NSString *)targetBundleIdentifier {
    return @"com.apple.TextEdit";
}

+ (BOOL)checkCompatibility {
    //Put any check for plugin dependencies here, return no if it can't be run, Return NO so Acrolinx wont load this plugin
    return YES;
}

+ (NSString *) minimumSidebarVersion {
    return @"14.5";
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


- (void)loadStartPage:(NSURL *)startPageURL {
    [[self sidebarController] loadStartPageURL:startPageURL];
}

- (void)startGlobalCheck {
    dispatch_async_main(^{
        NSString *fileContents = [[[[[self textEditApplication] documents] objectAtIndex:0] properties] valueForKey:@"text"];
        
        // Reset index store for every check
        _indexStore = nil;
        
        [[[self sidebarController] JSInterface] performGlobalCheck:fileContents];
    });

}

- (void)fileDidSave {
    //If you want any action on file save
}

#pragma mark - AcrolinxSidebarDelegate
- (void)sidebarLoaded {
    
    NSMutableDictionary *sidebarOptions = [self createSidebarOptionsForPlugin];
    
    // Parameter to make the sidebar readonly.
    //[sidebarOptions setValue:@"true" forKey:@"readOnlySuggestions"];
    
    [[[self sidebarController] JSInterface] initializeSidebarWithOptions:sidebarOptions];
}

- (id)sidebarDidFindNewMatch:(id)match {
    return match;
}

- (void)sidebarDidSelectWithMatches:(NSArray *)matches {
    
    /**
     * Here a plug-in typically tries to map the ranges represented by 'matches' into actual content. 
     * Then using selection API exposed by the editor it highlights appropriate ranges.
     *
     * Since TextEdit doesn't provide any such API, following code uses a script which changes the color of the relevant text range.
     *
     * This code assumes that there has not been any manual edits in the text after the check was run. Use of IndexStore makes sure 
     * that text offset changes due to any replacements done through sidebarDidReplaceWithReplacements are accounted for.
     *
     */
    
    NSUInteger unshiftedStartOfRange = [[[[matches firstObject] objectForKey:@"range"] firstObject] unsignedIntegerValue];
    NSUInteger unshiftedEndOfRange = [[[[matches lastObject] objectForKey:@"range"] lastObject] unsignedIntegerValue];
    
    NSUInteger startOfRange = [[self indexStore] shiftedIndex:unshiftedStartOfRange];
    NSUInteger endOfRange = startOfRange + (unshiftedEndOfRange - unshiftedStartOfRange);
    
    NSString *highlightTextScript = [NSString stringWithFormat:kHighlightTextScriptFormat, (unsigned long)startOfRange, (unsigned long)endOfRange];
    
    NSDictionary *errorDict;
    NSAppleScript *script = [[NSAppleScript alloc] initWithSource:highlightTextScript];
    NSAppleEventDescriptor *theResult = [script executeAndReturnError:&errorDict];
    if (!theResult) {
        LLog(@"Script to highlight text returned error: %@", [errorDict valueForKey:@"NSAppleScriptErrorBriefMessage"]);
        return ;
    }
    
    [[self textEditApplication] activate];
    return;
}

- (id)sidebarDidReplaceWithReplacements:(NSArray *)replacements {
    
    /*
    * This code assumes that there has not been any manual edits in the text after the check was run. Use of IndexStore makes sure that
    * text offset changes due to any replacements done through sidebarDidReplaceWithReplacements are accounted for.
    */

    for (NSDictionary *replacement in replacements) {
        LLog(@"rplcmnt: %@", replacement);
        
        NSUInteger unshiftedStartOfRange = [[[replacement objectForKey:@"range"] firstObject] unsignedIntegerValue];
        NSUInteger unshiftedEndOfRange = [[[replacement objectForKey:@"range"] lastObject] unsignedIntegerValue];
        
        NSUInteger startOfRange = [[self indexStore] shiftedIndex:unshiftedStartOfRange];
        NSUInteger endOfRange = startOfRange + (unshiftedEndOfRange - unshiftedStartOfRange) -1;
        NSString *replacementString = [replacement objectForKey:@"replacement"];
        
        NSString *replaceTextScript = [NSString stringWithFormat:kReplaceTextScriptFormat,
                                         (unsigned long)startOfRange, (unsigned long)endOfRange, replacementString];
        
        NSDictionary *errorDict;
        NSAppleScript *script = [[NSAppleScript alloc] initWithSource:replaceTextScript];
        NSAppleEventDescriptor *theResult = [script executeAndReturnError:&errorDict];
        if (!theResult) {
            LLog(@"Script to replace text returned error: %@", [errorDict valueForKey:@"NSAppleScriptErrorBriefMessage"]);
            return nil;
        }
        
        [[self indexStore] addShiftingIndex:unshiftedStartOfRange withShiftingWidth:([replacementString length] - (endOfRange - startOfRange + 1))];

    }

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

