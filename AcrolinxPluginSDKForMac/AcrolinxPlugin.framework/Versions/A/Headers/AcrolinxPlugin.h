//
//  AcrolinxPlugin.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 06.01.15.
//  Copyright (c) 2015 Julian Weinert Softwareentwicklung. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

//! Project version number for AcrolinxPlugin.
FOUNDATION_EXPORT double AcrolinxPluginVersionNumber;

//! Project version string for AcrolinxPlugin.
FOUNDATION_EXPORT const unsigned char AcrolinxPluginVersionString[];

#ifndef LLog
 #define LLogF(format, ...) NSLog(@"%@:%i: %@", ((DEBUG == 2) ? [NSString stringWithFormat:@"%s", __FILE__] : [[NSString stringWithFormat:@"%s", __FILE__] lastPathComponent]), __LINE__, [NSString stringWithFormat:format, ##__VA_ARGS__])

 #ifdef DEBUG
  #define LLog(format, ...) LLogF(format, ##__VA_ARGS__)
 #else
  #define DEBUG 0
  #define LLog(format, ...) while(0){}
 #endif
#endif

// In this header, you should import all the public headers of your framework using statements like #import <AcrolinxPlugin/PublicHeader.h>

#define SIDEBAR_VERSION_STRING @"v14"
#define WEBCHECKER_VERSION_STRING @"v1"
#import "AcrolinxWindow.h"
#import "PollingProxy.h"
#import "AcrolinxPluginProtocol.h"
#import "AcrolinxSidebarInterface.h"
#import "AcrolinxSidebarController.h"
#import "AcrolinxAuxiliaryWebWindowController.h"
#import "IndexStore.h"
#import "XMLCleaner.h"
#import "XMLLookUpHelper.h"
#import "LocalStorageManager.h"


void dispatch_async_main(dispatch_block_t block);
void dispatch_sync_main(dispatch_block_t block);

@interface AcrolinxPlugin : NSObject

@property (nonatomic, retain) AcrolinxSidebarController *sidebarController;
@property (nonatomic, retain, readonly) NSUserDefaults *acrolinxUserDefaults;
@property (nonatomic, retain, readonly) NSString *currentCheckID;

+ (instancetype)sharedPlugin;

//Utility methods for different versions info
+ (NSString *)pluginVersion;
+ (NSString *)bundleIdentifier;

- (NSMutableDictionary *)createSidebarOptionsForPlugin;


/**
 *	@author Julian Weinert
 *
 *	Get the sidebar version string needed for the plugin.
 *	
 *	@discussion This method should not be overritten!
 *	 The implementation returns the version defined in the framework base class header.
 *	 This method is used by the baseclimb to get the version string from the framework version linked to the plugin.
 *	 If the baseclimb is linked against another version, this avoids interface incompatibility.
 *	@return
 */
- (NSString *)neededSidebarVersionString;

- (void)sleepFileWatchDog;
- (void)wakeupFileWatchDog;
- (void)putDownFileWatchDog;

- (void)openSidebarWindow;
- (void)closeSidebarWindow;
- (void)sidebarWindowWillClose;
- (BOOL)shouldTerminate:(id)sender;
- (void)willTerminate:(id)sender;

- (BOOL)shouldUnhideSidebar;

- (void)fileClosed:(NSString *)filePath;

- (NSString *)tempDir;
- (NSString *)contentPieceUUID;

@end
