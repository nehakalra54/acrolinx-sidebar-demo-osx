//
//  XMLCleaner.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 17.04.15.
//  Copyright (c) 2015 Acrolinx GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 *	@author Julian Weinert
 *
 *	The default substitution is one of the official whitespace substitution character "⍽" (U+237D)
 */
extern NSString *const XMLCleanerDefaultSubstitution;

/**
 *	@author Julian Weinert
 *
 *	A bitmask representing all cleaning options.
 */
typedef NS_OPTIONS(NSUInteger, XMLCleanOptions){
	/**
	 *	@author Julian Weinert
	 *
	 *	Removes <w:binData> tags
	 */
	XMLCleanOptionBinData		= 1 << 0,

    /**
     *	@author Puneet Sanchiher
     *
     *	Removes <image> tags
     */
    XMLCleanOptionImageData		= 1 << 1,

	/**
	 *	@author Julian Weinert
	 *
	 *	Removes all tags. Must always be the last option (64 options max).
	 */
	XMLCleanOptionAllTags		= 1 << 64
};

@interface XMLCleaner : NSObject

+ (NSString *)cleanXMLString:(NSString *)XMLString withOptions:(XMLCleanOptions)options;
+ (NSString *)cleanXMLString:(NSString *)XMLString withOptions:(XMLCleanOptions)options withSubstitution:(NSString *)substitution;

@end
