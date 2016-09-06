//
//  AcrolinxWindow.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 18.12.14.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol AcrolinxWindowHoverDelegate;

@interface AcrolinxWindow : NSWindow

@property (nonatomic, assign) id<AcrolinxWindowHoverDelegate> hoverDelegate;

@end

@protocol AcrolinxWindowHoverDelegate <NSObject>
@optional
- (void)window:(AcrolinxWindow *)window mouseOver:(NSEvent *)theEvent;
- (void)window:(AcrolinxWindow *)window mouseOut:(NSEvent *)theEvent;

@end
