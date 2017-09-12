//
//  AcrolinxSidebarController.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 18.12.14.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AcrolinxWindow.h"
#import "AcrolinxSidebarInterface.h"

@class WebView;

@protocol AcrolinxSidebarControllerDelegate;

@interface AcrolinxSidebarController : NSWindowController <AcrolinxWindowHoverDelegate>

@property (strong) IBOutlet AcrolinxWindow *window;
@property (strong) IBOutlet WebView *webView;
@property (strong) IBOutlet NSButton *retryButton;
@property (strong) IBOutlet NSProgressIndicator *sidebarLoadingProgressIndicator;

@property (assign) id<AcrolinxSidebarControllerDelegate> delegate;
@property (assign) BOOL performGlobalCheckOnLoad;

@property (nonatomic, retain) AcrolinxSidebarInterface *JSInterface;

- (void)loadURL:(NSURL *)URL;
- (void)loadStartPageURL:(NSURL *)URL;
- (BOOL)isWindowVisible;

@end

@protocol AcrolinxSidebarControllerDelegate <NSObject>
@optional
- (void)pluginSidebarController:(AcrolinxSidebarController *)sidebarController mouseOver:(NSEvent *)theEvent;
- (void)pluginSidebarController:(AcrolinxSidebarController *)sidebarController mouseOut:(NSEvent *)theEvent;

@end
