//
//  IndexStore.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 02.04.15.
//  Copyright (c) 2015 Julian Weinert Softwareentwicklung. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface IndexStore : NSObject

+ (instancetype)indexStore;

- (void)addIndex:(NSInteger)index;
- (void)addShiftingIndex:(NSUInteger)index withShiftingWidth:(NSInteger)shiftingWidth;

- (void)removeAllIndexes;
- (void)removeIndex:(NSUInteger)index;
- (void)removeIndexesTo:(NSUInteger)toIndex;
- (void)removeIndexesFrom:(NSUInteger)fromIndex;

- (NSUInteger)shiftedIndex:(NSUInteger)index;
- (NSRange)shiftedRange:(NSRange)range;

- (NSInteger)shiftingWidthOfIndex:(NSUInteger)index;

- (NSArray *)indexesWithShiftingWidth:(NSInteger)shiftingWidth;
- (NSArray *)indexesWithShiftingWidthTo:(NSInteger)toShiftingWidth;
- (NSArray *)indexesWithShiftingWidthFrom:(NSInteger)fromShiftingWidth;

- (NSDictionary *)backingStore;

@end
