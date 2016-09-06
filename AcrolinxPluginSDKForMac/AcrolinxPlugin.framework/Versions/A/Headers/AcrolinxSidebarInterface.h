//
//  AcrolinxSidebarInterface.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 07.01.15.
//  Copyright (c) 2015 Julian Weinert Softwareentwicklung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <WebKit/WebKit.h>
#import <WebKit/WebScriptObject.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "AcrolinxPluginProtocol.h"

@protocol AcrolinxSidebarDelegate;

@interface AcrolinxSidebarInterface : NSObject

@property (nonatomic, assign) id<AcrolinxPluginProtocol, AcrolinxSidebarDelegate>JSDelegate;
@property (nonatomic, assign) JSContextRef JSContext;

@property (assign) BOOL shouldCheckOnSidebarLoaded;

/**
 *	@author Julian Weinert
 *
 *	Creates a new JS interface instance with a given script object.
 *   Usally this will be the "window" object.
 *
 *	@param scriptObject An instance of WebScriptObject that will be interfaced.
 *  @param instanceName The variable name of the interface in the scriptObject.
 *	@return A JavaScript interface to the scriptObject parameter.
 */
+ (instancetype)interfaceWithScriptObject:(WebScriptObject *)scriptObject forInstanceName:(NSString *)instanceName;

/**
 *	@author Julian Weinert
 *
 *	Initiates a JS interface instance with a given script object.
 *   Usally this will be the "window" object.
 *
 *	@param scriptObject An instance of WebScriptObject that will be interfaced.
 *  @param instanceName The variable name of the interface in the scriptObject.
 *	@return A JavaScript interface to the scriptObject parameter.
 */
- (instancetype)initWithWithScriptObject:(WebScriptObject *)scriptObject forInstanceName:(NSString *)instanceName;

/**
 *	@author Julian Weinert
 *
 *	Initializes the sidebar JS with a given configuration.
 *   This is usally the documentDownloadInfo passed by the baseclimb.
 *
 *	@param sidebarOptions NSDictionary with init options for the sidebar.
 *  @return Any object returned by the JS application. Might be nil.
 */
- (id)initializeSidebarWithOptions:(NSDictionary *)sidebarOptions;

/**
 *	@author Julian Weinert
 *
 *	Tells the JavaScript sidebar object to perform a global document check.
 *	
 *	@param checkString NSString with contents of document.
 */
- (void)performGlobalCheck:(NSString *)checkString;

/**
 *	@author Puneet Sanchiher
 *
 *	Tells the JavaScript sidebar object to cancel document check.
 *
 */
- (void)cancelCheck;

@end

/**
 *	@author Julian Weinert
 *
 *	A protocol used to communicate between the sidebars JS and the baseclimb plugin.
 *
 *  @discussion This protocol contains methods for highlighting and replacing issues is the document.
 *   It's also needed to get the documents contents for checking and downloading attachments.
 */
@protocol AcrolinxSidebarDelegate <NSObject>

/**
 *	@author Julian Weinert
 *
 *	Called when the sidebar JS is ready to run.
 */
- (void)sidebarLoaded;

/**
 *	@author Julian Weinert
 *
 *	Called when the sidebar finds a new match in the checked document string.
 *
 *	@param match A dictionary representing the match
 *	@return <unknown>
 */
- (id)sidebarDidFindNewMatch:(id)match;

/**
 *	@author Julian Weinert
 *
 *	Called when the sidebar selects an issue.
 *
 *	@param matches An array of dictionaries representing a match.
 *	@return <unknown>
 */
- (void)sidebarDidSelectWithMatches:(NSArray *)matches;

/**
 *	@author Julian Weinert
 *
 *	Called when the sidebar replaces an issues.
 *
 *	@param replacements An array of dictionaries representing a replacement.
 *	@return <unknown>
 */
- (id)sidebarDidReplaceWithReplacements:(NSArray *)replacements;



/**
 *	@author Julian Weinert
 *
 *	Returns an input format string like "TEXT", "XML", etc.
 *	 Tells the server if and how to extract received content.
 *
 *	@discussion This method should return ONE format that the content returned by
 *	@code +[id<AcrolinxSidebarDelegate> contentsOfDocument]; @endcode
 *
 *	@return A string containing one input format.
 */
- (NSString *)formatOfDocument;

/**
 *	@author Julian Weinert
 *
 *	Returns the plugins current file path.
 *
 *	@return A string representing the files location.
 */
- (NSString *)filePathOfDocument;

@end
