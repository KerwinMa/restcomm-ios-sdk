/*
 * TeleStax, Open Source Cloud Communications
 * Copyright 2011-2015, Telestax Inc and individual contributors
 * by the @authors tag.
 *
 * This program is free software: you can redistribute it and/or modify
 * under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 * For questions related to commercial use licensing, please contact sales@telestax.com.
 *
 */

#import <Foundation/Foundation.h>

#import "RCConnectionDelegate.h"
#import "SipManager.h"

/** @file RCConnection.h */

/**
 * Connection State
 */
typedef enum
{
	RCConnectionStatePending = 0,  /**< Connection is in state pending */
	RCConnectionStateConnecting,  /**< Connection is in state connecting */
	RCConnectionStateConnected,  /**< Connection is in state connected */
	RCConnectionStateDisconnected  /**< Connection is in state disconnected */
} RCConnectionState;


extern NSString* const RCConnectionIncomingParameterFromKey;
extern NSString* const RCConnectionIncomingParameterToKey;
extern NSString* const RCConnectionIncomingParameterAccountSIDKey;
extern NSString* const RCConnectionIncomingParameterAPIVersionKey;
extern NSString* const RCConnectionIncomingParameterCallSIDKey;

@class SipManager;

/**
 *  RCConnection represents a call. An RCConnection can be either incoming or outgoing. RCConnections are not created by themselves but
 *  as a result on an action on RCDevice. For example to initiate an outgoing connection you call [RCDevice connect:delegate:] which instantiates
 *  and returns a new RCConnection. On the other hand when an incoming connection arrives the RCDevice delegate is notified with 
 *  [RCDeviceDelegate device:didReceiveIncomingConnection:] and passes the new RCConnection object that is used by the delegate to
 *  control the connection.
 *
 *  When an incoming connection arrives through [RCDeviceDelegate device:didReceiveIncomingConnection:] it is considered RCConnectionStateConnecting until it is either
 *  accepted with [RCConnection accept] or rejected with [RCConnection reject]. Once the connection is accepted the RCConnection transitions to RCConnectionStateConnected
 *  state (Important: the media part is not implemented yet; for now only the signaling in in place so even though the RCConnection is in connected 
 *  state the two parties cannot talk -we are working on that)
 *
 *  When an outgoing connection is created with [RCDevice connect:delegate:] it starts with state RCConnectionStatePending. Once it starts ringing on the remote party it
 *  transitions to RCConnectionStateConnecting. When the remote party answers it, the RCConnection state transitions to RCConnectionStateConnected.
 *
 *  Once an RCConnection (either incoming or outgoing) is established (i.e. RCConnectionStateConnected) media can start flowing over it (not implemented yet: as already 
 *  discussed media is dummy for now). DTMF digits can be sent over to the remote party using [RCConnection sendDigits:] (Not implemented yet). When done with the
 *  RCConnection you can disconnect it with [RCConnection disconnect].
 */
@interface RCConnection : NSObject<SipManagerConnectionDelegate>

/** 
 *  @abstract State of the connection.
 *
 *  @discussion A new connection created by RCDevice starts off RCConnectionStatePending. It transitions to RCConnectionStateConnecting when it starts ringing. Once the remote party answers it it transitions to RCConnectionStateConnected. Finally, when disconnected it resets to RCConnectionStateDisconnected.
 */
@property RCConnectionState state;

/**
 *  @abstract Direction of the connection. True if connection is incoming; false otherwise
 */
@property (nonatomic, getter=isIncoming) BOOL incoming;

/**
 *  @abstract Connection parameters (**Not Implemented yet**)
 */
@property (nonatomic, readonly) NSDictionary* parameters;

/**
 *  @abstract Delegate that will be receiving RCConnection events described at RCConnectionDelegate
 */
@property (nonatomic, assign) id<RCConnectionDelegate> delegate;

/**
 *  @abstract Is connection currently muted? If a connection is muted the remote party cannot hear the local party (**Not Implemented yet**)
 */
@property (nonatomic, getter = isMuted) BOOL muted;


/**
 *  @abstract Initialize a new RCConnection object
 *
 *  @param delegate Delegate of RCConnection that will be receiving RCConnection events (see RCConnectionDelegate)
 *
 *  @return Newly initialized object
 */
- (id)initWithDelegate:(id<RCConnectionDelegate>)delegate;

/**
 *  @abstract Accept an incoming connection that is ringing. The connection state changes to 'RCConnectionStateConnected'
 */
- (void)accept;

/**
 *  @abstract Ignore connection (**Not Implemented yet**)
 */
- (void)ignore;

/**
 *  @abstract Reject an incoming connection
 */
- (void)reject;

/**
 *  @abstract Disconnect a connection that is in state either connecting or connected
 */
- (void)disconnect;

/**
 *  @abstract Send DTMF tones over a connection that is in state 'RCConnectionStateConnected' (**Not Implemented yet**)
 *
 *  @param digits A string of digits that will be sent
 */
- (void)sendDigits:(NSString*)digits;

// avoid reference cycle
@property (weak) SipManager * sipManager;

@end
