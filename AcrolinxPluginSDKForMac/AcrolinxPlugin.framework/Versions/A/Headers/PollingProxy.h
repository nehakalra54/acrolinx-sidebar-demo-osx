//
//  AcrolinxPollingProxy.h
//  AcrolinxPlugin
//
//  Created by Julian Weinert on 09.06.14.
//  Copyright (c) 2016 Acrolinx GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AcrolinxPollingProxy : NSObject

typedef NS_ENUM(BOOL, kJWPollingResult) {
	kJWPollingResultFailure,
	kJWPollingResultSuccess
};

typedef kJWPollingResult (^ JWPollingQueryHandler)(AcrolinxPollingProxy *pollingProxy);
typedef void			 (^ JWPollingResponseHandler)(AcrolinxPollingProxy *pollingProxy, id oldValue, id newValue);

extern JWPollingQueryHandler const JWPollingQueryHandlerDefault;

@property (atomic, assign, readonly, getter=isPollingPaused) BOOL pollingPaused;

@property (nonatomic, copy) JWPollingQueryHandler pollingQueryHandler;
@property (nonatomic, copy) JWPollingResponseHandler pollingResponseHandler;

@property (nonatomic, copy, readonly) NSString *observedKeyPath;
@property (nonatomic, assign) NSTimeInterval pollingInterval;
@property (nonatomic, copy, readonly) id lastQueriedObject;
@property (nonatomic, assign) id observedObject;

+ (instancetype)pollingProxyWithReponseHandler:(JWPollingResponseHandler)responseHandler andObject:(id)object forKeyPath:(NSString *)keyPath;
+ (instancetype)pollingProxyWithReponseHandler:(JWPollingResponseHandler)responseHandler andObject:(id)object forKeyPath:(NSString *)keyPath queryInitialValue:(BOOL)qInitVal;

+ (instancetype)pollingProxiWithQueryHandler:(JWPollingQueryHandler)queryHandler andResponseHandler:(JWPollingResponseHandler)responseHandler;
+ (instancetype)pollingProxiWithQueryHandler:(JWPollingQueryHandler)queryHandler andResponseHandler:(JWPollingResponseHandler)responseHandler forTimeInterval:(NSTimeInterval)interval;
+ (instancetype)pollingProxiWithQueryHandler:(JWPollingQueryHandler)queryHandler andResponseHandler:(JWPollingResponseHandler)responseHandler forTimeInterval:(NSTimeInterval)interval startPolling:(BOOL)startPolling;

- (instancetype)initWithQueryHandler:(JWPollingQueryHandler)queryHandler andResponseHandler:(JWPollingResponseHandler)responseHandler;
- (instancetype)initWithQueryHandler:(JWPollingQueryHandler)queryHandler andResponseHandler:(JWPollingResponseHandler)responseHandler forTimeInterval:(NSTimeInterval)interval;
- (instancetype)initWithQueryHandler:(JWPollingQueryHandler)queryHandler andResponseHandler:(JWPollingResponseHandler)responseHandler forTimeInterval:(NSTimeInterval)interval startPolling:(BOOL)startPolling;

- (NSTimeInterval)tolerance						NS_AVAILABLE_MAC(10_9);
- (void)setTolerance:(NSTimeInterval)tolerance	NS_AVAILABLE_MAC(10_9);

- (id)associatedObjectForKey:(id)key;
- (void)removeAssociatedObjectForKey:(id)key;
- (void)setAssociatedObject:(id)object forKey:(id<NSCopying>)key;

- (void)startPolling;
- (void)startPollingWithTimeInterval:(NSTimeInterval)interval;

- (void)pausePolling;
- (void)resumePolling;

- (void)stopPolling;

- (void)addResponseHandler:(JWPollingResponseHandler)responseHandler;
- (void)removeResponseHandler:(JWPollingResponseHandler)responseHandler;

@end
