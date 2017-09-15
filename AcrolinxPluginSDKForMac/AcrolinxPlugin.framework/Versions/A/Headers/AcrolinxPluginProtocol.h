//
//  AcrolinxPluginProtocol.h
//  Acrolinx Sidebar
//
//  Created by Julian Weinert on 10.12.14.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/**
 *
 *	A Plugin protocol that defines methods to implement in an plugins principal class.
 *
 *  @discussion Plugins conforming to this protocol must know what types of files it can handle.
 *   The Acrolinx Application determines which of the plugins can handle a certain file extension and tells the corresponding instance to open the file.
 */
@protocol AcrolinxPluginProtocol <NSObject>

@optional

/**
 *
 *	Returns the name of the Plugin.
 *	Default implementation returns the bundle name.
 *
 *	@return A string representing the plugins name.
 */
+ (NSString *)pluginName;

/**
 *
 *	Returns the path of the frontmost file.
 *	
 *	@discussion This should return the path of the frontmost file in the supported application.
 *	 This might return, but must otherwise represent a local file.
 *
 *	@return A string representing the frontmost file.
 */
+ (NSString *)frontmostFilePath;

/**
 *
 *	Opens a local file in the editor the plugin is written for.
 *
 *  @param filePath A string representing a file to open.
 *  @discussion It is possible that a plugin saves the file to a different location (or format).
 *   For this case the plugin should return the new file path so that the application can watchDog the correct file.
 *
 *  @return A path the file was actually written to, returns nil on error.
 */
- (NSString *)openFileAtPath:(NSString *)filePath;


@required

/**
 *
 *	Returns the version of the installed plugin.
 *	@return A string representing the plugins version.
 */
+ (NSString *)pluginVersion;

/**
 *
 *	Returns a set of suffixes this plugin is able to handle.
 *
 *	@return An array of instances of NSString.
 */
+ (NSArray *)supportedExtensions;

/**
 *
 *	Returns the client signature for the plugin for licensing.
 *
 *	@return A string containing the signature.
 */
+ (NSString *)clientSignature;

/**
 *
 *	Returns the bundle identifier of the application the plugin is written for.
 *
 *	@return A string representing the bundle identifier.
 */
+ (NSString *)targetBundleIdentifier;

/**
 *
 *	Used to determine the plugin compatibility. A Plugin can check for dependencies and act accordingly.
 *
 *  @return A boolean value indicating the plugins compatibility.
 */
+ (BOOL)checkCompatibility;

/**
 *
 *  Used to determine the restriction for plugin based on sidebar version.
 *
 *  @return A string containing the minimum Sidebar version
 */
+ (NSString *)minimumSidebarVersion;

/**
 *
 *	Loads the Acrolinx Start Page index HTML from the specified URL.
 *
 *	@param startPageURL A URL of start page HTML.
 */
- (void)loadStartPage:(NSURL *)startPageURL;

/**
 *
 *	Called when the sidebar JS is ready to run.
 */
- (void)sidebarLoaded;

/**
 *
 *	Tells the plugin to perform a global check.
 */
- (void)startGlobalCheck;

/**
 *
 *	Called when the file was saved by its editor.
 */
- (void)fileDidSave;

@end
