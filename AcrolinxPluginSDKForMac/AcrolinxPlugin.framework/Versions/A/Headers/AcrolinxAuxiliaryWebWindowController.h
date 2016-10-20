//
//  AcrolinxAuxiliaryWebWindowController.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 04/03/15.
//  Copyright (c) 2015 Acrolinx GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

@protocol AcrolinxAuxiliaryWebWindowDelegate;

@interface AcrolinxAuxiliaryWebWindowController : NSWindowController

@property (nonatomic, retain) IBOutlet WebView *webView;
@property (nonatomic, assign) id<AcrolinxAuxiliaryWebWindowDelegate>delegate;

+ (instancetype)auxiliaryController;
+ (instancetype)auxiliaryControllerWithFrameAutosaveName:(NSString *)frameAutosaveName;

@end

@protocol AcrolinxAuxiliaryWebWindowDelegate <NSObject>

- (void)auxiliaryWindowClosed:(AcrolinxAuxiliaryWebWindowController *)windowController;

@end
