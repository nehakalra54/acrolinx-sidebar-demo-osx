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

/** 
 * AcrolinxPlugin is base class for all the AcrolinxPlugin’s written for any target editors.
 *
 */
@interface AcrolinxPlugin : NSObject

/**
 AcrolinxSidebarController instance associated with the plugin.
 */

@property (nonatomic, retain) AcrolinxSidebarController *sidebarController;

@property (nonatomic, retain, readonly) NSUserDefaults *acrolinxUserDefaults;
@property (nonatomic, retain, readonly) NSString *currentCheckID;


+ (instancetype)sharedPlugin;


/**
 *	Returns the version of the installed plugin.
 *	@return A string representing the plugins version.
 */
+ (NSString *)pluginVersion;

/**
 *	Returns the bundle identifier of the installed plugin.
 *	@return A string representing the bundle identifier
 */
+ (NSString *)bundleIdentifier;

/**
 *	Creates and returns sidebar options.
 *
 *  @discussion This method can be called from AcrolinxSidebarDelegate method 'sidebarLoaded' to creates options needed
 *  for initialising the sidebar. Plugin can add more options as needed.
 *
 *	@return A string representing the bundle identifier
 */
- (NSMutableDictionary *)createSidebarOptionsForPlugin;


/**
 *
 *	Get the sidebar version string needed for the plugin.
 *	
 *	@discussion This method should not be overridden!
 *	 The implementation returns the version defined in the framework base class header.
 *	 This method is used by the Acrolinx Application to get the version string from the framework version linked to the plugin.
 *	 If the baseclimb is linked against another version, this avoids interface incompatibility.
 *
 *	@return The sidebar version string
 */
- (NSString *)neededSidebarVersionString;


/**
 *	File watchdog methods.
 *
 *	@discussion When a plugin is created for a file open in target editor, Acrolinx creates a file watchdog. 
 *  The watchdog would call AcrolinxPluginProtocol method fileDidSave if the plugin respondds to this selector.
 */


/**
 *	Put the file watchdog to sleep.
 *
 *	@discussion While the watchdog is asleep plugin won't receive fileDidSave call.
 */
- (void)sleepFileWatchDog;

/**
 *	Wake up the file watchdog.
 *
 *	@discussion While the watchdog is awake plugin would receive fileDidSave call whenever file associated to the plugin is saved.
 */
- (void)wakeupFileWatchDog;

/**
 *	Stop the file watchdog.
 *
 *	@discussion Call it if the file watchdog is no longer needed.
 */
- (void)putDownFileWatchDog;

/**
 *	Open AcrolinxSidebarController window.
 *
 *	@discussion Called by acrolinx application.
 */
- (void)openSidebarWindow;

/**
 *	Close AcrolinxSidebarController window.
 *
 *	@discussion Called by acrolinx application.
 */
- (void)closeSidebarWindow;


/**
 *	Called before closing AcrolinxSidebarController window.
 *
 *	@discussion Override if special handling is need by the plugin.
 */
- (void)sidebarWindowWillClose;

/**
 *	Called before terminating the sidebar.
 *
 *	@discussion Override this method to control the sidebar termination if needed.
 *  @param sender The message sender.
 *
 *  @return TRUE/FALSE
 */
- (BOOL)shouldTerminate:(id)sender;

/**
 *	Called before terminating the sidebar.
 *
 *  @param sender The message sender.
 *	@discussion Override this method to perform any clean-up needed before unloading the plugin.
 *
 *  @return TRUE/FALSE
 */
- (void)willTerminate:(id)sender;

/**
 *	Called to query if the sidebar window can be shown.
 *
 *	@discussion This method must be overridden to handle hiding/unhiding of sidebar window while user switches between windows.
 *  e.g. If the target application becomes active and front most document is associated with the plugin then sidebar window 
 *  must be shown.
 *
 *  @return TRUE/FALSE
 */
- (BOOL)shouldUnhideSidebar;

/**
 *	Tell application when a file is closed.
 *
 *  @param filePath A string representing the file to close.
 *
 *	@discussion This method should be called to tell Acrolinx Application when a file associated with plugin is closed.
 *  This would trigger the process of unloading the plugin.
 */
- (void)fileClosed:(NSString *)filePath;

- (NSString *)tempDir;
- (NSString *)contentPieceUUID;

@end
