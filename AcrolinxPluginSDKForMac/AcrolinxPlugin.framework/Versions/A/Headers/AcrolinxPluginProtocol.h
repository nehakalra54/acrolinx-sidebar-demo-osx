//
//  AcrolinxPluginProtocol.h
//  Acrolinx Sidebar
//
//  Created by Julian Weinert on 10.12.14.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/**
 *	@author Julian Weinert
 *
 *	A Plugin protocol that defines methods to implement in an plugins principal class.
 *
 *  @discussion Plugins conforming to this protocol must know what types of files it can handle.
 *   The Acrolinx Application determines which of the plugins can handle a certain file extension and tells the corresponding instance to open the file.
 */
@protocol AcrolinxPluginProtocol <NSObject>

@optional

/**
 *	@author Julian Weinert
 *
 *	Returns the name of the Plugin.
 *	Default implementation returns the bundle name.
 *
 *	@return A string representing the plugins name.
 */
+ (NSString *)pluginName;

/**
 *	@author Julian Weinert
 *
 *	Returns the path of the fronmost file.
 *	
 *	@discussion This should return the path of the frontmost file in the supported application.
 *	 This might return, but must otherwise represent a local file.
 *
 *	@return A string representing the frontmost file.
 */
+ (NSString *)frontmostFilePath;

/**
 *	@author Julian Weinert
 *
 *	Opens a local file in the editor the plugin is written for.
 *
 *  @discussion It is possible that a plugin saves the file to a different location (or format) and.
 *   For this case the plugin should return the new file path so that the baseclimb can watchDog the correct file.
 *
 *	@param A string representing a file to open.
 *  @return Apath the file was actually written to, returns nil on error.
 */
- (NSString *)openFileAtPath:(NSString *)filePath;


@required

/**
 *	@author Julian Weinert
 *
 *	Returns the version of the installed plugin.
 *	@return A string representing the plugins version.
 */
+ (NSString *)pluginVersion;

/**
 *	@author Julian Weinert
 *
 *	Returns a set of suiffixes this plugin is able to handle.
 *
 *	@return An array of instances of NSString.
 */
+ (NSArray *)supportedExtensions;

/**
 *	@author Julian Weinert
 *
 *	Returns the client signature for the plugin for licensing.
 *
 *	@return A string containing the signature.
 */
+ (NSString *)clientSignature;

/**
 *	@author Julian Weinert
 *
 *	Returns the bundle identifier of the application the plugin is written for.
 *
 *	@return A string representing the bundle identifier.
 */
+ (NSString *)targetBundleIdentifier;

/**
 *	@author Puneet Sanchiher
 *
 *	Used to determine the plugin compatibility. An Plugin can check for dependencies and act accordingly.
 *
 *  @return A boolean value indicating the plugins compatibility.
 */
+ (BOOL)checkCompatibility;

/**
 *	@author Julian Weinert
 *
 *	Loads the Acrolinx Sidebar HTML from the specified URL.
 *
 *	@param sidebarURL An URL of sidebar HTML.
 */
- (void)loadSidebarURL:(NSURL *)sidebarURL;

/**
 *	@author Julian Weinert
 *
 *	Called when the sidebar JS is ready to run.
 */
- (void)sidebarLoaded;

/**
 *	@author Julian Weinert
 *
 *	Tells the plugin to perform a global check.
 */
- (void)startGlobalCheck;

/**
 *	@author Julian Weinert
 *
 *	Called when the file was saved by its editor.
 */
- (void)fileDidSave;

/**
 *	@author Julian Weinert
 *
 *	Called when the file was changed by its editor. This is required for the "check-as-you-type" feature.
 */
//- (void)fileDidChange;

@end
